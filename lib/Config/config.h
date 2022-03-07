#ifndef CONFIG_H
#define CONFIG_H

// Here you main find all settings that are possible to change to suit your needs!
// while some are better not changing ;)

#define ENABLED 1
#define DISABLED 0
// Possible changes ^


#define EARTH_ACCEL 9.8067f

/*************************************
            USB DEBUGGING (serial)
 *************************************/
#define is_DEBUG ENABLED   // change to disabled to stop debug mode
#define SERIAL_BAUD 115200 // 115200 max with seeeduino xiao?

/*************************************
                RADIO (serial1)
 *************************************/
#define is_RADIO DISABLED 
#define RADIO_BAUD_RATE 115200

/*************************************
                I2C
 *************************************/
#define I2C_BMP_ADDR 0x77 // default
#define I2C_IMU_ADDR 0x68 // default
// IMU AD0 low = 0x68
// IMU AD0 high = 0x69

/*************************************
              EKF (not that useful at this stage)
 *************************************/
/*Current States:
Altitude
Vertical Velocity
*/
//#define EKF_UPDATE_TIME 5 // in ms // 200 Hz update rate

/*************************************
              IMU
 *************************************/
#define FILTER_UPDATE_RATE 25 // Hz // gradient-descent algorithm (madgwick algorithm)

/*************************************
              ALTIMETER
 *************************************/
#define LIFTOFF_ALTITUDE_THRESHOLD 2.0f // m
#define LAUNCH_ACCEL_THRESHOLD 3.0f     // gs
#define LAUNCH_ACCEL_TIME_THRESHOLD 30  // ms
#define ACCEL_UNPOWERED_THRESHOLD 1.5f  // m/s^2

/*************************************
              LOGGER & RADIO
 *************************************/
#define is_LOGGING DISABLED
#define FILE_BASE_NAME "log"      // Base name must be 6 or less chars
#define TELEMETRY_REFRESH_RATE 50 // hz

/*************************************
            PYRO CHANNEL
 *************************************/
#define CONTINUITY_CHECK_TIME_RATE 100 // ms Time taken with each check
#define FIRE_ON_TIME 250               // ms

/*************************************
            BMS (BATTERY MANAGEMENT SYSTEM)
 *************************************/
#define MIN_BATTERY_VOLTAGE 7.65 // minimum battery voltage in Volts
#define BATTERY_CHECK_UPDATE_TIME 500// ms
/*************************************
                LED
 *************************************/
#define LED_FAST_BLINK_DURATION 500  // ms
#define LED_SLOW_BLINK_DURATION 1000 // ms

#endif /* CONFIG_H */
