#ifndef LAMP_HPP
#define LAMP_HPP

#include <nbavr.hpp>
#include "renderer.hpp"

template <class ledStripPin>
class Lamp : public Task {
    Clock& clock;
    Renderer<ledStripPin> renderer;
    float rotation = 0;

public:
    Lamp(Clock& clock) : clock(clock), renderer(shader) {
        renderer.setData(&rotation);
    }

private:
    void loop() override {
        if(renderer.render()) {
            rotation += M_PI / 10;
            delay(clock, MS_TO_TICKS(1000 / 20));
        }
    }

    static Colour shader(float x, float y, void* data) {
        float* rotation = static_cast<float*>(data);

        uint8_t n = 5 * (1 + sin(M_PI * x + *rotation));

        uint8_t r = n;
        uint8_t g = 0;
        uint8_t b = 0;

        return {r, g, b};
    }
};

#endif
