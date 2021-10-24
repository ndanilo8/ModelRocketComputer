void shortBeep()
{
  const int shortBeep_timing = 300;
  if(milis() -startTime >= shortBeep_timing){
  tone(buzzer, 600, 25);
  startTime =milis();  
  }
else{
  noTone(buzzer);
}
}

void longBeep()
{
  const int periodDuration = 1000;
  if (millis() - startTime >= periodDuration) {
    tone(buzzer, 600);
    startTime = millis();
  }
  else {
    noTone(buzzer);
  }
}

void RecoveryBeep()
{
  tone(buzzer, 2000);
  delay(1000);
  noTone(buzzer);
}

void beginBeepSeq()
{
  int i = 0;
  for (i = 0; i < 10; i++)
  {
    tone(buzzer, 1600, 1000);
    delay(50);
    noTone(buzzer);
  }
  delay(800);
}

void activity_LED() {
  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(LED_BUILTIN, blinkState);
}
void err_Led() {
  digitalWrite(redLed, HIGH); //RED LED
}

void init_Led() {
  for (int j = 0; j < 3; j++) {
    digitalWrite(redLed, HIGH);
    delay(100);
    digitalWrite(redLed, LOW);
    delay(100);
    digitalWrite(greenLed, HIGH);
    delay(100);
    digitalWrite(greenLed, LOW);
    delay(100);
    digitalWrite(blueLed, HIGH);
    delay(100);
    digitalWrite(blueLed, LOW);
    delay(500);
  }
}
