#ifndef _INCL_GUARD
#define _INCL_GUARD
#include "../noiser.h"
#endif

class Saw : public Oscillator {
    public:
    float _detune;
    float _phase;
    Saw() {
        _detune = 0.0f;
        _phase = 0.0f;
    }
    ~Saw() {}
    float next(float frequency){
        frequency += frequency * _detune;
        if (_phase > 1.0f){
            _phase-= 1.0f;
        } 
        float blep = (1 + polyblep(frequency, _phase));
        float val = (_phase * 2.0f) - blep;
        _phase = _phase + frequency; // TODO it seems phase is not persistent. its always 0.0 when next() is called
        return val;
    }
};