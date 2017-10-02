#ifndef CONTROL_HPP
#define CONTROL_HPP

struct LampState {
    bool on = false;
    int8_t brightness = 1;
    int8_t redOffset = 0;
    int8_t greenOffset = 0;
    int8_t blueOffset = 0;
};

template <class Nbavr, class irin_t>
struct Control : public nbavr::Task<Nbavr> {
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
    Button mPreviousButton;

    Control(LampState& lampState, irin_t& irin) : lampState(lampState), irin(irin) {
    }

    void loop() override {
        int16_t m = 0;

        if(irin.pop(&m)) {
            Button button = Button(m);

            repeat: ;

            switch(button) {
            case Button::Power:
                lampState.on = !lampState.on;
                break;
            case Button::Plus:
                lampState.brightness++;
                // lampState.red++;
                // lampState.green++;
                // lampState.blue++;
                break;
            case Button::Minus:
                lampState.brightness--;
                break;
            case Button::One:
                lampState.redOffset++;
                break;
            case Button::Two:
                lampState.greenOffset++;
                break;
            case Button::Three:
                lampState.blueOffset++;
                break;
            case Button::Seven:
                lampState.redOffset--;
                break;
            case Button::Eight:
                lampState.greenOffset--;
                break;
            case Button::Nine:
                lampState.blueOffset--;
                break;

                break;
            case Button::Repeat:
                if((mPreviousButton == Button::Plus)
                || (mPreviousButton == Button::Minus)
                || (mPreviousButton == Button::One)
                || (mPreviousButton == Button::Two)
                || (mPreviousButton == Button::Three)
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
        }

        lampState.brightness = nbavr::clip(lampState.brightness, int8_t(0), int8_t(16));
        lampState.redOffset = nbavr::clip(lampState.redOffset, int8_t(0), int8_t(16));
        lampState.greenOffset = nbavr::clip(lampState.greenOffset, int8_t(0), int8_t(16));
        lampState.blueOffset = nbavr::clip(lampState.blueOffset, int8_t(0), int8_t(16));
    }
};

#endif
