#ifndef IR_HPP
#define IR_HPP

#include <nbavr.hpp>
#include "bitstream.hpp"

// Pushes received bytes to given stream.
// Pushes -1 on repeat signal.

template <class Clock, class irin_t>
struct Ir : nbavr::Task<Clock> {
    static constexpr uint16_t HeaderLength = 3335;
    static constexpr uint16_t RepeatLength = 2778;
    static constexpr uint16_t ZeroLength = 287;
    static constexpr uint16_t OneLength = 554;
    static constexpr uint16_t Threshold = 30;

    static constexpr uint32_t MaxTimeForRepeat = 50000;

    using InputCapture = typename Clock::Timer::InputCapture;

    enum class State : int8_t {
        Waiting,
        Listening,
    };

    irin_t& irin;
    bool& irActive;
    nbavr::Queue<uint16_t, 10> inputQueue;
    State mState = State::Waiting;
    BitStream<uint32_t> mBitStream;
    uint32_t lastMessageTimestamp = 0;

    Ir(irin_t& irin, bool& irActive) : irin(irin), irActive(irActive) {
        InputCapture::callback(inputCallback, this);
        InputCapture::edge(InputCapture::Edge::Falling);
        InputCapture::intEnable(true);
    }

    void loop() override {
        static uint16_t p;

        uint16_t v;

        while(inputQueue.pop(&v)) {
            int16_t d = v - p;

            if(near(d, HeaderLength)) {
                mState = State::Listening;
                mBitStream.reset();
            } else if(near(d, RepeatLength)) {
                irActive = false;
                mState = State::Waiting;
                mBitStream.reset();

                uint32_t now = Clock::getTicks();

                if((now - lastMessageTimestamp) < MaxTimeForRepeat) {
                    irin.push(-1);
                    lastMessageTimestamp = now;
                }
            } else if(mState == State::Listening) {
                if(near(d, ZeroLength)) {
                    mBitStream.push(0);
                } else if(near(d, OneLength)) {
                    mBitStream.push(1);
                } else {
                    irActive = false;
                    mState = State::Waiting;
                }

                if(mBitStream.done()) {
                    irActive = false;
                    mState = State::Waiting;

                    uint32_t v = mBitStream.value();

                    uint8_t a = v >> 24;
                    uint8_t b = v >> 16;
                    uint8_t c = v >> 8;
                    uint8_t d = v;

                    if(((a ^ b) == 0xff) && ((c ^ d) == 0xff)) {
                        irin.push((a << 8) | c);
                        lastMessageTimestamp = Clock::getTicks();
                    }

                    mBitStream.reset();
                }
            } else {
                irActive = false; // XXX
            }

            p = v;
        }

        this->sleep();
    }

    static bool near(uint16_t a, uint16_t b) {
        return nbavr::abs(int16_t(a - b)) <= int16_t(Threshold);
    }

    static void inputCallback(void* data) {
        Ir* self = (Ir*)data;

        self->inputQueue.push_(InputCapture::value());
        self->irActive = true;
        self->wake();
    }
};

#endif
