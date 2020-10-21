#ifndef STAFF_CONFIG_H
#define STAFF_CONFIG_H

/*
 *  This file contains various configuration  parameters  and defines
 */


// define various pins
#define PIN_VSENSE A1
//if NOT using SPI for controlling the stips, uncomment the two lines below and set appropriate values
//#define PIN_CLOCK 0
//#define PIN_DATA 1
//physical configuration of the staff
//number of pixels in each strip
#define NUM_PIXELS 72
#define NUM_STRIPS 2
//battery info
#define VOLTAGE_MAX 4200 //maximal and minimal battery voltage, in mV
#define VOLTAGE_MIN 3300

//buffer for storing image data in memory
#define BUF_SIZE 64000
//update speed, in degrees per line of the image
#define DEG_PER_LINE 1.0f

//for reading filelist of images to load
#define MAX_LINE_LENGTH 20
#define MAX_FILES 50
#define MAX_FILENAME 20
//some colors
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define OFF 0x000000
// current of different color LEDs, in mA
#define CURRENT_B 13
#define CURRENT_G 8
#define CURRENT_R 8
// constants for staff mode
#define MODE_SHOW 0
#define MODE_DEBUG 1
#define MODE_UPLOAD 2


#define RAD_TO_DEG 57.2958F

#endif
