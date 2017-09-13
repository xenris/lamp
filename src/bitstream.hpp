#ifndef BIT_STREAM_HPP
#define BIT_STREAM_HPP

#include <nbavr.hpp>

template <class T, bool LeftToRight = false>
class BitStream {
    T mValue = 0;
    uint8_t mSize = 0;

public:

    force_inline void push(uint8_t b) {
        if(mSize == (sizeof(T) * 8)) {
            return;
        }

        if constexpr (LeftToRight) {
            mValue = (mValue >> 1) | (b ? nbavr::bv(sizeof(T) * 8 - 1) : 0);
        } else {
            mValue = (mValue << 1) | (b ? 1 : 0);
        }

        mSize++;
    }

    force_inline uint8_t size() {
        return mSize;
    }

    bool done() {
        return mSize == (sizeof(T) * 8);
    }

    force_inline void reset() {
        mValue = 0;
        mSize = 0;
    }

    force_inline T value() {
        return mValue;
    }
};

#endif
