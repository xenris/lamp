#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <nbavr.hpp>
#undef _delay_us
#undef _delay_ms
#include <util/delay.h>

struct Colour {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

template <class ledStripPin>
class Renderer {
    static constexpr int8_t width = 10;
    static constexpr int8_t height = 12;
    Colour buffer[width][height];

public:
    Renderer() {
        ledStripPin::direction(ledStripPin::Direction::Output);
    }

    void render(Colour (*shader)(rational16<1>, rational16<1>, void*), rational16<4> rotation, rational16<1> blur, void* data) {
        for(int8_t y = 0; y < height; y++) {
            for(int8_t x = 0; x < width; x++) {
                buffer[x][y] = shader(rational16<1>(x, width), rational16<1>(y, height), data);
                // TODO blur with neighbouring pixels.
            }
        }

        // TODO send colour information to led strips.
    }

    void sendColours(Colour* colours, uint8_t count, bool ensureFinish) {
        atomic {
            for(uint8_t i = 0; i < count; i++) {
                sendColour(colours[i]);
            }

            if(ensureFinish) {
                finish();
            }
        }
    }

    void sendColour(Colour colour) {
        sendByte(colour.green);
        sendByte(colour.red);
        sendByte(colour.blue);
    }

    force_inline void sendByte(uint8_t b) {
        for(uint8_t n = 0x80; n; n >>= 1) {
            sendBit(b & n);
        }
    }

    force_inline void sendBit(uint8_t bit) {
        if(bit == 0) {
            ledStripPin::value(ledStripPin::Value::High);
            _delay_us(0.25);
            ledStripPin::value(ledStripPin::Value::Low);
            _delay_us(0.80);
        } else {
            ledStripPin::value(ledStripPin::Value::High);
            _delay_us(0.80);
            ledStripPin::value(ledStripPin::Value::Low);
            _delay_us(0.25);
        }
    }

    void finish() {
        _delay_us(55);
    }
};

#endif
