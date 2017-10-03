#ifndef RAIN_HPP
#define RAIN_HPP

#include "color.hpp"

struct Rain {
    struct Drop {
        enum class State : int8_t {
            Blank,
            Top,
            Fall,
            Splash,
        };

        State state = State::Blank;
        uint32_t startTime = 0;
        float pos = 0;
        static constexpr float acc = 9.8 * 3;
    };

    Drop mDrops[10] = {};
    nbavr::Random rand;

    Rain(int s) : rand(s) {
    }

    void process(Color (&m)[10][10], uint32_t ticks, int h, int s, int v) {
        for(int8_t i = 0; i < 10; i++) {
            if((rand.next<uint16_t>() % 100) < 1) {
                if(mDrops[i].state == Drop::State::Blank) {
                    mDrops[i].state = Drop::State::Top;
                } else if(mDrops[i].state == Drop::State::Top) {
                    mDrops[i].state = Drop::State::Fall;
                    // mDrops[i].startTime = Clock::getTicks();
                    mDrops[i].startTime = ticks;
                }
            }
        }

        // uint32_t now = Clock::getTicks();
        uint32_t now = ticks;

        for(Drop& drop : mDrops) {
            if(drop.state == Drop::State::Fall) {
                float dTime = float(now - drop.startTime) / 250000.0f;//Clock::millisToTicks(1000);

                drop.pos = drop.acc * dTime * dTime;

                if(drop.pos > 11) {
                    drop.pos = 0;
                    drop.state = Drop::State::Blank;
                }
            }
        }

        for(int8_t i = 0; i < 10; i++) {
            for(int8_t j = 0; j < 10; j++) {
                m[i][j] = 0;
            }
        }

        for(int8_t i = 0; i < 10; i++) {
            Drop& drop = mDrops[i];

            if(drop.state == Drop::State::Fall) {
                uint8_t row = drop.pos;

                Color c = Color::fromHSV(h, s, v);

                if(row < 10) {
                    m[9 - row][i] = c;
                }
            } else if(drop.state == Drop::State::Top) {
                Color c = Color::fromHSV(h, s, v);

                m[9][i] = c;
            }
        }
    }
};

#endif
