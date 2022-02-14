/*
   Model Rocket Telemetry Logger

   Copyright (C) 2021 Danilo Nascimento
   GNU General Public License v3.0
   Permissions of this strong copyleft license are conditioned on making available complete source code of licensed works and modifications,
   which include larger works using a licensed work, under the same license.
   Copyright and license notices must be preserved. Contributors provide an express grant of patent rights.

   Contact information
   -------------------
   E-mail: ndanilo8@hotmail.com
   Website: daniloonspace.com
*/

#define SOFTWARE_VERSION 1.00

#include <Arduino.h>
#include <Wire.h>

// Config Files
#include <global.h>
#include <config.h>
#include <data.h>

// AHRS 
#include <KalmanFilter/kalman.h>
#include <IMU/imu.h>
#include <Altimeter/altimeter.h>
#include <Altimeter/pyro.h>

// Logging and Downlink
#include <telemetry.h>
#include <sd.h>

// State indicators and battery feedback
#include <battery.h>
#include <led.h>

EKF ekf; 
IMU imu;
Altimeter altimeter;
Pyro pyro;
Telemetry telemetry;
SDLogger sdLoggger;
EPS battery;
LED led;

unsigned long prevLoopTime, currentLoopTime, startTime;

Chrono ekfTimer;
Chrono liftoffTimer; // time taken to detect positive liftoff

void setup()
{
  int delayPeriod = 400; // ms
  // initialize debuging
#if is_DEBUG
  goToState(TEST);
  Serial.begin(SERIAL_BAUD);
  while (!Serial)
  {
    ; // pause till we open our serial monitor
  }
  Serial.println("*-*-*-*-*-*-*-*-*-*-*\nModel Rocket Telemetry Logger\nSoftware Version: " + String(SOFTWARE_VERSION) + "\n*-*-*-*-*-*-*-*-*-*-*");
  delay(delayPeriod);
#endif
  Wire.begin();
  SPI.begin();

  bool startupError = false;
  // delays make it easier to read
  if (!led.begin())
    startupError = true;

#ifdef is_RADIO
  delay(delayPeriod);
  if (!telemetry.begin())
    startupError = true;
#endif

  delay(delayPeriod);
  if (!imu.begin())
    startupError = true;

  delay(delayPeriod);
  if (!pyro.begin())
    startupError = true;

  delay(delayPeriod);
  if (!altimeter.begin())
    startupError = true;

  delay(delayPeriod);
  if (!battery.begin())
    startupError = true;

#ifdef is_LOGGING
  delay(delayPeriod);
  if (!sdLoggger.begin())
    startupError = true;
#endif

  delay(delayPeriod);

  ekf.initkalman();

  while (startupError == true)
  {
    // beep and light red LED
    // led.led_ON();
    ;
  }
}

void loop()
{
  // Timing
  currentLoopTime = micros();
  data.delta_t = float(currentLoopTime - prevLoopTime) / 1000000.0f;
  prevLoopTime = currentLoopTime;

  // this is the flight timestamp
  if (data.state < LIFTOFF)
  {
    data.flightTime = 0.0;
    startTime = millis();
  }
  else if (data.state > LIFTOFF || data.state < LANDED)
  {
    data.flightTime = float((millis() - startTime) / 1000.0f);
  }
  else if (data.state == LANDED)
  {
    data.flightTime = data.flightTime;
  }

  // functions to run every loop
  imu.update();          // updates the IMU when new data is available
  altimeter.update();    // updates the barometric sensor when new data is available
  battery.readVoltate(); // reads the battery voltage

  float accel = (data.imu.accel.x - 9.81); // accel - gravity -> read 0 ms-2 whe at rest
  ekf.predict(accel);                      // predicts system state according to acceleration data (x axis)

  if (ekfTimer.hasPassed(EKF_UPDATE_TIME)) 
  {
    ekf.updateBaro(data.altimeter.altitude); // update ekf with barometric data
    ekfTimer.restart();                      // restart timer for next update
  }

  switch (data.state)
  {
  case IDLE:
    // add a radio receiver function to change state or a physical button when pressed or not pressed it changes to liftoff state (remove before flight kinda thing)
    // right now it will just forward to liftoff detection mode

    // Checks continuity every (check config.h for timer)
    pyro.checkContinuity();
    goToState(LIFTOFF);
    break;

  case LIFTOFF:

    if (data.imu.accel.x >= LAUNCH_ACCEL_THRESHOLD || data.altimeter.altitude >= LIFTOFF_ALTITUDE_THRESHOLD)
    {
      if (liftoffTimer.hasPassed(LAUNCH_ACCEL_TIME_THRESHOLD)) // noise safe timer so liftoff is only detect if a accel threshold is exceded for some particular time
        goToState(POWERED_ASCENT);
    }
    break;

  case POWERED_ASCENT:

    // FIXME
    // calculate total accel of the vehicle = sqrt(x^2 + y^2 + z^2)
    data.imu.accelTotal = sqrt(sq(data.imu.accel.x) + sq(data.imu.accel.y) + sq(data.imu.accel.z));

    if (data.imu.accelTotal < ACCEL_UNPOWERED_THRESHOLD)
      goToState(MECU);

    break;

  case MECU:
    if (altimeter.detectApogge())
      goToState(APOGGE);

    break;

  case APOGGE:
    if (pyro.fire())
      goToState(PARACHUTE_DESCENT);

    break;

  case PARACHUTE_DESCENT:
    pyro.allOff(); // just to make sure the pyro channels go off
    // TODO find a better way
    if (data.altimeter.altitude <= LIFTOFF_ALTITUDE_THRESHOLD) // this might not work if we stay above the pre-set AGL altitute, then the threshold will never be reached and the system will be kept in this state: to mitigate this we could check the rocket attitute to see if its stationary or not
      goToState(LANDED);

    break;

  case LANDED:
    // BEEP and blink LED: make it easier to find!
    led.blinkSlow();
    break;

  case ABORT:
    // not much we could do besides firing the parachute.. and send it back to Parachute descend and then landed

    break;

  case TEST: // debug mode

#if is_DEBUG
    Serial.print("Yaw: ");
    Serial.print(data.imu.eulerAngles.yaw);
    Serial.print(" , Pitch: ");
    Serial.print(data.imu.eulerAngles.pitch);
    Serial.print(" , Roll: ");
    Serial.print(data.imu.eulerAngles.roll);

    Serial.print(" Accel x: ");
    Serial.print(data.imu.accel.x);
    Serial.print(", y: ");
    Serial.print(data.imu.accel.y);
    Serial.print(", z: ");
    Serial.print(data.imu.accel.z);

    Serial.print(" Gyro x: ");
    Serial.print(data.imu.gyro.x);
    Serial.print(", y: ");
    Serial.print(data.imu.gyro.y);
    Serial.print(", z:");
    Serial.print(data.imu.gyro.z);

    Serial.print(" Altitude: ");
    Serial.print(data.kalman.altitude);
    Serial.print(" , Temperature: ");
    Serial.print(data.altimeter.temperature);
    Serial.print(" , verticalSpeed: ");
    Serial.print(data.kalman.verticalVelocity);
    Serial.println();
#endif
    break;
  }

#ifdef is_RADIO
  telemetry.send2radio();
#endif

#ifdef is_LOGGING
  sdLoggger.logData(); // Log Data to SD Card when not on IDLE or LANDED state
#endif
}
