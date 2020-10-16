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
        uint16_t batteryVoltage();
        void showVoltage();
        void clear();
        void show();
        void showLine(byte * line, uint16_t size);
        void blink();
        void setImage(BMPimage * image);
        void showNextLine();
        uint32_t timeSinceUpdate();
        void setPixel(uint16_t i, uint32_t c);
        float rotationSpeed(); //returns total rotation speed, in (deg/s)
        bool atRest();
        float getCurrent(BMPimage image); //returns average current in mA

    private:
        BMPimage * currentImage=NULL;
        int16_t currentLine=-1;
        uint32_t lastUpdate=0; //time in microseconds
        //sesnor objects
        Adafruit_Sensor *accel, *gyro;


};
extern Adafruit_DotStar _pixels;
extern POVstaff staff;
extern Adafruit_MPU6050 _mpu;

#endif
