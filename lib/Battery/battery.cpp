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
    Serial.printf("\nReading Battery Voltage");
#endif
    for (int i = 0; i < 10; i++)
    {
        readVoltate();
#if is_DEBUG
        Serial.printf(".");
#endif
    }

#if is_DEBUG
    Serial.printf("\nBATT VOLTAGE: %f BATERY MANAGEMENT --> OK!", data.batteryVoltage);
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
    if (batteryVoltageTimer.hasPassed(BATTERY_CHECK_UPDATE_TIME))
    {

        samples = analogRead(PIN_BATT_VOLTAGE);
        // Determine Battery Voltage
        data.batteryVoltage = ((samples * Vref) / ADC_resolution) * ((R1 + R2) /R2 );
        batteryVoltageTimer.restart();

        if (data.batteryVoltage < MIN_BATTERY_VOLTAGE)
        {
#if is_DEBUG
            // Serial.printf("\nBATTERY LEVEL LOW");
#endif
        }
        return true;
    }
}
