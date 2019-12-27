#include "noiser.h"
Supersaw test(3);
void loopinit(){
    test.detune(0.498);
}
float loop() {
    return test.next(0.005);
}
