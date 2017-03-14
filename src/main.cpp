#include "main.hpp"

#include <nbavr.cpp>

void main() {
    typedef PinC0 ledStripPin;
    typedef TimerCounter1 clockTimer;

    ClockT<clockTimer> clock;

    Lamp<ledStripPin> lamp(clock);

    Task* tasks[] = {&lamp};

    nbavr(clock, tasks);
}
