#ifndef _INCL_GUARD
#define _INCL_GUARD
#include "../noiser.h"
#endif

class Square : public Oscillator {
    public:
    float _pulsewidth;
    Square() {}
    ~Square() {}
    float next(float frequency){
        frequency+= frequency * _detune;
        _phase = (_phase > 1.0f) ? _phase -1.0f : _phase;
        float val = (_phase < _pulsewidth) ? 1.0 : -1.0;
        val += polyblep(frequency, _phase);
        val -= polyblep(frequency, fmod(_phase + _pulsewidth, 1.0));
        _phase+= frequency;
        return val;
    }
};