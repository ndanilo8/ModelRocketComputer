#include <Altimeter/altimeter.h>
#include <Adafruit_BMP3XX.h>
#include "MovingAverageFilter.h"
#include <Chrono.h>

#include <global.h>
#include <config.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;
MovingAverageFilter MAF_altitude(10);
MovingAverageFilter MAF_velocity(20);

Chrono apogeeTimer;

bool Altimeter::begin()
{
  // Start sensor on I2C
  if (bmp.begin_I2C())
  {
#if is_DEBUG
    Serial.println("Starting BMP --> OK!");
#endif
    // Set up oversampling and filter initialization
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    bmp.setOutputDataRate(BMP3_ODR_200_HZ);

    // KalmanInit(); // start kalman filter for the Altitude readings // KALMAN FILTER ON main.cpp

#if is_DEBUG
    Serial.print("Calculating Relative Altitude");
#endif

    for (int i = 0; i < 50; i++) // let the sensor make a few readings to get it started
    {
      bmp.readPressure();
      bmp.readTemperature();
    }

    // Just take the average for (relative Altitude determination)
    for (int i = 0; i < startingAltitudeReadings; i++)
    {
      sealevel_offset += bmp.readAltitude(SEALEVELPRESSURE_HPA);

      // ground_pressure += bmp.readPressure();

#if is_DEBUG
      Serial.print(".");
#endif
    }
    sealevel_offset /= startingAltitudeReadings;

    // ground_pressure /= 100;                      // Pascal to hPa
    // ground_pressure /= startingAltitudeReadings; // Avg to get relative ground pressure (set my AGL to 0m)

    // Equation taken from BMP180 datasheet (page 16):
    //  http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf
    // current_altitude = 44330.0 * (1.0 - pow((bmp.pressure / 100.0) / ground_pressure, 0.1903)); // /100 Pascal to hPa

    current_altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA) - sealevel_offset;
    data.altimeter.altitude = current_altitude;
    data.altimeter.temperature = bmp.readTemperature();
    previous_altitude = 0; // make sure this is 0

#if is_DEBUG
    Serial.printf("\nLAUNCH ALTITUDE: %f AMBIENT TEMP: %f ", data.altimeter.altitude, data.altimeter.temperature);
#endif

#if is_DEBUG
    Serial.printf("\nPYRO CHANNELS --> OK!");
#endif
    return true;
  }
  else
  {
#if is_DEBUG
    Serial.print("\nStarting BMP --> FAILED!");
#endif
    return false;
  }
}

bool Altimeter::update()
{
  if (bmp.performReading())
  {
    current_altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA) - sealevel_offset;
    data.altimeter.temperature = bmp.temperature;
  }

  current_altitude = MAF_altitude.process(current_altitude);
  data.altimeter.verticalVelocity = MAF_velocity.process(Altimeter::getVerticalVelocity());

  data.altimeter.altitude = current_altitude;

  // Record the max altitude
  if (current_altitude > max_altitude)
  {
    max_altitude = data.altimeter.altitude;
  }

  

  // Replace the previous_altitude with the current altitude for next loop
  previous_altitude = current_altitude;
  return true;
}

bool Altimeter::detectApogge()
{
  if ((current_altitude < max_altitude))
  {
    if (apogeeTimer.hasPassed(100))
    {
#if is_DEBUG
      Serial.printf("\n---- APOGGE! ---- Max altitude: %f", max_altitude);
#endif
      return true;
    }
  }
  else
  {
    return false;
  }
}

float Altimeter::getVerticalVelocity()
{
  return (current_altitude - previous_altitude) / data.delta_t;
}
