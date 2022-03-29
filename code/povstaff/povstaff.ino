/*
 * This file is part of POV Staff project by Alexander Kirillov <shurik179@gmail.com>
 * See github.com/shurik179/povstaff for details
 * Distributed under the terms of MIT license; see LICENSE file in the repository for details.
 *
 *  Requires the following libraries:
 *  FastLED
 *  Adafruit_TinyUSB
 *  Adafruit_SPIFlash
 *  Sd_Fat (adafruit fork)
 *  Adafruit_Sensor
 *  Adafruit_MPU6050
 *  POV library (from github.com/shurik179/pov-library)
 *
 *  For ItsyBitsy M4, make sure that in your Arduino IDE you have selected
 *  Tools->USB stack: TinyUSB
 *  Finally it is assumed that you have already created the FAT filesystem on your
 *  flash memory, using SdFat_format example sketch from Sd_Fat library (Adafruit fork)
 */
#include <FastLED.h>
#include <pov.h>
#include "IMU.h"
//number of pixels in your strip/wand
#define NUM_PIXELS 72
// Strip type. Common options are DOTSTAR (APA102, SK9822) and NEOPIXEL (WS2812B, SK6812 and
// compatible). For other options, see FastLED documentation
#define LED_TYPE DOTSTAR
// color order. For DOTSTAR (APA102), common order is BGR
// For NeoPixel (WS2812B), most common is  GRB
#define COLOR_ORDER BGR

#define IMU_ADDRESS 0x69


// Voltage sense pin
#define PIN_VSENSE A1
//battery info
#define VOLTAGE_MAX 4200 //maximal and minimal battery voltage, in mV
#define VOLTAGE_MIN 3300

//reference voltage for ADC, in mV
//nRF52840 ItsyBitsy uses internal reference voltage of 0.6*6=3.6 v
//all others use  3.3v
#ifdef ARDUINO_NRF52_ITSYBITSY
#define VREF 3600L
#else
#define VREF 3300L
#endif

#define MAXTIME 0xffffffff  //maximal possible uint32 value

// frame rate. Instead of using constant frame rate per second, we will adjust
// depending on rotation speed

// how many degrees of staff turn between successive lines?
#define DEG_PER_LINE 1.0f




/* Global Variables */
CRGB leds[NUM_PIXELS];
POV staff(NUM_PIXELS, leds);
uint32_t nextImageChange=0; //in milliseconds

uint32_t lastIMUcheck = 0; //when did we last check IMU speed, in ms
uint32_t lastPause = 0;    //when did we last pause/unpause staff, in ms
float speed=0.0;           //staff rotation speed, in deg/s



void setup(){
    #ifdef ARDUINO_NRF52_ITSYBITSY
    // For ItsyBitsy nRF52840, SPI Clock pin is #25, and SPI Data out (MOSI) is #24
    FastLED.addLeds<LED_TYPE, 24, 25, COLOR_ORDER>(leds, NUM_PIXELS);
    #else
    FastLED.addLeds<LED_TYPE, COLOR_ORDER>(leds, NUM_PIXELS);
    #endif


    pinMode(PIN_VSENSE, INPUT);
    //get supply voltage (battery or USB)
    //because of voltage divider, VREF corresponds to read value of  1023/2=512
    //since battery is connected to VHI through a diode, we need to add back 100mV lost on the diode.
    uint16_t voltage =(analogRead(PIN_VSENSE)*VREF)/512  + 100;

    //if we are connected to USB - which we get from looking at voltage -  go into upload mode!
    //note: in this case, there should be no Serial.begin() before this, and no delay()
    if (voltage>VOLTAGE_MAX + 100 ) {
        //we are connected to USB
        staff.begin(MODE_UPLOAD);
        //now, do nothing else - do not run loop(), just let TinyUSB work
        while (1) yield();
    } else {
        //otherwise, regular show
        staff.begin(MODE_SHOW);
        //debugging
        Serial.begin(115200);
        //Serial.println("Starting...");
        //delay(500);

        //show battery charge using the staff as bargraph
        staff.showValue((float)(voltage-VOLTAGE_MIN)/(VOLTAGE_MAX-VOLTAGE_MIN));
        delay(2000);
        //read the image list
        staff.addImageList("imagelist.txt");
        //determine when we will need to change the image
        nextImageChange=millis()+staff.currentDuration()*1000;
        if ( IMUbegin(IMU_ADDRESS) ) {
          Serial.println("IMU STARTED");
        } else {
          Serial.println("IMU failed to start");
          while(1) {
            staff.blink(CRGB::Red);
            staff.blink(CRGB::Blue);
          }
        }
    }
}

//note that loop() only runs in  MODE_SHOW
void loop(){
    if (millis()-lastIMUcheck > 100 ) {
        //let's check if staff is at rest. To avoid overloading the MCU, we only do it 10 times/sec.
        uint32_t now = millis();
        lastIMUcheck = now;
        bool atRest =  IMUatRest();
        if (staff.paused && (now - lastPause>1000) && !atRest){
            //if staff was paused for more than 1 sec and is now moving again, resume show
            staff.paused = false;
            //move to new image in the list
            staff.nextImage();
            //determine when we will need to change the image
            if (staff.currentDuration()==0){
                nextImageChange = MAXTIME;
            } else {
                nextImageChange = now + staff.currentDuration()*1000;
            }
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
        //also, get  rotation speed (in deg/s)
        speed = IMUrotationSpeed();
    }

    if (!staff.paused){
        //how much has the staff turned since last update?
        float rotAngle = speed * staff.timeSinceUpdate() * 0.000001;
        if (rotAngle>DEG_PER_LINE) staff.showNextLine();
        if (millis()>nextImageChange){
            //time to switch to next image
            staff.nextImage();
            //determine when we will need to change the image
            if (staff.currentDuration()==0){
                nextImageChange = MAXTIME;
            } else {
                nextImageChange = millis() + staff.currentDuration()*1000;
            }
        }
    }
}
