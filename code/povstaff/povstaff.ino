/*
 * This file is part of POV Staff project by Alexander Kirillov <shurik179@gmail.com>
 * See github.com/shurik179/povstaff for details
 * Distributed under the terms of MIT license; see LICENSE file in the repository for details.
 *
 * For overview of code and API, see README file in github.com/shurik179/povstaff/code
 *
 */


#include "staff.h"
/*
 * Gloabl variables
 */
int         mode;        //staff operating mode. Possible values are
                         // MODE_SHOW (default when not connected to USB), MODE_UPLOAD, MODE_DEBUG
uint32_t    lastCheck=0; //time in ms since last check of "staff at rest"
float       speed=0;        //current rotation speed, in deg/s
//setting this true will force the staff, if connected to USB, go into MODE_DEBUG mode
//otherwise, when connected to USB, the staff will go into upload mode.
bool         FORCE_DEBUG = false;
BMPimageList imageList; // list of all image files (can store up to MAX_FILES entries)
                        // initially empty, will be populated by reading from file imagelist.txt during setup()
BMPimage *   currentImg;  // pointer to current image
uint32_t     imageStart=0; //when did we start showing current image? (in ms)
bool         paused;           //indicates that the show must be paused
uint32_t     timePaused=0; //when did we pause the show? (in ms)


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
    //mode=MODE_DEBUG;
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
        digitalWrite(LED_BUILTIN, LOW);
        staff.boardLED();
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
        if (millis()-lastCheck>200){
            //more than 0.2 seconds since last update
            lastCheck=millis();
            // update speed and if necessary pause/restart the show or load next image
            // setting all global varaibles
            updateState();
        }
        //show new line if necessary
        checkNextLine();
    }
}
/* ************************************************************
 *     Utility functions
 * ************************************************************
 */

//if necessary, show next line
void checkNextLine(){
    float rotAngle=speed*staff.timeSinceUpdate()*0.000001; //how much have we turned since last update, in degrees?
    if ( (!paused) && (rotAngle>DEG_PER_LINE) ) {
        if (mode==MODE_DEBUG) {delay(50);Serial.println("showing next line");}
        staff.showNextLine();
    }
}

//move to next image in list
void nextImage(){
    char name[MAX_FILENAME];
    staff.clear();
    currentImg->unload();        //unload  from RAM
    currentImg=imageList.next(); //move to next image in list
    currentImg->load();          //load to RAM
    staff.setImage(currentImg);
    if (mode==MODE_DEBUG) {
        currentImg->getFilename(name);
        Serial.print("Moving to next image: "); Serial.println(name);
    }
}

// update speed and check if we need to pause/restart the show,
// or move to next image
void updateState(){
    //update speed, using low-pass filter
    speed=0.6*speed+0.4*staff.rotationSpeed();
    //now, let us determine our next actions
    if (paused) {
        //check if we have started rotating - then we need to restart the show
        if (speed>150) {
            paused=false;
            imageStart=millis();
            if (mode==MODE_DEBUG){Serial.println("Restaring show after pause");}
        } else {
            //if we have been paused for more than 30 seconds, blink to remind the user
            if ((millis()-timePaused>30000)  && (!staff.USBconnected())) {
                staff.blink();
                //reset paused time
                timePaused=millis();
            }
        }
        return;
    }
    //if we are here, the show was not  paused
    // have the staff been stopped in horizontal position?
    bool staffStopped = (speed<50) &&(staff.atRest());
    if (staffStopped) {
        paused=true;
        timePaused=millis();
        //load next image into staff
        nextImage();
        return;
    }
    //have we been showing current image too long?
    bool imageExpired = (imageList.currentDuration()>0) && (millis()-imageStart>1000*imageList.currentDuration());
    if (imageExpired) {
        nextImage();
        imageStart=millis();
    }
    return;
}
