#ifndef NAV_H
#define NAV_H

#include <Arduino.h>
#include <config.h>
#include <global.h>
#include <MPU6050.h>
#include <I2Cdev.h>

#if (I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE) && !defined(PARTICLE)
#include "Wire.h"
#endif

class Nav
{
public:
    Nav(){};
    bool begin();
    bool update(); // TODO write dead reckoning 
    bool calibrate(); // TODO write 

private:
    unsigned long microsPerReading, microsPrevious;
    unsigned long microsNow;
};

#endif /* NAV_H */
