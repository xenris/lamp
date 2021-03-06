#ifndef LAMP_HPP
#define LAMP_HPP

#include <nbavr.hpp>
#include "control.hpp"
#include "color.hpp"

template <class Clock, class LedStripPin>
class Lamp : public nbavr::Task<Clock> {
    LampState& lampState;
    bool& irActive;
    Color m[10][10];

public:
    Lamp(LampState& lampState, bool& irActive) : lampState(lampState), irActive(irActive) {
        block LedStripPin::direction(nbavr::hw::Direction::Output);
        clear();
    }

private:
    void loop() override {
        if(!irActive) {
            if(lampState.on) {
                switch(lampState.effect) {
                case LampState::Effect::Uniform:
                    lampState.uniform.process(m, lampState.h, lampState.s, lampState.v);
                    break;
                case LampState::Effect::Rain:
                    lampState.rain.process(m, Clock::getTicks(), lampState.h, lampState.s, lampState.v);
                    break;
                case LampState::Effect::Flame:
                    lampState.flame.process(m, lampState.h, lampState.s, lampState.v);
                    break;
                }
            } else {
                clear();
            }

            draw();

            this->sleep(Clock::millisToTicks(20));
        }
    }

    void draw() {
        atomic {
            for(Color (&cs)[10] : m) {
                for(Color &c : cs) {
                    sendColour(c);
                }
            }
        }
    }

    void clear() {
        for(Color (&cs)[10] : m) {
            for(Color& c : cs) {
                c = 0;
            }
        }
    }

    static force_inline void sendColour(const Color& c) {
        sendByte(c.g);
        sendByte(c.r);
        sendByte(c.b);
    }

    static force_inline void sendByte(uint8_t b) {
        for(uint8_t n = 0x80; n; n >>= 1) {
            sendBit(b & n);
        }
    }

    static force_inline void sendBit(uint8_t bit) {
        if(bit == 0) {
            block LedStripPin::output(nbavr::hw::Value::High);
            Clock::template delay<50>();
            block LedStripPin::output(nbavr::hw::Value::Low);
            // Clock::template delay<700>();
        } else {
            block LedStripPin::output(nbavr::hw::Value::High);
            Clock::template delay<250>();
            block LedStripPin::output(nbavr::hw::Value::Low);
            // Clock::template delay<150>();
        }
        // if(bit == 0) {
        //     block LedStripPin::output(nbavr::hw::Value::High);
        //     Clock::template delay<200>();
        //     block LedStripPin::output(nbavr::hw::Value::Low);
        //     Clock::template delay<750>();
        // } else {
        //     block LedStripPin::output(nbavr::hw::Value::High);
        //     Clock::template delay<750>();
        //     block LedStripPin::output(nbavr::hw::Value::Low);
        //     Clock::template delay<200>();
        // }
    }

    static force_inline void finish() {
        Clock::template delay<55000>();
    }
};

#endif
