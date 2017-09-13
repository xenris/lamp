#ifndef LAMP_HPP
#define LAMP_HPP

#include <nbavr.hpp>

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Color() : r(0), g(0), b(0) {}

    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

    Color& operator=(const uint32_t c) {
        r = (c >> 16) & 0xff;
        g = (c >> 8) & 0xff;
        b = (c >> 0) & 0xff;

        return *this;
    }
};

template <class Clock, class LedStripPin, class cout_t, class irin_t>
class Lamp : public nbavr::Task<Clock> {
    cout_t& cout;
    irin_t& irin;
    bool& irActive;
    Color m[10][10];

public:
    Lamp(cout_t& cout, irin_t& irin, bool& irActive) : cout(cout), irin(irin), irActive(irActive) {
        block LedStripPin::direction(nbavr::Direction::Output);
        clear();
    }

private:
    void loop() override {
        int16_t b;
        while(irin.pop(&b)) {
            cout << b << '\n';
        }

        if(!irActive) {
            static int8_t i = 0;

            clear();

            i = (i + 1) % 10;

            m[i][i] = 0x010101;

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
            block LedStripPin::output(nbavr::Value::High);
            Clock::template delay<50>();
            block LedStripPin::output(nbavr::Value::Low);
            // Clock::template delay<700>();
        } else {
            block LedStripPin::output(nbavr::Value::High);
            Clock::template delay<250>();
            block LedStripPin::output(nbavr::Value::Low);
            // Clock::template delay<150>();
        }
        // if(bit == 0) {
        //     block LedStripPin::output(nbavr::Value::High);
        //     Clock::template delay<200>();
        //     block LedStripPin::output(nbavr::Value::Low);
        //     Clock::template delay<750>();
        // } else {
        //     block LedStripPin::output(nbavr::Value::High);
        //     Clock::template delay<750>();
        //     block LedStripPin::output(nbavr::Value::Low);
        //     Clock::template delay<200>();
        // }
    }

    static force_inline void finish() {
        Clock::template delay<55000>();
    }
};

#endif
