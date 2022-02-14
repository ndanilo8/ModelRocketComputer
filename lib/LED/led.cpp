#include <led.h>

bool LED::begin()
{
    pinMode(LED_BUILTIN, OUTPUT);
    // pinMode(BUZZER_PIN, OUTPUT);
    return true;
}

void LED::led_ON()
{
    digitalWrite(LED_BUILTIN, HIGH);
}
void LED::led_OFF()
{
    digitalWrite(LED_BUILTIN, LOW);
}

void LED::blinkFast()
{
    if (ledBlinkTimer.hasPassed(LED_FAST_BLINK_DURATION))
    {
        if (state == false)
        {
            led_OFF();
            // noTone(BUZZER_PIN);
            state = true;
        }

        if (state == true)
        {
            // tone(BUZZER_PIN, 500);
            led_ON();
            state = false;
        }
        ledBlinkTimer.restart();
    }
}

void LED::blinkSlow()
{

    if (ledBlinkTimer.hasPassed(LED_SLOW_BLINK_DURATION))
    {
        if (state == false)
        {
            led_OFF();
           // noTone(BUZZER_PIN);
            state = true;
        }

        if (state == true)
        {
          //  tone(BUZZER_PIN, 1000);
            led_ON();
            state = false;
        }
        ledBlinkTimer.restart();
    }
}
