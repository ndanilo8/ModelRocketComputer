#ifndef DATA_H
#define DATA_H

#include <Arduino.h>
// #include <vector.h>
#include <config.h>
#include <global.h>

/*
This data library is inspired on Adam Marciniak! (his way of organizing everything in one struct and use it throught the software)
Go show him some love https://github.com/AdamMarciniak
*/

// extern uint16_t rateHz;
// extern uint16_t numSeconds;
// extern uint16_t totalSamples;
// extern uint16_t millisPerSample;

enum State_t
{
    IDLE = 0,    // LAUNCHPAD
    LIFTOFF = 1, // ARMED waiting liftoff
    POWERED_ASCENT = 2,
    MECU = 3, // MECU
    APOGGE = 4,
    PARACHUTE_DESCENT = 5,
    LANDED = 6,
    ABORT = 7,
    TEST = 8, // run specific code (debug mode rigth?)
};

struct axis_t
{
    float x;
    float y;
    float z;
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
    float accelTotal;
    axis_t gyro;
    euler_t eulerAngles;
};

struct altimeter_t
{
    float altitude;
    float temperature;
    float verticalVelocity;
};

struct ekf_t
{
    float gain;
    float altitude;
    float verticalVelocity;
};

void goToState(State_t state);

struct Telemetry_t
{
    State_t state;    // State of the system
    float delta_t;    // Discrete Time
    float flightTime; // Keeps track of the time after liftoff
    float sampleRate; // samples per second

    // MARG and EKF
    imu_t imu;             // IMU reports
    altimeter_t altimeter; // Altimeter Reports
    ekf_t kalman;          // ekf Filter

    // System: Battery, pyro continuity and State and loop time
    float batteryVoltage; // Battery Voltage
    int pyro1Continuity;  // Continuity of the pyro channel
};

extern Telemetry_t data;

#endif /* DATA_H */
