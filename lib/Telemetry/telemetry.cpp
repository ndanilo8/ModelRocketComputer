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
    Serial1.begin(RADIO_BAUD_RATE);
    radio.begin(Serial1);
    return true;
}

// TODO test the downlink with a radio
//  https://stackoverflow.com/questions/59101351/char-buffer-new-vs-char-buffer-in-c
bool oneTime = true;
bool Telemetry::send2radio()
{
    // print the header at system start and never run again...
    if (oneTime)
    {
        String header = "state,loopTime,kal_altitude,temperature,kal_verticalVelocity,accelX,accelY,accelZ,yaw,pitch,roll,pyroCont,batteryVoltage";
#if is_DEBUG
        Serial.println(header); // send over the usb serial
        delay(800);
#endif
        oneTime = false;
    }

    if (timer.hasPassed(timeRate))
    {
        // FIXME check buffers size
        char packet[256];
        char buffer[16];

        sprintf(buffer, "%d,", data.state);
        strcat(packet, buffer);

        dtostrf(data.delta_t, 6, 2, buffer); // Leave room for too large numbers!
        strcat(packet, buffer);
        strcat(packet, ",");

        dtostrf(data.altimeter.altitude, 6, 2, buffer); // Leave room for too large numbers!
        strcat(packet, buffer);
        strcat(packet, ",");

        dtostrf(data.altimeter.temperature, 6, 2, buffer); // Leave room for too large
        strcat(packet, buffer);
        strcat(packet, ",");

        dtostrf(data.altimeter.verticalVelocity, 6, 2, buffer); // Leave room for too large numbers!
        strcat(packet, buffer);
        strcat(packet, ",");

        dtostrf(data.imu.accel.x, 6, 2, buffer); // Leave room for too large numbers!
        strcat(packet, buffer);
        strcat(packet, ",");

        dtostrf(data.imu.accel.y, 6, 2, buffer); // Leave room for too large numbers!
        strcat(packet, buffer);
        strcat(packet, ",");

        dtostrf(data.imu.accel.z, 6, 2, buffer); // Leave room for too large numbers!
        strcat(packet, buffer);
        strcat(packet, ",");

        dtostrf(data.imu.eulerAngles.yaw, 6, 2, buffer); // Leave room for too large numbers!
        strcat(packet, buffer);
        strcat(packet, ",");

        dtostrf(data.imu.eulerAngles.pitch, 6, 2, buffer); // Leave room for too large numbers!
        strcat(packet, buffer);
        strcat(packet, ",");

        dtostrf(data.imu.eulerAngles.roll, 6, 2, buffer); // Leave room for too large numbers!
        strcat(packet, buffer);
        strcat(packet, ",");

        dtostrf(data.pyro1Continuity, 6, 2, buffer); // Leave room for too large numbers!
        strcat(packet, buffer);
        strcat(packet, ",");

        dtostrf(data.batteryVoltage, 6, 2, buffer); // Leave room for too large numbers!
        strcat(packet, buffer);

        // Add checksum? https://github.com/bdureau/RocketFlightLogger/blob/140cf36829486526b69e4608e735478b8145a25b/config.cpp#L377
        strcat(packet, ";\n"); // Seperate each packet
#if is_DEBUG
        // Serial.print("$");
        Serial.print(packet); // send over the usb serial
#endif
        radio.sendDatum(packet); // send data via radio to GCS (ground control Station)
        timer.restart();
        return true;
    }
}