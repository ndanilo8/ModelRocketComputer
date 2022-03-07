#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <global.h>
#include <config.h>
#include <data.h>
#include <Chrono.h>

class LED
{

public:
    LED(){};
    bool begin();

    void blinkFast();

    void blinkSlow();

private:
    void led_ON();
    void led_OFF();
    Chrono ledBlinkTimer;
    bool state = false;
};

#endif /* LED_H */
