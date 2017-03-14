#ifndef LAMP_HPP
#define LAMP_HPP

#include <nbavr.hpp>
#include "renderer.hpp"

template <class ledStripPin>
class Lamp : public Task {
    Clock& clock;
    Renderer<ledStripPin> renderer;
    int8_t count = 0;

public:
    Lamp(Clock& clock) : clock(clock) {
    }

private:
    void loop() override {
        int8_t numColours = 60;

        Colour colours[numColours];

        for(int8_t i = 0; i < numColours; i++) {
            rational16<1> x(i, numColours);
            rational16<1> y(0.0);

            colours[i] = {0, 3, 0};//calcColour(x, y);
        }

        colours[count] = {20, 20, 20};

        renderer.sendColours(colours, numColours, false);

        count = (count + 1) % 60;

        // renderer.render(shader, 0.0, 0.5, nullptr);

        delay(clock, MS_TO_TICKS(4));
    }

    static Colour shader(rational16<1> x, rational16<1> y, void* data) {
        uint8_t r = 0;
        uint8_t g = 10;
        uint8_t b = 0;

        return {r, g, b};
    }

    Colour calcColour(rational16<1> x, rational16<1> y) {
        rational16<1> time(clock.getTicks16() / 2, type_info<uint16_t>::max / 2);

        uint8_t r = 0;//1 * time;
        uint8_t g = 4;
        uint8_t b = 0;

        return {r, g, b};
    }
};

#endif
