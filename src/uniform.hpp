#ifndef UNIFORM_HPP
#define UNIFORM_HPP

#include "color.hpp"

struct Uniform {
    void process(Color (&m)[10][10], int h, int s, int v) {
        Color u = Color::fromHSV(h, s, v);

        for(Color (&cs)[10] : m) {
            for(Color& c : cs) {
                c = u;
            }
        }
    }
};

#endif
