#ifndef _INCL_GUARD
#define _INCL_GUARD
#include "../noiser.h"
#endif

class Sine : public Oscillator {
    public:
    Sine() {}
    ~Sine() {}
    float next(float frequency){
        frequency+= frequency * _detune;
        _phase = (_phase > 1.0f) ? _phase -1.0f : _phase;
        float val = interpolate(lut_sine, _phase, 1024.0f);
        _phase+= frequency;
        return val;
    }
};