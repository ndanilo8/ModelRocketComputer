#ifndef HEADER_H
#define HEADER_H


// header file for main.cpp

#include <Arduino.h>
#include <Wire.h>
// #include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <MPU6050.h>

#include <global.h>
#include <config.h>
#include <data.h>

#include <Nav/nav.h>
#include <Altimeter/altimeter.h>
#include <Altimeter/pyro.h>

#include <telemetry.h>
#include <battery.h>


Nav nav;
Altimeter altimeter;
Pyro pyro;
Telemetry telemetry;

BMS battery;

#endif /* HEADER_H */
