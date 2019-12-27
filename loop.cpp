#include "noiser.h"
Supersaw test(5); // 5 concurrent saws
float f;

void loopinit(){
    test.detune(0.001); // detuned by this ratio. set this to
}                       // bigger values to hear horrible sounds.
                        // some bigger values sound good. try 0.499

// the poorly named loop function gets called repeatedly by main.cpp
float loop() {
    f = 220; // frequency in hz
    return test.next(f * sr_factor); // sr_factor = 1/samplerate (44.1khz)
}
