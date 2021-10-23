#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <data.h>
#include <config.h>
#include <global.h>

// this library has all the code to manage the telemetry logging and radio downlink
class Telemetry
{
    
    Telemetry(){};

    bool begin();
    bool formatData();
    bool sendData2Radio();
    bool logData2SsdCard();
};
#endif /* TELEMETRY_H */