#ifndef ALTIMETER_H
#define ALTIMETER_H

#include <Arduino.h>
#include "data.h"

class Altimeter
{
public:
    Altimeter(){};

    bool begin();         // return true if OK
    float updateAltimeter(); // update Altimeter Data and it goes on top of the loop()
    bool detectApogge();     // detect apogge
    
private:
    float current_pressure, ground_pressure; // Pressure
    float previous_altitude, current_altitude, max_altitude; // Altitude
    float startingAltitudeReadings = 16.0f; // amount of readings
};

#endif /* ALTIMETER_H */
