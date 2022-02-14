#ifndef ALTIMETER_H
#define ALTIMETER_H

#include <Arduino.h>
#include "data.h"

/**
 * @brief Altimeter
 * 
 */
class Altimeter
{
public:
    Altimeter(){};

    bool begin();        // return true if OK
    bool update();       // update Altimeter Data and it goes on top of the loop()
    bool detectApogge(); // detect apogge

private:
    float getVerticalVelocity();
    float getAltitude_Temperature();

    float current_pressure, ground_pressure;                     // Pressure
    float previous_altitude, current_altitude, max_altitude = 0; // Altitude
    float startingAltitudeReadings = 16.0f;                      // amount of readings

    float sealevel_offset = 0;

};

#endif /* ALTIMETER_H */
