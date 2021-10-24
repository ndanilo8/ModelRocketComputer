#ifndef CONFIG_H
#define CONFIG_H


/*************************************
            USB DEBUGGING (serial)
 *************************************/
#define is_DEBUG true
#define SERIAL_BAUD 115200 // 115200 max?

/*************************************
                RADIO (serial1)
 *************************************/
#define RADIO_BAUD_RATE 115200

/*************************************
                I2C
 *************************************/
#define I2C_BMP_ADDR 0x77 // default
#define I2C_IMU_ADDR 0x68
// IMU AD0 low = 0x68 
// IMU AD0 high = 0x69


/*************************************
              ALTIMETER
 *************************************/
#define FILTER_UPDATE_RATE 250 // Hz 
// TODO change update value ^

/*************************************
              ALTIMETER
 *************************************/
#define LIFTOFF_ALTITUDE_THRESHOLD 2.0f             // m
#define LAUNCH_ACCEL_THRESHOLD 2.0f    // m/s^2
#define LAUNCH_ACCEL_TIME_THRESHOLD 30 // ms
#define ACCEL_UNPOWERED_THRESHOLD 1.5f // m/s^2
#define ACCEL_TIME_SENSITIVITY 2500 // us

/*************************************
              LOGGER
 *************************************/
#define TELEMETRY_SAMPLE_RATE 30      // ms
#define TELEMETRY_SAMPLE_TOTAL_TIME 45 // seconds | time that the logger will work

/*************************************
            PYRO CHANNEL
 *************************************/
#define CONTINUITY_CHECK_TIME_RATE 1000 // ms Time taken with each check
#define FIRE_ON_TIME 250 // ms

/*************************************
            BMS (BATTERY MANAGEMENT SYSTEM)
 *************************************/
#define NUM_BMS_SAMPLES 10 // read voltate samples


#endif /* CONFIG_H */
