#ifndef CONTROL_HPP
#define CONTROL_HPP

#include "color.hpp"
#include "flame.hpp"
#include "rain.hpp"
#include "uniform.hpp"

struct LampState {
    enum class Effect : int8_t {
        Uniform,
        Rain,
        Flame,
    };

    bool on = false;
    Effect effect = Effect::Uniform;
    union {
        Uniform uniform;
        Rain rain;
        Flame flame;
    };
    int h = 0;
    int s = 0;
    int v = 10;

    LampState() {
    }
};

template <class Clock, class irin_t, class cout_t>
struct Control : public nbavr::Task<Clock> {
    enum Button : int16_t {
        Repeat = -1,
        Power = 162,
        Mode = 98,
        Mute = 226,
        PlayPause = 34,
        Rewind = 2,
        FastForward = 194,
        Equaliser = 224,
        Minus = 168,
        Plus = 144,
        Zero = 104,
        Shuffle = 152,
        USD = 176,
        One = 48,
        Two = 24,
        Three = 122,
        Four = 16,
        Five = 56,
        Six = 90,
        Seven = 66,
        Eight = 74,
        Nine = 82,
    };

    LampState& lampState;
    irin_t& irin;
    cout_t& cout;
    Button mPreviousButton;

    Control(LampState& lampState, irin_t& irin, cout_t& cout) : lampState(lampState), irin(irin), cout(cout) {
    }

    void loop() override {
        int16_t m = 0;
        bool& on = lampState.on;
        auto& effect = lampState.effect;
        int& h = lampState.h;
        int& s = lampState.s;
        int& v = lampState.v;

        if(irin.pop(&m)) {
            Button button = Button(m);

            repeat: ;

            switch(button) {
            case Button::Power:
                on = !on;
                break;
            case Button::Mode:
                if(effect == LampState::Effect::Uniform) {
                    effect = LampState::Effect::Rain;
                    lampState.rain = Rain(Clock::getTicks());
                } else if(effect == LampState::Effect::Rain) {
                    effect = LampState::Effect::Flame;
                    lampState.flame = Flame();
                } else {
                    effect = LampState::Effect::Uniform;
                    lampState.uniform = Uniform();
                }
                break;
            case Button::Rewind:
                h -= 2;
                break;
            case Button::FastForward:
                h += 2;
                break;
            case Button::Minus:
                v -= 1;
                break;
            case Button::Plus:
                v += 1;
                break;
            case Button::One:
                s -= 2;
                break;
            case Button::Two:
                break;
            case Button::Three:
                s += 2;
                break;
            case Button::Four:
                break;
            case Button::Five:
                break;
            case Button::Six:
                break;
            case Button::Seven:
                break;
            case Button::Eight:
                break;
            case Button::Nine:
                break;
            case Button::Repeat:
                if((mPreviousButton == Button::Minus)
                || (mPreviousButton == Button::Plus)
                || (mPreviousButton == Button::One)
                || (mPreviousButton == Button::Rewind)
                || (mPreviousButton == Button::FastForward)
                || (mPreviousButton == Button::Two)
                || (mPreviousButton == Button::Three)
                || (mPreviousButton == Button::Four)
                || (mPreviousButton == Button::Five)
                || (mPreviousButton == Button::Six)
                || (mPreviousButton == Button::Seven)
                || (mPreviousButton == Button::Eight)
                || (mPreviousButton == Button::Nine)) {
                    button = mPreviousButton;
                    goto repeat;
                }
                break;
            default:
                break;
            }

            mPreviousButton = button;

            if(h > 360) {
                h -= 360;
            } else if(h < 1) {
                h += 360;
            }

            s = nbavr::clip(s, 0, 100);
            v = nbavr::clip(v, 1, 100);
        }
    }
};

#endif
