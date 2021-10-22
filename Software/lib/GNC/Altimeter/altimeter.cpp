#include <Altimeter/altimeter.h>
#include <Adafruit_BMP3XX.h>

#include <global.h>
#include <config.h>

Adafruit_BMP3XX bmp;

bool Altimeter::begin()
{



    // Start sensor on I2C
    if (bmp.begin_I2C())
    {
#if is_DEBUG
        Serial.println("bmp --> OK");
#endif
        // Set up oversampling and filter initialization
        bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
        bmp.setPressureOversampling(BMP3_OVERSAMPLING_8X);
        bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
        bmp.setOutputDataRate(BMP3_ODR_200_HZ);

        //TODO rewrite kalman filter
        // KalmanInit(); // start kalman filter for the Altitude readings

#if is_DEBUG
        Serial.print("Calculating Relative Altitude");
#endif

        for (int i = 0; i < 100; i++) // let the sensor make a few readings to get it started
        {
            bmp.readPressure();
            bmp.readTemperature();
        }

        // //Take readings and average them for (Starting Altitude determination)
        for (int i = 0; i < startingAltitudeReadings; i++)
        {
            ground_pressure += bmp.readPressure();

#if is_DEBUG
            Serial.print(".");
#endif
        }
        ground_pressure /= 100;                      // Pascal to hPa
        ground_pressure /= startingAltitudeReadings; // Avg to get relative ground pressure (set my AGL to 0m)

        // TODO design the kalman filter
        for (int i = 0; i < 100; i++) // let the sensor make a few readings to get the filter started
        {
            // data.altitude = KalmanCalc(bmp.readAltitude(ground_pressure));
        }
        Serial.println();
        previous_altitude = 0; //make sure this is 0

#if is_DEBUG
    Serial.println("LAUNCH ALTITUDE: " + String(data.altitude) + " TEMP: " + String(data.temperature);
#endif

#if is_DEBUG
    Serial.println("PYRO CHANNELS --> OK");
#endif
    return true;
    }
    else
    {
#if is_DEBUG
            Serial.println("Starting bmp --> FAILED!");
#endif
        return false;
    }
}

float Altimeter::updateAltimeter()
{    
  if (bmp.performReading())
  {
    data.altimeter.altitude = 44330.0 * (1.0 - pow((bmp.pressure / 100.0) / ground_pressure, 0.1903)); // /100 Pascal to hPa
    // current_altitude = KalmanCalc(current_altitude);
    // current_pressure = bmp.pressure;
    data.altimeter.temperature = bmp.temperature;
  }

  // Ignore negative altitude
  // if (current_altitude < 0)
  // {
  //   current_altitude = 0;
  // }

  // Record the max altitude (overide)
  if (current_altitude > max_altitude)
  {
    max_altitude = data.altimeter.altitude;
  }

  // Replace the previous_altitude with the current altitude for next pass.
  previous_altitude = data.altimeter.altitude;
  return data.altimeter.altitude;
}

bool Altimeter::detectApogge()
{
  if ((current_altitude < max_altitude)) // TODO maybe add a 10 measures safe mode? (if curr_altitude<max_altidue for 10 loops we got to apogge)
  {
#if is_DEBUG
    Serial.println("---- APOGGE! ---- Max altitude: ");
    Serial.print(max_altitude);
#endif
    return true;
  }
  else
  {
    return false;
  }
}