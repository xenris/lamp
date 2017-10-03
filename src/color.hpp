#ifndef COLOR_HPP
#define COLOR_HPP

#include <nbavr.hpp>

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Color() : r(0), g(0), b(0) {}

    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
    Color(float r, float g, float b) : r(r * float(255)), g(g * float(255)), b(b * float(255)) {}

    Color& operator=(const uint32_t c) {
        r = (c >> 16) & 0xff;
        g = (c >> 8) & 0xff;
        b = (c >> 0) & 0xff;

        return *this;
    }

    static Color fromHSV(float h, float s, float v) {
        h /= 360;
        s /= 100;
        v /= 100;

        h *= 6;

        float c = v * s;
        float x = c * (1.0f - nbavr::abs(nbavr::fmod(h, 2.0f) - 1.0f));

        float r;
        float g;
        float b;

        if(h < 1) {
            r = c;
            g = x;
            b = 0;
        } else if(h < 2) {
            r = x;
            g = c;
            b = 0;
        } else if(h < 3) {
            r = 0;
            g = c;
            b = x;
        } else if(h < 4) {
            r = 0;
            g = x;
            b = c;
        } else if(h < 5) {
            r = x;
            g = 0;
            b = c;
        } else {
            r = c;
            g = 0;
            b = x;
        }

        float m = v - c;

        return Color(r + m, g + m, b + m);
    }
};

#endif
