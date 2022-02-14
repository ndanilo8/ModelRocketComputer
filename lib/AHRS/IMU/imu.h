#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <config.h>
#include <global.h>
#include <MPU6050.h>
#include <I2Cdev.h>

#if (I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE) && !defined(PARTICLE)
#include "Wire.h"
#endif

class IMU
{
public:
    IMU(){};
    bool begin();
    bool update();
    bool calibrate();

private:
    unsigned long microsPerReading, microsPrevious;
    unsigned long microsNow;
};

#endif /* IMU_H */
