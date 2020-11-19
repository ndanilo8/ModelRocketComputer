/*

   Flight Computer Version 2.01 SMART APOGEE (ONE STAGE ROCKET)

   Contact information
   -------------------
   author: Danilo Nascimento
   e-mail: ndanilo8@hotmail.com
*/

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_BMP085.h>
#include <MPU6050.h>

//DEBUGGING->>>VERBOSE MODE
//#define DEBUG //uncomment to start debug mode



// ***********************************SENSORS************************************

//********IMU********
MPU6050 mpu;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high
int16_t ax, ay, az;

//********BMP********
Adafruit_BMP085 bmp;

//********SD CARD********
unsigned long time;
const int chipSelect = 10;
//SD card variables
File dataFile;

//Buzzer
unsigned long startTime = millis();
unsigned long interval = 10000; // timer for buzzer

//********************************Global variables**********************
//ground level altitude
long initialAltitude;

//current altitude
long currAltitude;

//Apogee altitude
long apogeeAltitude;
long liftoffAltitude;
long lastAltitude;

//boolean variables
boolean apogeeHasFired = false; //Our drogue/chute has been ejected i.e: apogee has been detected
boolean apogeeContinuity = true;

boolean err = false; //Error check
boolean liftoff = false;
boolean blinkState = false;

unsigned long measures; //number of measures to do so that we are sure that apogee has been reached


//********************************LED, Buzzer, all Pins**********************
const int redLed = 9;
const int greenLed = 8;
const int blueLed = 7;
const int pinApogee = 5;
const int pinApogeeContinuity = 3;
const int buzzer = 6; // pin 6


//*********Kalman filter Variables*****************
float f_1 = 1.00000; //cast as float
float kalman_x;
float kalman_x_last;
float kalman_p;
float kalman_p_last;
float kalman_k;
float kalman_q;
float kalman_r;
float kalman_x_temp;
float kalman_p_temp;
//********end of Kalman Variables*****************

void setup()
{
  int val = 0;     // variable to store the read value

  //init Kalman filter
  KalmanInit();

  // initialize debuging
#ifdef DEBUG
  Serial.begin(115200);
#endif
  Wire.begin();

  //************INIT*************
  SDcardInit(); //SD card: see if the card is present and can be initialized:
  bmpInit(); //Pressure Sensor Init & Check
  mpuInit(); //Accel Sensor Init & Check

  //our drogue/chute has not been fired
  apogeeHasFired = false;
  //Initialise the Pyro & buzzer pins)
  pinMode(pinApogee, OUTPUT);
  pinMode(buzzer, OUTPUT);
  //Initialise continuity Pyro check pins)
  pinMode(pinApogeeContinuity, INPUT);
  //Make sure that the output are turned off
  digitalWrite(pinApogee, LOW);
  digitalWrite(buzzer, LOW);
  //LED pins
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);


  //************************ FLIGHT SETUP************************

  //number of measures to do to detect Liftoff & Apogee
  measures = 10;

  // Do some dummy altitude reading
  // to initialise the Kalman filter
  for (int i = 0; i < 50; i++) {
    KalmanCalc(bmp.readAltitude());
  }
  //Read the lauch site altitude
  long sum = 0;
  long curr = 0;
  for (int i = 0; i < 10; i++) {
    curr = KalmanCalc(bmp.readAltitude());
    sum += curr;
    delay(50);
  }

  initialAltitude = (sum / 10.0);

  lastAltitude = 0;

  liftoffAltitude = 2; //change to 5 m on real flight

  //************ ERROR CHECK*************
  Err(); //returns 3 beeps if setup was successful or 1 long beep if there was an error on setup
}

void loop()
{
  //read current altitude
  currAltitude = (KalmanCalc(bmp.readAltitude()) - initialAltitude);

  //*_*_*_*_*FLIGHT PROTOCOL*_*_*_*_*

  continuity_check(); //pyro channel continuity check
  liftoffDetection(4); //accel threshold for liftoff in Gs
  apogeeDetection(); //apogee detection
  recoveryProtocol();
  landed(); // check if landed & turn datalogging off
  dataLogger(); //accel
  activity_LED(); //blink onboard LED when there's any activity

}
