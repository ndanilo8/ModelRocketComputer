#ifndef CONFIG_H
#define CONFIG_H


/*************************************
            USB DEBUGGING
 *************************************/
#define is_DEBUG false
#define SERIAL_BAUD 115200 // 115200 max?

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
// TODO EXAMPLE VALUE

/*************************************
              ALTIMETER
 *************************************/
#define LIFTOFF_ALTITUDE 2.0f             // m
#define LAUNCH_ACCEL_THRESHOLD 2.0f    // m/s^2
#define ACCEL_UNPOWERED_THRESHOLD 1.5f // m/s^2

/*************************************
              LOGGER
 *************************************/
#define TELEMETRY_REFRESH_RATE 20 // Hz

/*************************************
            PYRO CHANNEL
 *************************************/
#define CONTINUITY_CHECK_TIME 1000 // ms Time taken with each check
#define FIRE_ON_TIME 1000 // ms


#endif /* CONFIG_H */
