/*
 * This file is part of POV Staff project by Alexander Kirillov <shurik179@gmail.com>
 * See github.com/shurik179/povstaff for details
 * Distributed under the terms of MIT license; see LICENSE file in the repository for details. 
 * 
 * For overview of code and API, see README file in github.com/shurik179/povstaff/code
 * 
 */


#include "staff.h"

uint32_t lastCheck=0; //time in ms since last check of "staff at rest"
float speed=0; //current rotation speed

/* 
 * For selecting active mode. 
 * possible values are: MODE_SHOW (default when not connected to USB), MODE_UPLOAD, MODE_DEBUG
 */
int mode;


//setting this true will force the staff, if connected to USB, go into MODE_DEBUG mode
//otherwise, when connected to USB, the staff will go into upload mode. 
bool FORCE_DEBUG = false;


//list of all image files (can store up to MAX_FILES entries)
// initially empty, will be populated by reading from file imagelist.txt during setup()
BMPimageList imageList;
BMPimage * currentImg;
//indicates that the show must be paused
bool paused;
uint32_t timePaused=0; //when did we pause the show?

void setup() {

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    staff.begin();
     
    //set up mode - depending on whether we are connected to USB 
    if (!staff.USBconnected()) {
        // we are not connected to USB 
        // then we do not have a choice
        mode=MODE_SHOW; 
    } else {
        //we are connected to USB 
        if (FORCE_DEBUG) {
            mode=MODE_DEBUG;
        } else {
            mode=MODE_UPLOAD;
        }
    }
    //mode=MODE_UPLOAD;
    //now let us do appropriate setup depending on the mode 
    if (mode==MODE_UPLOAD) {
        // start mass storage protocol
        msc_init();
        Serial.begin(9600);
        delay(1000);
        Serial.println("Mass storage device started ");
        Serial.print("JEDEC ID: "); Serial.println(flash.getJEDECID(), HEX);
        Serial.print("Flash size: "); Serial.println(flash.size());
    } else {
        //we are running the show or debug  
        staff.showVoltage();
        if (mode==MODE_DEBUG) {
            Serial.begin(9600);
            delay(2000);
            Serial.println("Starting in debug mode...");
        }
        // read the  image list 
        imageList.addFromFile("imagelist.txt");
        if (mode==MODE_DEBUG) {
            imageList.print();
            Serial.print("Voltage: ");     Serial.print(staff.batteryVoltage()); Serial.println(" mV");
        }


        //load first image 
        currentImg=imageList.first();
        currentImg->load();
        staff.setImage(currentImg);
        //wait until we start rotating the staff
        paused=true;
    }

}

void loop(){
    if (mode==MODE_UPLOAD){
        //do nothing
        delay(10);
    } else {
        if (millis()-lastCheck>250){
            lastCheck=millis();
            //more than 0.25 sec since last check - let's update speed, using low-pass filter.
            speed=0.6*speed+0.4*staff.rotationSpeed();
            if ((speed<50) &&(staff.atRest()) && !paused)  {
                //staff is at rest - let's pause
                staff.clear();
                paused=true;
                timePaused=millis();
                //and pre-load next image from the list
                currentImg->unload();        //unload  from RAM 
                currentImg=imageList.next(); //move to next image in list
                currentImg->load();          //load to RAM
            } else if (paused && (speed>200)) {
                //restart the show with new image
                staff.setImage(currentImg);
                paused=false;                
            }
            //finally, let us check if  we have been paused longer than 30 seconds; if so, let's remind the user about it
            if ((millis()-timePaused>30000) && paused && (!staff.USBconnected())) {
                staff.blink();
                //reset paused time
                timePaused=millis();
            }
        }
        //check if it is time to show new line
        float rotAngle=speed*staff.timeSinceUpdate()*0.000001; //how much have we turned since last update, in degrees?
        if ( (!paused) && (rotAngle>DEG_PER_LINE) ) {
            if (mode==MODE_DEBUG) {delay(50);}
            staff.showNextLine();
        }

    }
}
