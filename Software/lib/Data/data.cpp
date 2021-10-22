#include <data.h>

Telemetry_t data;
void goToState(State_t state)
{
    data.state = state;
}