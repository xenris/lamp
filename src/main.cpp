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

    using LedStripPin = nbavr::hw::PinC1;
    using SystemTimer = nbavr::hw::TimerCounter1;
    using irin_t = nbavr::Queue<int16_t, 8>;

    using Clock = nbavr::Clock<SystemTimer, CpuFreq>;

    static LampState lampState;
    static irin_t irin;
    // Used to prevent led updating while an ir message is being received.
    // Otherwise the message will be missed.
    static bool irActive = false;

    // TODO Have another task which actually does the effects.
    static Control<Clock, irin_t> control(lampState, irin);
    static Ir<Clock, irin_t> ir(irin, irActive);
    static Lamp<Clock, LedStripPin> lamp(lampState, irActive);

    static nbavr::Task<Clock>* tasks[] = {&control, &lamp, &ir};

    static nbavr::TaskManager<Clock> tm(tasks);
}
