#include <battery.h>

/**
 * @brief Init EPS
 * 
 * @return true if start up nominal
 */
bool EPS::begin()
{
    pinMode(PIN_BATT_VOLTAGE, INPUT);

#if is_DEBUG
    Serial.print("Reading Battery Voltage");
#endif
    for (int i = 0; i < 10; i++)
    {
        readVoltate();
#if is_DEBUG
        Serial.print(".");
#endif
    }

#if is_DEBUG
    Serial.println();
    Serial.println("BATT VOLTAGE: " + String(data.batteryVoltage) + " BATERY MANAGEMENT --> OK!");
#endif
    return true;
}

/**
 * @brief Read Battery Voltage
 * 
 * @return true if read is successful
 */
bool EPS::readVoltate()
{
    if (batteryVoltageTimer.hasPassed(refreshTime))
    {

        // if (readVoltCounter <= NUM_EPS_SAMPLES)
        // {
        samples = analogRead(PIN_BATT_VOLTAGE);

        //     if (readVoltCounter == NUM_EPS_SAMPLES)
        //     {
        // Determine Battery Voltage
        data.batteryVoltage = ((samples * referenceVoltage) / ADC_resolution) * (R2 / (R1 + R2));
        batteryVoltageTimer.restart();
        return true;

        //             readVoltCounter = 0; // reset counter
        //             samples = 0;
        //             // See if Batery level is low (might add a system level flag)
        //             if (data.batteryVoltage < MIN_BATTERY_VOLTAGE)
        //             {
        // #if is_DEBUG
        //                 // Serial.println("BATTERY LEVEL LOW");
        // #endif
        //             }
        //         }
        //         else
        //         {
        //             readVoltCounter += 1;
        //         }
        // }
        // else
        // {
        //     return false;
        // }
    }
}
