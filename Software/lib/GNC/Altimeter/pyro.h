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
    bool fire(int channel);
    bool checkContinuity(int channel); // select the channel

private:
    unsigned long fireTime = 0; // millis
    bool firingStatus = false;
    float analogVal;
};

#endif /* PYRO_H */
