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

typedef Colour (*ShaderFunc)(float, float, void*);

template <class ledStripPin>
class Renderer {
    static constexpr int8_t width = 10;
    static constexpr int8_t height = 12;
    Colour buffer[width][height];
    ShaderFunc shader;
    void* data;
    int8_t x = 0;
    int8_t y = 0;

public:
    Renderer(ShaderFunc shader) : shader(shader) {
        ledStripPin::direction(ledStripPin::Direction::Output);
    }

    void setData(void* d) {
        data = d;
    }

    bool render() {
        buffer[x][y] = shader(float(x) / width, float(y) / height, data);

        x += 1;

        if(x == width) {
            x = 0;
            y += 1;

            if(y == height) {
                y = 0;

                sendBuffer();

                return true;
            }
        }

        return false;
    }

    void sendBuffer() {
        atomic {
            int8_t offset = 0;

            for(int8_t y = 0; y < height; y++) {
                for(int8_t x = 0; x < width; x += 2) {
                    int8_t tx = wrap(x - offset, width);
                    sendColour(buffer[tx][y]);
                }

                offset += 1;
            }
        }
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

    int8_t wrap(int8_t x, int8_t w) {
        while(x < 0) {
            x += w;
        }

        while(x >= w) {
            x -= w;
        }

        return x;
    }
};

#endif
