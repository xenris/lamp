#include "main.hpp"

INCLUDE_DEFAULT_CALLBACK();
INCLUDE_TIMERCOUNTER_OUTPUT_CALLBACK(1, A);
INCLUDE_TIMERCOUNTER_OUTPUT_CALLBACK(1, B);
INCLUDE_TIMERCOUNTER_OVERFLOW_CALLBACK(1);
INCLUDE_TIMERCOUNTER_INPUT_CALLBACK(1);
INCLUDE_USART_CALLBACK(0, RX);
INCLUDE_USART_CALLBACK(0, DE);

void main() {
    const uint32_t CpuFreq = 16000000;

    using LedStripPin = nbavr::PinC1;
    using SystemTimer = nbavr::TimerCounter1;
    using SerialUsart = nbavr::Usart0;
    using cout_t = nbavr::Queue<char, 60>;
    using irin_t = nbavr::Queue<int16_t, 20>;

    using Clock = nbavr::Clock<SystemTimer, CpuFreq>;

    static cout_t cout;
    static irin_t irin;
    // Used to prevent led updating while an ir message is being received.
    // Otherwise the message will be missed.
    static bool irActive = false;

    nbavr::Serial<SerialUsart, cout_t>::init(CpuFreq, 115200, &cout);

    static Ir<Clock, irin_t> ir(irin, irActive);
    static Lamp<Clock, LedStripPin, cout_t, irin_t> lamp(cout, irin, irActive);

    static nbavr::Task<Clock>* tasks[] = {&lamp, &ir};

    static nbavr::TaskManager<Clock> tm(tasks);
}
