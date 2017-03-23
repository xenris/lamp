#include "main.hpp"

#include <nbavr.cpp>

Stream<char>* stdoutp;

void main() {
    typedef PinC0 ledStripPin;
    typedef TimerCounter1 clockTimer;
    typedef Usart0 serialUsart;

    StreamBuffer<char, 60> stdout;
    StreamBuffer<char, 0> stdin;

    stdoutp = &stdout;

    stdout << "start" << endl;

    ClockT<clockTimer> clock;

    Serial<serialUsart> serial(stdout, stdin);
    Lamp<ledStripPin> lamp(clock);

    Task* tasks[] = {&serial, &lamp};

    nbavr(clock, tasks);
}
