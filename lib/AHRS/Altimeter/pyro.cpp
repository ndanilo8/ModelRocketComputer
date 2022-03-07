#include <Altimeter/pyro.h>
#include <data.h>
#include <Chrono.h>
/*
A special thank you to Adam Marciniak!
part of this Pyro timers usage are inspired on his work found at https://github.com/AdamMarciniak
Go show him some appreciation!
*/

Chrono continuityTimer;

bool Pyro::begin()
{
    pinMode(PIN_PYRO_1_EN, OUTPUT);
    pinMode(PIN_PYRO_1_CONT, INPUT);
#if is_DEBUG
    Serial.println("Starting Pyro Channel 1 --> OK!");
#endif
    return true;
}

//TODO just use chrono instead lol
bool Pyro::fire()
{
    //after firing this will never fire again.. since the fireTime Variable will be != 0
    if (hasFired == false && fireTime == 0)
    {
        //Fire pyro charge
        fireTime = millis();
        hasFired = true;
        analogWrite(PIN_PYRO_1_EN, 255);
        return false;
    }

    if (hasFired == true && millis() - fireTime >= FIRE_ON_TIME)
    {
        //Stop pyro charge
        analogWrite(PIN_PYRO_1_EN, 0);
        hasFired = false;
        return true;
    }
}

// TODO test needed
bool Pyro::checkContinuity()
{
    if (continuityTimer.hasPassed(CONTINUITY_CHECK_TIME_RATE)) // Check continuity time
    {
        if (analogRead(PIN_PYRO_1_CONT) < 200)
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
        return true;
    }
}

bool Pyro::allOff()
{

    analogWrite(PIN_PYRO_1_EN, 0);
    return true;
}