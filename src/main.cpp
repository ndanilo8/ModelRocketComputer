/*
   Model Rocket Telemetry Logger Version 3

   Copyright (C) 2021 Danilo Nascimento. All rights reserved
   GNU General Public License v3.0
   Permissions of this strong copyleft license are conditioned on making available complete source code of licensed works and modifications,
   which include larger works using a licensed work, under the same license.
   Copyright and license notices must be preserved. Contributors provide an express grant of patent rights.

   Contact information
   -------------------
   E-mail: ndanilo8@hotmail.com
*/

#include <header.h>

unsigned long prevLoopTime;
unsigned long currentLoopTime;

void setup()
{
  // put your setup code here, to run once:

  // initialize debuging
#if is_DEBUG
  Serial.begin(SERIAL_BAUD);
  goToState(TEST);
#endif
  Wire.begin();

  bool error = false;

  if (!nav.begin())
    error = true;

  if (!pyro.begin())
    error = true;

  if (!altimeter.begin())
    error = true;

  while (error == true)
  {
    // beep and light red LED
    ;
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  currentLoopTime = micros();
  data.loopTime = float(currentLoopTime - prevLoopTime) / 1000000.0f;
  prevLoopTime = currentLoopTime;

  nav.update();
  altimeter.update();
 

  switch (data.state)
  {
  case IDLE:
    goToState(LIFTOFF);

    break;

  case LIFTOFF:

    // TODO add a timer so it looks for a constant accel over some short time (prevent false starts due to spikes induced by moving the rocket)
    if (data.imu.accel.x >= LAUNCH_ACCEL_THRESHOLD || data.altimeter.altitude >= LIFTOFF_ALTITUDE)
    {
      // if (micros() - currentLoopTime >=  )
      goToState(POWERED_ASCENT);
    }

    break;

  case POWERED_ASCENT:
    // calculate total accel of the vehicle atotal = sqrt(x^2 + y^2 + z^2)
    data.imu.accelTotal = sqrt((data.imu.accel.x * data.imu.accel.x) + data.imu.accel.y * data.imu.accel.y + (data.imu.accel.z * data.imu.accel.z));

    //TODO add log function here
    if (data.imu.accelTotal < ACCEL_UNPOWERED_THRESHOLD)
      goToState(MECU);

    break;

  case MECU:
    if (altimeter.detectApogge())
      goToState(APOGGE);

    break;

  case APOGGE:
    if (pyro.fire(PIN_PYRO_1_EN))
      goToState(PARACHUTE_DESCENT);

    break;

  case PARACHUTE_DESCENT:
    if (data.altimeter.altitude <= LIFTOFF_ALTITUDE)
      goToState(LANDED);

    break;

  case LANDED:
// BEEP and blink LED

    break;

  case ABORT:
//Fire
    break;

  case TEST:
#if is_DEBUG
    // Serial.println("Yaw: ");
    // Serial.print(data.imu.eulerAngles.yaw);
    // Serial.print(", Pitch: ");
    // Serial.print(data.imu.eulerAngles.pitch);
    // Serial.print(", Roll: ");
    // Serial.print(data.imu.eulerAngles.roll);

    // Serial.println("Accel x: ");
    // Serial.print(data.imu.accel.x);
    // Serial.print(", y: ");
    // Serial.print(data.imu.accel.y);
    // Serial.print(", z: ");
    // Serial.print(data.imu.accel.z);

    // Serial.println("Gyro x: ");
    // Serial.print(data.imu.gyro.x);
    // Serial.print(", y: ");
    // Serial.print(data.imu.gyro.y);
    // Serial.print(", z:");
    // Serial.print(data.imu.gyro.z);

    // Serial.println("Altitude: ");
    // Serial.print(data.altimeter.altitude);
    // Serial.print(" , Temperature: ");
    // Serial.print(data.altimeter.temperature);
    // Serial.print(" , verticalSpeed: ");
    // Serial.print(data.altimeter.verticalVelocity);
#endif
    break;
  }

   telemetry.send2uart();
}