#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>
#include <config.h>
#include <global.h>
#include <data.h>

// https://startingelectronics.org/articles/arduino/measuring-voltage-with-arduino/

// read voltage and update data struct battVoltate variable
// this varies a lot from each voltage divider resistance values and their respective tolerances ( <5%)
// Max Vin: 12v
// R1: 270kOhm
// R2: 100kOhm
// Max Vout: 3.243v when Vin is 12v
// Input impedance: 270kOhm/100kOhm = 2.7kOhm
// Voltage divider factor: 12v/3.243v = 3.7
// 10bit ADC resolution: 1024

class BMS //battery management system
{
public:
    BMS(){};
    bool begin();
    bool readVoltate();

private:
    int count = 0;
    float samples = 0.0f;
    float referenceVoltage = 3.3f;
    float voltageDividerFactor = 3.7f;
    float ADC_resolution = 1024.0f;
};

#endif /* BATTERY_H */
