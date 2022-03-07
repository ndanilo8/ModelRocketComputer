#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>
#include <config.h>
#include <global.h>
#include <data.h>
#include <Chrono.h>


/**
 * @brief BMS - Battery Managment System
 *
 * @note 
 * this varies a lot from each voltage divider resistance values and their respective tolerances ( <5%)
 * Max Vin: 12v
 * R1: 270kOhm
 * R2: 100kOhm
 * Max Vout: 3.243v when Vin is 12v
 * Input impedance: 270kOhm/100kOhm = 2.7kOhm
 * Voltage divider factor: 12 v/3.243 v = 3.7
 * 10bit ADC resolution: 1024
 * 
 * @see [source 1](https://startingelectronics.org/articles/arduino/measuring-voltage-with-arduino/)
 * @see [source 2]https://www.instructables.com/Voltage-Measurement-Using-Arduino/
 */
class EPS //Electric power system
{
public:
    EPS(){};
    bool begin();
    bool readVoltate();

private:
    // int readVoltCounter = 0;
    float samples = 0.0f;
    //float voltageDividerFactor = 3.7f;
    float Vref = 3.3f;
    float ADC_resolution = 1023.0f;
    // voltage divider specific stuff
    int R1 = 270000.0f;
    int R2 = 100000.0f;

    Chrono batteryVoltageTimer;
};

#endif /* BATTERY_H */
