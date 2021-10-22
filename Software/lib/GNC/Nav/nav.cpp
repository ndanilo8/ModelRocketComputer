#include <Nav/nav.h>
#include <data.h>
#include <MadgwickAHRS.h>

MPU6050 mpu(I2C_IMU_ADDR);
Madgwick filter;

bool Nav::begin()
{
        mpu.initialize();
        if (mpu.testConnection())
        {
                //TODO check refreshRate values
                filter.begin(25);
                microsPerReading = 1000000 / 25;
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
Nav will work when not under accel.. but under acceleration data will get corrupted
to maintain attitute looging use other method.. kalman filter or dead reckoning: gyro integration for position and change local to global fram of reference
*/
bool Nav::update()
{
        // check if it's time to read data and update the filter
        microsNow = micros();
        if (microsNow - microsPrevious >= microsPerReading)
        {
                mpu.getMotion6(&data.imu.accel.x, &data.imu.accel.y, &data.imu.accel.z, &data.imu.gyro.x, &data.imu.gyro.y, &data.imu.gyro.z);
                // update the filter, which computes orientation
                filter.updateIMU(data.imu.gyro.x, data.imu.gyro.y, data.imu.gyro.z, data.imu.accel.x, data.imu.accel.y, data.imu.accel.z);

                // print the heading, pitch and roll
                data.imu.eulerAngles.roll = filter.getRoll();
                data.imu.eulerAngles.pitch = filter.getPitch();
                data.imu.eulerAngles.yaw = filter.getYaw();
#if is_DEBUG
                Serial.println("Orientation: x,    y,     z");
                Serial.print(data.imu.eulerAngles.yaw);
                Serial.print(" ");
                Serial.print(data.imu.eulerAngles.pitch);
                Serial.print(" ");
                Serial.print(data.imu.eulerAngles.roll);
#endif
                // increment previous time, so we keep proper pace
                microsPrevious = microsPrevious + microsPerReading;
        }
        return true;
}

// calibrate gyro mostly... ZRO calibration
bool Nav::calibrate()
{
        return true;
}