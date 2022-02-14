#include <sd.h>

/*
Special Thanks to Adam Marciniak for inspiration and his thoughtfull code!
Go show some love!
Marciniak's github here: https://github.com/AdamMarciniak
*/

bool SDLogger::writeFileHeader()
{
    String header = "state, loopTime, flightTime, kal_altitude, temperature, kal_verticalVelocity, accelX, accelY, accelZ, gyroX, gyroY, gyroZ, yaw, pitch, roll, pyroCont, batteryVoltage";
    file.println(header);
    file.close();
    return true;
}

bool SDLogger::begin()
{
    if (sd.begin(SDCARD_CS, SD_SCK_MHZ(50)))
    {
#if is_DEBUG
        Serial.println("Starting SdCard --> OK!");
#endif
        while (sd.exists(fileName))
        {
            if (fileName[BASE_NAME_SIZE + 1] != '9')
            {
                fileName[BASE_NAME_SIZE + 1]++;
            }
            else if (fileName[BASE_NAME_SIZE] != '9')
            {
                fileName[BASE_NAME_SIZE + 1] = '0';
                fileName[BASE_NAME_SIZE]++;
            }
            else
            {
                return false;
            }
        }
        if (!file.open(fileName, O_WRONLY | O_CREAT | O_EXCL))
        {
#if is_DEBUG
            Serial.println("Didn't open file!");
#endif
            return false;
        }
        delay(100);
        writeFileHeader(); // print fileHeader to SD
#if is_DEBUG
        Serial.println("Wrote CSV header");
#endif

        return true;
    }
    else
    {
#if is_DEBUG
        Serial.println("Starting SdCard --> FAILED!");
#endif
        return false;
    }
}

// this will log to SD Card if the vehicle is not on IDLE or LANDED state
// for debugging purpuses it is enabled for TEST mode
// TODO find a way to not close and open each loop....
// maybe a triple FIFO Buffer method
bool SDLogger::logData()
{
    file.open(fileName, FILE_WRITE);
    if (file)
    {
        if (data.state != LANDED || data.state != IDLE)
        {
            file.print(data.state);
            file.write(',');
            file.print(data.delta_t, numDecimals);
            file.write(',');
            file.print(data.flightTime);
            file.write(',');
            file.print(data.kalman.altitude, numDecimals);
            file.write(',');
            file.print(data.altimeter.temperature, numDecimals);
            file.write(',');
            file.print(data.kalman.verticalVelocity, numDecimals);
            file.write(',');
            file.print(data.imu.accel.x, numDecimals);
            file.write(',');
            file.print(data.imu.accel.y, numDecimals);
            file.write(',');
            file.print(data.imu.accel.z, numDecimals);
            file.write(',');
            file.print(data.imu.gyro.x, numDecimals);
            file.write(',');
            file.print(data.imu.gyro.y, numDecimals);
            file.write(',');
            file.print(data.imu.gyro.z, numDecimals);
            file.write(',');
            file.print(data.imu.eulerAngles.yaw, numDecimals);
            file.write(',');
            file.print(data.imu.eulerAngles.pitch, numDecimals);
            file.write(',');
            file.print(data.imu.eulerAngles.roll, numDecimals);
            file.write(',');
            file.print(data.pyro1Continuity, numDecimals);
            file.write(',');
            file.print(data.batteryVoltage, numDecimals);
            file.println();
            file.close();
            return true;
        }
    }
}