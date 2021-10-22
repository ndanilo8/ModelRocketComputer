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

void setup()
{
  // put your setup code here, to run once:

  // initialize debuging
#if is_DEBUG
  Serial.begin(SERIAL_BAUD);
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
  switch (data.state)
  {
  case IDLE:
    goToState(LIFTOFF);

    break;

  case LIFTOFF:
    goToState(POWERED_ASCENT);

    break;

  case POWERED_ASCENT:
    goToState(MECU);
    
    break;

  case MECU:
    goToState(APOGGE);

    break;

  case APOGGE:
    goToState(PARACHUTE_DESCENT);

    break;

  case PARACHUTE_DESCENT:
    goToState(LANDED);

    break;

  case LANDED:

    break;

  case ABORT:

    break;

  case CALIBRATION:

    break;

  case TEST:

    break;
  }
}