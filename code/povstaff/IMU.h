#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>

#define RAD_TO_DEG 57.2958F


extern Adafruit_MPU6050 mpu;
extern Adafruit_Sensor *accel, *gyro;

bool IMUbegin(uint8_t address=0x68);

float IMUrotationSpeed(); //returns total rotation speed, in (deg/s)

bool IMUatRest();
