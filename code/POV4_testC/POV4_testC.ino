/*
POV staff project by shurik179

*/
//first, include all libraries
//NeoPixel 
#include <Adafruit_NeoPixel.h>


//Filesystem-related
#include <FS.h>
#include <LittleFS.h>
//WiFi and webserver
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
// getting access to the nice mime-type-table and getContentType()
#include <detail/RequestHandlersImpl.h>
#include <ESPxWebFlMgr.h>

//finally, our own custom library
#include <pov-esp32.h>
#include "LSM6.h"



// Now, various constants and defines

const word filemanagerport = 8080;
const char *ssid = "POVSTAFF1267";
const char *password = "LedsAreGreat";


// POV Staff details
#define PIN_VSENSE 9
#define NUM_PIXELS 72
// frame rate. Instead of using constant frame rate per second, we will adjust
// depending on rotation speed

// how many degrees of staff turn between successive lines?
#define DEG_PER_LINE 1.0f
// where is the lsit of images stored?
#define IMAGELIST "/imagelist.txt"
//Finally, some colors
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define YELLOW 0xFFFF00
#define PURPLE 0x8000FF


// Now, global objects

//the on-board NeoPixel
Adafruit_NeoPixel pixel(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);


ESPxWebFlMgr filemgr(filemanagerport); // we want a different port than the webserver
WiFiServer server(80);

LSM6 imu;
POV staff(NUM_PIXELS);
uint32_t lastIMUcheck = 0; //when did we last check IMU speed, in ms
uint32_t lastPause = 0;    //when did we last pause/unpause staff, in ms
uint32_t nextImageChange = 0;
float speed=0.0;           //staff rotation speed, in deg/s



void setup() {
    // the usual Serial stuff....
    Serial.begin(115200);
    //deal with on-board neopixel 
    pixel.begin();
    pixel.setPixelColor(0,GREEN);
    pixel.show();
    //initialize staff
    Wire.begin();
    staff.begin();
    delay(500);
    //measure battery voltage
    pinMode(PIN_VSENSE, INPUT);
    float voltage = analogReadMilliVolts(PIN_VSENSE)*0.001*2.0;
    staff.showValue(voltage/3.7);
    delay(2000);
    Serial.print("Voltage: "); Serial.println(voltage);
    //clear both the staff and neopixel 
    staff.blank();
    pixel.clear();pixel.show();
    // Open LittleFS file system on the flash
    if ( !LittleFS.begin(true) ) {
        Serial.println("Error: filesystem doesn't not exist. Please format LittleFS filesystem");
        while(1) {
            pixel.setPixelColor(0,RED);pixel.show();
            staff.blink();//blink red
        }
    }
    // start the imu
    bool imu_found = imu.init(); 
    if (!imu_found) {
        Serial.println("Failed to detect and initialize IMU!");
        pixel.setPixelColor(0,YELLOW);pixel.show();
        staff.blink(YELLOW);//blink yellow
    } else {
        imu.enableDefault();
        Serial.println("IMU Enabled");
        staff.blink(GREEN);      
    }
    //now, let us check which mode are we in
    delay(1000);
    imu.read();
    if (!imu_found || imu.isHorizontal()){
        //we need to go into uploader mode!
        // let us start wifi, web server etc
        WiFi.softAP(ssid, password);
        IPAddress myIP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(myIP);
        if (!MDNS.begin("povstaff")) {
              Serial.println("Error setting up MDNS responder!");
              while(1) {
                  pixel.setPixelColor(0,PURPLE);pixel.show();
                  staff.blink(PURPLE);//blink red
              }
        }
        Serial.println("mDNS responder started");
        server.begin();
        MDNS.addService("http", "tcp", 80);
        Serial.println("Server started");
        setupWebserver();
        filemgr.begin();
        Serial.println("Filemanager started");
        //light up staff in light green
        staff.blank();
        for (int i=0; 4*i<NUM_PIXELS;i++){
            staff.setPixel(4*i,0x004000); //set every 4th pixel light green
        }
        staff.show();
        pixel.setPixelColor(0,0x004000); pixel.show();
        // start webserver loop
        while(1){
            filemgr.handleClient();
            loopWebServer();
        }
    } else {
        //normal mode - showing files from the list
        staff.addImageList(IMAGELIST);
        Serial.println("imagelist added");
        pixel.setPixelColor(0,BLUE); pixel.show();
        staff.paused = true;
    }
}

//Note that loop will only be used in image show mode.
void loop() {
    //check IMU - do we need to pause the show?
    if (millis()-lastIMUcheck > 50 ) {
        //let's check if staff is at rest. To avoid overloading the MCU, we only do it 20 times/sec.
        uint32_t now = millis();
        lastIMUcheck = now;
        imu.read();
        //also, get  rotation speed (in deg/s)
        speed = imu.getSpeed();
        bool atRest=(imu.isVertical() && (speed < 30));
        if (staff.paused && (now - lastPause>1000) && !atRest){
            //if staff was paused for more than 1 sec and is now moving again, resume show
            staff.paused = false;
            //move to new image in the list - unless we are just starting and haven't yet shown any images, which can be detected from lastPause value
            if (lastPause == 0) {
                staff.firstImage();
            } else  {
                staff.nextImage();
            }
            //determine when we will need to change the image
            setNewImageChange();
            lastPause = now;
        } else if (!staff.paused && (now - lastPause>1000) && atRest){
            //stopping
            staff.paused = true;
            staff.blank();
            lastPause = now;
        } else if (staff.paused && (now-lastPause>30000)){
            //blink every 30 seconds to remind the user
            staff.blink();
            lastPause = now;
        }
    }
    if (!staff.paused){
        float rotAngle = speed * staff.timeSinceUpdate() * 0.000001;
        if (rotAngle>DEG_PER_LINE) staff.showNextLine();
        if (millis()>nextImageChange) {
            //time to switch to next image
            staff.nextImage();
            setNewImageChange();
        }
    }
}

void setNewImageChange(){
    if (staff.currentDuration() == 0) {
        nextImageChange = ULONG_MAX; //maximal possible unsigned 32-bit int
    } else {
        nextImageChange=millis()+staff.currentDuration()*1000;
    }
}
