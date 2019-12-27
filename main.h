#include "pa_ringbuffer.h"
//#include "pa_unix_util.h"
#include "pa_allocation.h"
#include "pa_util.h"
//#include <pa_util.h>

typedef struct{
    PaUtilRingBuffer    rBufToRT;
    void*               rBufToRTData;

    PaUtilRingBuffer    rBufFromRT;
    void*               rBufFromRTData;
} portaudio_data;