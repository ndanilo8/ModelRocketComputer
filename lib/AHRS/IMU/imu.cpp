#include "imu.h"
#include "data.h"
#include <MadgwickAHRS.h>

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

void IMU::deadReckoning()
{
        //*********************************************************** Dead Reckoning ***********************************************************
        // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/arithmetic/index.htm
        // deg2rad = deg * (PI/180.0f)
        float pitch = data.imu.gyro.x * (PI / 180.0f);
        float yaw =  data.imu.gyro.y * (PI / 180.0f);
        float roll = data.imu.gyro.z * (PI / 180.0f);

        // calculate the norm of the vector
        float norm = sqrtf(powf(yaw, 2) + powf(pitch, 2) + powf(roll, 2));
        norm = copysignf(max(abs(norm), 1e-9), norm); // NO DIVIDE BY 0

        q *= Quaternion::from_axis_angle(data.delta_t * norm, yaw / norm, pitch / norm, roll / norm); // from axis angle convert to rotation quaternion

        float sinr_cosp = 2 * (q.a * q.b + q.c * q.d);
        float cosr_cosp = 1 - 2 * (q.b * q.b + q.c * q.c);
        data.imu.eulerAngles.roll = atan2(sinr_cosp, cosr_cosp);

        float sinp = 2 * (q.a * q.c + -q.d * q.b);
        if (abs(sinp) >= 1)
                data.imu.eulerAngles.pitch = copysign(PI / 2, sinp); // return 90 if out of range
        else
                data.imu.eulerAngles.pitch = asin(sinp);

        float siny_cosp = 2 * (q.a * q.d + q.b * q.c);
        float cosy_cosp = 1 - 2 * (q.c * q.c + q.d * q.d);
        data.imu.eulerAngles.yaw = atan2(siny_cosp, cosy_cosp);
}

void IMU::gradientDescent()
{
        // update the filter, which computes orientation
        filter.updateIMU(data.imu.gyro.x, data.imu.gyro.y, data.imu.gyro.z, data.imu.accel.x, data.imu.accel.y, data.imu.accel.z);
        // heading, pitch and roll
        data.imu.eulerAngles.roll = filter.getRoll();
        data.imu.eulerAngles.pitch = filter.getPitch();
        data.imu.eulerAngles.yaw = filter.getYaw();
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
                // mpu.getRotation(&gX, &gY, &gZ);
                mpu.getMotion6(&aX, &aY, &aZ, &gX, &gY, &gZ);
                // According to the datasheet we have to divide by the set sensitivity
                // multiply by 9.81 so the output will be in ms-2 instead of Gs

                data.imu.accel.x = (aX / 8192.0f) * 9.81f;
                data.imu.accel.y = (aY / 8192.0f) * 9.81f;
                data.imu.accel.z = (aZ / 8192.0f) * 9.81f;

                data.imu.gyro.x = gX / 131.0f;
                data.imu.gyro.y = gY / 131.0f;
                data.imu.gyro.z = gZ / 131.0f;

                gradientDescent(); // Madgwick Filter
                //deadReckoning(); // Gyro Integration

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