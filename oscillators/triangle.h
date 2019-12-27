#ifndef _INCL_GUARD
#define _INCL_GUARD
#include "../noiser.h"
#endif

class Triangle : public Oscillator {
    public:
    float _pulsewidth;
    Triangle() {
        _lastval = 1.0f;
    }
    ~Triangle() {}
    float next(float frequency){
        frequency+= frequency * _detune;
        _phase = (_phase > 1.0f) ? _phase -1.0f : _phase;
        float val = (_phase < _pulsewidth) ? 1.0 : -1.0;
        val += polyblep(frequency, _phase);
        val -= polyblep(frequency, fmod(_phase + _pulsewidth, 1.0)); // TODO: fix polyblep for pw != 0.5
        val = frequency * val + (1.0 - frequency) * _lastval;
        _lastval = val;
        val = val * 4.0;
        _phase+= frequency;
        return val;
    }

    private:
    float _lastval;
};


