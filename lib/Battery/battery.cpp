#include <battery.h>

bool BMS::begin()
{
    pinMode(PIN_BATT_VOLTAGE, INPUT);

#if is_DEBUG
    Serial.print("Reading Battery Voltage");
#endif
    while (!readVoltate()) // wait to get a voltage reading
    {
        readVoltate();
#if is_DEBUG
        Serial.print(".");
#endif
    }
#if is_DEBUG
    Serial.println("BATT VOLTAGE: " + String(data.batteryVoltage) + " BATERY MANAGEMENT --> OK");
#endif
    return true;
}

// non interrupting readVoltage function
bool BMS::readVoltate()
{
    if (count <= NUM_BMS_SAMPLES)
    {
        samples = analogRead(PIN_BATT_VOLTAGE);
        count += 1;
        if (count = NUM_BMS_SAMPLES)
        {
            data.batteryVoltage = (samples / NUM_BMS_SAMPLES * referenceVoltage) / ADC_resolution;
            data.batteryVoltage *= voltageDividerFactor; // update voltage reading
            count = 0;                                   // reset counter
            return true;
        }
    }
    else
    {
        return false;
    }
}