#ifndef PYRO_H
#define PYRO_H

#include <Arduino.h>
#include <global.h>
#include <config.h>

class Pyro
{
public:
    Pyro(){};
    bool begin();
    bool fire();
    bool checkContinuity(); // select the channel
    bool allOff();

private:
    unsigned long fireTime = 0; // millis
    bool hasFired = false;
    float analogVal;
};

#endif /* PYRO_H */
