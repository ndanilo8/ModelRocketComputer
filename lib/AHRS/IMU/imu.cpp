#include <IMU/imu.h>
#include <data.h>
#include <MadgwickAHRS.h>

// TODO add dead reakoning for in-flight stabilization
// TODO write calibration code

MPU6050 mpu(I2C_IMU_ADDR);
Madgwick filter;

bool IMU::begin()
{
        mpu.initialize();
        // ^^
        // gyro on 250
        // accel on 4g
        if (mpu.testConnection())
        {
                mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_4);
                filter.begin(FILTER_UPDATE_RATE);
                microsPerReading = 1000000 / FILTER_UPDATE_RATE;
                microsPrevious = micros();
#if is_DEBUG
                Serial.println("Starting MPU6050 --> OK!");
#endif
                return true;
        }
        else
        {
#if is_DEBUG
                Serial.println("Starting MPU6050 --> FAILED!");
#endif
                return false;
        }
}

/*
to maintain attitute looging use other method.. kalman filter or dead reckoning: gyro integration for position and change local to global frame of reference
*/
bool IMU::update()
{
        // check if it's time to read data and update the filter
        microsNow = micros();
        if (microsNow - microsPrevious >= microsPerReading)
        {
                int16_t aX, aY, aZ;
                int16_t gX, gY, gZ;
                mpu.getMotion6(&aX, &aY, &aZ, &gX, &gY, &gZ);
                // According to the datasheet we have to divide by the set sensitivity
                   // multiply by 9.81 so the output will be in ms-2 instead of Gs
                data.imu.accel.x = (aX / 8192.0f)* 9.81f;
                data.imu.accel.y = (aY / 8192.0f)* 9.81f;
                data.imu.accel.z = (aZ / 8192.0f)* 9.81f;
                data.imu.gyro.x = gX / 131.0f;
                data.imu.gyro.y = gY / 131.0f;
                data.imu.gyro.z = gZ / 131.0f;

                // update the filter, which computes orientation

                filter.updateIMU(data.imu.gyro.x, data.imu.gyro.y, data.imu.gyro.z, data.imu.accel.x, data.imu.accel.y, data.imu.accel.z);

                // heading, pitch and roll
                data.imu.eulerAngles.roll = filter.getRoll();
                data.imu.eulerAngles.pitch = filter.getPitch();
                data.imu.eulerAngles.yaw = filter.getYaw();

                // increment previous time, so we keep proper pace
                microsPrevious = microsPrevious + microsPerReading;
        }
        return true;
}

// Sensor calibration
bool IMU::calibrate()
{
        return true;
}