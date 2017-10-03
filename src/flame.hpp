#ifndef FLAME_HPP
#define FLAME_HPP

#include "color.hpp"

struct Flame {
    void process(Color (&m)[10][10], int h, int s, int v) {
        // TODO
        for(Color (&cs)[10] : m) {
            for(Color& c : cs) {
                c = 0;
            }
        }
    }
};

#endif
