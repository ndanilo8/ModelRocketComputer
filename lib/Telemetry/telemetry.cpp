#include <telemetry.h>

/* 
Special Thanks to Boris Du Reau & Adam Marciniak for inspiration and these thoughtfull code! When I had no idea how to mamage the format making of the telemetry packets...
Go show them some love! 

Du Reau's github here: https://github.com/bdureau
Marciniak's github here: https://github.com/AdamMarciniak
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
// https://stackoverflow.com/questions/59101351/char-buffer-new-vs-char-buffer-in-c
bool Telemetry::send2uart()
{
    // if (timer.hasPassed(DATA_SAMPLE_RATE))
    // {
    //     // TODO find the correct size of the packet and buffer
    //     char packet[150];
    //     char buffer[15];
    //     strcat(packet, "telemetry,");
    //     sprintf(buffer, "%i,", data.state);
    //     strcat(packet, buffer);
    //     sprintf(buffer, "%i,", data.loopTime);
    //     strcat(packet, buffer);
    //     sprintf(buffer, "%i,", data.altimeter.altitude);
    //     strcat(packet, buffer);
    //     sprintf(buffer, "%i,", data.altimeter.temperature);
    //     strcat(packet, buffer);
    //     sprintf(buffer, "%i,", data.altimeter.verticalVelocity);
    //     strcat(packet, buffer);
    //     sprintf(buffer, "%i,", data.imu.accel.x);
    //     strcat(packet, buffer);
    //     sprintf(buffer, "%i,", data.imu.accel.y);
    //     strcat(packet, buffer);
    //     sprintf(buffer, "%i,", data.imu.accel.z);
    //     strcat(packet, buffer);
    //     sprintf(buffer, "%i,", data.imu.eulerAngles.yaw);
    //     strcat(packet, buffer);
    //     sprintf(buffer, "%i,", data.imu.eulerAngles.pitch);
    //     strcat(packet, buffer);
    //     sprintf(buffer, "%i,", data.imu.eulerAngles.roll);
    //     strcat(packet, buffer);
    //     sprintf(buffer, "%i,", data.batteryVoltage);
    //     strcat(packet, buffer);
    //     sprintf(buffer, "%i,", data.pyro1Continuity);
    //     strcat(packet, buffer);
    //     sprintf(buffer, "%i,", data.imu.eulerAngles.pitch);
    //     //TODO add checksum? https://github.com/bdureau/RocketFlightLogger/blob/140cf36829486526b69e4608e735478b8145a25b/config.cpp#L377
    //     strcat(packet, ",\n");
    //     Serial.print("$");
    //     Serial.print(packet);
    //     return true;
    // }

    if (timer.hasPassed(TELEMETRY_SAMPLE_RATE))
    {
        // TODO find the correct size of the packet and buffer
        char packet[150];
        char buffer[15];
        switch (telemetryState)
        {
        case 0:
            sprintf(buffer, "%d", data.state);
            strcpy(packet, "State");
            telemetryState += 1;
            break;
        case 1:
            dtostrf(data.loopTime, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "loopTime");
            telemetryState += 1;
            break;
        case 2:
            dtostrf(data.altimeter.altitude, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "Alt");
            telemetryState += 1;
            break;
        case 3:
            dtostrf(data.altimeter.temperature, 6, 2, buffer); // Leave room for too large
            strcpy(packet, "Temp");
            telemetryState += 1;
            break;
        case 4:
            dtostrf(data.altimeter.verticalVelocity, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "verticalVel");
            telemetryState += 1;
            break;
        case 5:
            dtostrf(data.imu.accelTotal, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "aTotal");
            telemetryState += 1;
            break;
        case 6:
            dtostrf(data.imu.accel.x, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "Ax");
            telemetryState += 1;
            break;
        case 7:
            dtostrf(data.imu.accel.y, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "Ay");
            telemetryState += 1;
            break;
        case 8:
            dtostrf(data.imu.accel.z, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "Az");
            telemetryState += 1;
            break;
        case 9:
            dtostrf(data.imu.gyro.x, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "Gx");
            telemetryState += 1;
            break;
        case 10:
            dtostrf(data.imu.gyro.y, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "Gy");
            telemetryState += 1;
            break;
        case 11:
            dtostrf(data.imu.gyro.z, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "Gz");
            telemetryState += 1;
            break;
        case 12:
            dtostrf(data.imu.eulerAngles.yaw, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "yaw");
            telemetryState += 1;
            break;
        case 13:
            dtostrf(data.imu.eulerAngles.pitch, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "pitch");
            telemetryState += 1;
            break;
        case 14:
            dtostrf(data.imu.eulerAngles.roll, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "roll");
            telemetryState += 1;
            break;
        case 15:
            dtostrf(data.pyro1Continuity, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "pyroCont");
            telemetryState += 1;
            break;
        case 16:
            dtostrf(data.batteryVoltage, 6, 2, buffer); // Leave room for too large numbers!
            strcpy(packet, "battVolt");
            telemetryState = 0;
            break;
        }
        strcat(packet, ","); // Seperate the data by ","
        strcat(packet, buffer);
        // sendTelemetry(packet);
        timer.restart();
    }
}