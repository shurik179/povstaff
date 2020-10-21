#ifndef _STAFF_H
#define _STAFF_H
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_DotStar.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include "config.h"
#include "flashstorage.h"
#include "bmpimage.h"


class POVstaff {
    public:
        void begin();
        void setPixel(uint16_t i, uint32_t c);
        void clear();
        void show();

        /* quickly blink each 8th LED red, for "I am alive" indication */ 
        void blink();

        /* returns supply voltage (higher of battery or USB) in mV */
        uint16_t batteryVoltage();

        /* Is the staff connected to USB power?         */
        bool USBconnected() { return (batteryVoltage()>VOLTAGE_MAX+100); }

        /* Shows battery voltage by lighting the LEDs on staff for 2 seconds */
        void showVoltage();
        
        /* Shows one line. line should be pointer to array which holds  pixel colors, in BGR order.
         *  size shoudl be size of array (number of pixels, not number of bytes)
         */
        void showLine(byte * line, uint16_t size);
        
        /* Set active image. The image must have been loaded to memory */        
        void setImage(BMPimage * image);
        
        /* Show next line of active image */
        void showNextLine();
        
        uint32_t timeSinceUpdate();
        
        float rotationSpeed(); //returns total rotation speed, in (deg/s)
        
        bool atRest();
        
        float getCurrent(BMPimage image); //returns average current in mA

    private:
        BMPimage * currentImage=NULL;
        int16_t currentLine=-1;
        uint32_t lastUpdate=0; //time in microseconds
        //sensor objects
        Adafruit_Sensor *accel, *gyro;


};
extern Adafruit_DotStar _pixels;
extern POVstaff staff;
extern Adafruit_MPU6050 _mpu;

#endif
