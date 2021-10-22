#include <data.h>

uint16_t rateHz = DATA_SAMPLE_RATE;
uint16_t numSeconds = DATA_SAMPLE_TOTAL_TIME;
uint16_t millisPerSample = 1000 / rateHz;
uint16_t totalSamples = rateHz * numSeconds;

Telemetry_t data;
void goToState(State_t state)
{
    data.state = state;
}