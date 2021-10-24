void continuity_check()
{
  // takeOff Check GO / NO - GO
  if (err == false && liftoff == false && apogeeHasFired == false) // If error check is nominal -> Start continuity check
  {
    continuityCheck(pinApogeeContinuity);
  }
}

void liftoffDetection(float threshold) //accelTotal, threshold
{
  //Accel calcs
  // mpu.getAcceleration(&ax, &ay, &az);
  // float aX = ax / 2048;
  // Serial.println(aX);
  if ((currAltitude > liftoffAltitude) && liftoff == false) //look for liftoff
  {
    liftoff = true; //Liftoff is true, carry on to flight stuff
    //  Serial.println("LIFTOFF");
   // beginBeepSeq();
  }
}

void apogeeDetection()
{
  //detect apogee
  if (currAltitude > liftoffAltitude)
  {
    if (currAltitude < lastAltitude )
    {
      measures -= 1;
      if (measures == 0) // Apogee with Barometer readings
      {
        //fire drogue/ chute
        apogeeAltitude = currAltitude;
        //  Serial.println("APOGEE HAS FIRED!"); //DEBUG

        digitalWrite(pinApogee, HIGH);
        delay (2000);
        apogeeHasFired = true;
        digitalWrite(pinApogee, LOW);
      }
      else
      {
        lastAltitude = currAltitude;
        measures = 10;
      }
    }
  }
}

void landed() //check if Rocket landed
{
  if (apogeeHasFired == true && liftoff == true && currAltitude <= liftoffAltitude)
  {
    liftoff = false;
    // Serial.println("LANDED!");
  }
}

void recoveryProtocol()
{
  //Recovery Sequence
  if (apogeeHasFired == true)
  {
    RecoveryBeep();
    beginBeepSeq();
    beginBeepSeq();
  }
}

void dataLogger()
{
  if (liftoff == true)
  {
    //Accel calcs
    mpu.getAcceleration(&ax, &ay, &az);
    float aX = ax / 2048; float aY = ay / 2048; float aZ = az / 2048;
    float at = sqrt(aX * aX + aY * aY + aZ * aZ); // RAW total accel of axis
    /* OTHER DATA
      // String pressure = String (bmp.readPressure()); // pressure in pascals
      // String temp = String (bmp.readTemperature()); //temperature in degrees
    */
    String aT = String (at);
    String comma = String ("  ,  ");
    String alt = String (currAltitude);
    //Time Stamp String
    String timer = String (millis());

    //make a big string containing above strings
    String Baro_data = String (("Altitude= " + alt) + comma + ("accel= " + aT) + comma + timer);

    //String flight_Data = String (alt + comma + at + comma + timer);
    //String all_data = String (("Altitude= " + alt + " m") + comma + ("Pressure= " + pressure + " Pa") + comma + ("Temp = " + temp + " °C") + comma + ("aX= " + accelX + " g") + comma + timer) ;

    //  Serial.println (Baro_data); // DEGUB

    File dataFile = SD.open("dataLog.txt", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(Baro_data); //put Baro_data on the SD card & print apogee altitude to SD Card
      dataFile.close();
    }
  }
}
