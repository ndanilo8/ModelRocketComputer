#include <Altimeter/pyro.h>
#include <data.h>
#include <Chrono.h>
/*
A special thank you to Adam Marciniak!
part of this library is inspired on his work found at 
https://github.com/AdamMarciniak/Cygnus-X1-Software/blob/master/src/Pyro.cpp
Go show him some appreciation!
*/

Chrono continuityTimer;

bool Pyro::begin()
{
    pinMode(PIN_PYRO_1_CONT, INPUT);
    pinMode(PIN_PYRO_1_EN, OUTPUT);
    return true;
}

bool Pyro::fire(int channel)
{
    if (hasFired == false && fireTime == 0)
    {
        //Fire pyro charge
        fireTime = millis();
        hasFired = true;
        analogWrite(PIN_PYRO_1_EN, 255);
    }

    if (hasFired == true && millis() - fireTime >= FIRE_ON_TIME)
    {
        //Stop pyro charge
        analogWrite(PIN_PYRO_1_EN, 0);
        hasFired = false;
    }
}

// TODO test needed to see if digitalRead works instead of using the analogRead
bool Pyro::checkContinuity(int channel)
{

    if (continuityTimer.hasPassed(CONTINUITY_CHECK_TIME)) // Check continuity time
    {
        if (digitalRead(channel))
        {
            data.pyro1Continuity = 1; // continuity
#if is_DEBUG
            Serial.println("Continuity on Pyro CH!");
#endif
            return true;
        }
        else
        {
            data.pyro1Continuity = 0; //no continuity
            return false;
#if is_DEBUG
            Serial.println("NO Continuity on Pyro CH!");
#endif
        }
        continuityTimer.restart();
    }
}