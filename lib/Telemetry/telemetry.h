#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <data.h>
#include <config.h>
#include <global.h>
#include <Chrono.h>

// this library has all the code to manage the telemetry logging and radio downlink
class Telemetry
{
public:
    Telemetry(){};

    bool begin();
    bool send2uart();
    // bool sendData2Radio();
    // bool logData2SsdCard();

    private:
    int telemetryState = 0; //state machine for the telemetry
};
#endif /* TELEMETRY_H */
