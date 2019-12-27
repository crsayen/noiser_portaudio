#ifndef _INCL_GUARD
#define _INCL_GUARD
#include "utility.h"
#include "assets/sin_table.h"
#include "oscillators/oscillator.h"
#include "oscillators/sine.h"
#include "oscillators/triangle.h"
#include "oscillators/saw.h"
#include "oscillators/square.h"
#include "voices/voice.h"
#include "voices/supersaw.h"
#include <queue>
#endif
//#include <SPI.h>
//#include <TimerOne.h>
#define VOCT A8
#define CV1 A7
#define POT1 A5
#define POT2 A9
#define OCT_UP 2
#define OCT_DOWN 1
#define SS 8
#define FREQ_DIVISOR 8190.0f
#define BOUNCE_DURATION 100

//unsigned __exidx_start;
//unsigned __exidx_end;
float fine = 0.0f;
float v_in = 0.0f;
float freq = 0.0f;
float unison_mapped = 0.0f;
float fcode = 0.0f;
uint16_t code = 0;
int unison_read = 0;
float unison_detune = 0.0f;
int last_unison = 0;
long last_fm = 0;
float fm = 1.0f;
volatile long lastl;
volatile bool tck = false;
volatile float oct = 0.0f;
//std::queue<uint16_t> buffer;
//IntervalTimer tick;
float low_c =  16.35159781f;
float sr_factor = 2.0833333333e-05;

