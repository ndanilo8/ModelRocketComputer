#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <data.h>
#include <config.h>
#include <global.h>
#include <Chrono.h>
#include <SerialTransfer.h>

// this library has all the code to manage the telemetry logging and radio downlink
class Telemetry
{
public:
    Telemetry(){};

    bool begin();
    bool send2radio();

private:
    SerialTransfer radio;
    int telemetryState = 0; //state machine for the telemetry

    float timeRate = 1000 / TELEMETRY_REFRESH_RATE; // hz to ms
};
#endif /* TELEMETRY_H */
