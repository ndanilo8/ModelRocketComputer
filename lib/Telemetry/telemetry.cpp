#include <telemetry.h>

/* 
Special Thanks to Boris Du Reau for inspiration on the telemetry packets
Check his github here: https://github.com/bdureau
*/

Chrono timer;

bool Telemetry::begin()
{
    // TODO start up SDcard
    return true;
}

/*
Send Telemetry to SD Card

 */
//TODO test
bool Telemetry::send2uart()
{
    if (timer.hasPassed(DATA_SAMPLE_RATE))
    {
        // TODO find the correct size
        char packet[150];
        char buffer[15];
        strcat(packet, "telemetry,");
        sprintf(buffer, "%i,", data.state);
        strcat(packet, buffer);
        sprintf(buffer, "%i,", data.loopTime);
        strcat(packet, buffer);
        sprintf(buffer, "%i,", data.altimeter.altitude);
        strcat(packet, buffer);
        sprintf(buffer, "%i,", data.altimeter.temperature);
        strcat(packet, buffer);
        sprintf(buffer, "%i,", data.altimeter.verticalVelocity);
        strcat(packet, buffer);
        sprintf(buffer, "%i,", data.imu.accel.x);
        strcat(packet, buffer);
        sprintf(buffer, "%i,", data.imu.accel.y);
        strcat(packet, buffer);
        sprintf(buffer, "%i,", data.imu.accel.z);
        strcat(packet, buffer);
        sprintf(buffer, "%i,", data.imu.eulerAngles.yaw);
        strcat(packet, buffer);
        sprintf(buffer, "%i,", data.imu.eulerAngles.pitch);
        strcat(packet, buffer);
        sprintf(buffer, "%i,", data.imu.eulerAngles.roll);
        strcat(packet, buffer);
        sprintf(buffer, "%i,", data.batteryVoltage);
        strcat(packet, buffer);
        sprintf(buffer, "%i,", data.pyro1Continuity);
        strcat(packet, buffer);
        sprintf(buffer, "%i,", data.imu.eulerAngles.pitch);

        //TODO add checksum? https://github.com/bdureau/RocketFlightLogger/blob/140cf36829486526b69e4608e735478b8145a25b/config.cpp#L377
        strcat(packet, ";\n");
        Serial.print("$");
        Serial.print(packet);

        return true;
    }
}