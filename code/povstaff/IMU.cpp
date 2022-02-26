#include "IMU.h"

Adafruit_MPU6050 mpu;
Adafruit_Sensor *accel, *gyro;

bool IMUbegin(uint8_t address){
    if (! mpu.begin(address)) {
        Serial.println("Failed to find MPU6050 chip");
        return false;
    }
    //configure  IMU
    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    accel = mpu.getAccelerometerSensor();
    gyro = mpu.getGyroSensor();
    return true;
}

float IMUrotationSpeed(){
    sensors_event_t g;
    gyro->getEvent(&g);
    return (RAD_TO_DEG*sqrtf(g.gyro.x*g.gyro.x +g.gyro.y*g.gyro.y+g.gyro.z*g.gyro.z));
}

bool IMUatRest(){
    sensors_event_t a;
    accel->getEvent(&a);
    return ( (fabs(a.acceleration.x)<3.0) && (IMUrotationSpeed()<30.0)); //x-component of accel< 3m/s^2 and rotation speed < 30 deg/s

}
