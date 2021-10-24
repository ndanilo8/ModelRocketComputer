void Err()
{
  //Give 3 beeps if setup was successful or 1 long beep if there was an error
  if (err == true) {

    err_Led(); //error lED
    //longBeep();
    Serial.println("Error!"); //DEBUG
    return 0; // don't do anything more:
  }
  else
  {
    init_Led();
  }
}


void continuityCheck(int pin)
{
  int val = 0;     // variable to store the read value
  // read the input pin to check the continuity if apogee has not fired
    val = digitalRead(pin);
    if (val == 0)
    {
      //no continuity long beep
      longBeep();
    }
    else
    {
      // continuity short beep
      //shortBeep();

    }
}
