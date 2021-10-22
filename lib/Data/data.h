#ifndef DATA_H
#define DATA_H

#include <Arduino.h>
// #include <vector.h>
#include <config.h>
#include <global.h>

extern uint16_t rateHz;
extern uint16_t numSeconds;
extern uint16_t totalSamples;
extern uint16_t millisPerSample;

enum State_t
{
    IDLE = 1,    // LAUNCHPAD
    LIFTOFF = 2, // ARMED waiting liftoff
    POWERED_ASCENT = 3,
    MECU = 4, // MECU
    APOGGE = 5,
    PARACHUTE_DESCENT = 6,
    LANDED = 7,
    ABORT = 8,
    CALIBRATION = 9, // Bias gathering
    TEST = 10,       // run specific code (debug mode rigth?)
};

struct axis_t
{
    int16_t x;
    int16_t y;
    int16_t z;
};

struct euler_t
{
    float yaw;
    float pitch;
    float roll;
};

struct imu_t
{
    axis_t accel;
    axis_t gyro;
    euler_t eulerAngles;
};

struct altimeter_t
{
    float altitude;
    float temperature;
    float verticalVelocity;
};

void goToState(State_t state);

struct Telemetry_t
{
    State_t state;
    float loopTime;
    float sampleRate; //samples per second
    // MARG
    imu_t imu;
    // Altimeter
    altimeter_t altimeter;
    // System: Battery, pyro continuity and State and loop time
    float batteryVoltage;
    float pyro1Continuity;
};

extern Telemetry_t data;

#endif /* DATA_H */
