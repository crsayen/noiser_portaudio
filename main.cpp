#include <portaudio.h>
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "loop.cpp"

#define NUM_SECONDS   (5)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (200)
typedef struct{
    unsigned short     nframes;
    PaUtilRingBuffer    rBufToRT;
    void*               rBufToRTData;
    PaUtilRingBuffer    rBufFromRT;
    void*               rBufFromRTData;
    char message[20];
} paTestData;

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned long i;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;
    float ptr;
    memset(out, 0, FRAMES_PER_BUFFER * sizeof(float));
    for( i=0; i<framesPerBuffer; i++ )
    {
        if (PaUtil_GetRingBufferReadAvailable(&data->rBufToRT)){
            PaUtil_ReadRingBuffer(&data->rBufToRT, &ptr, 1);
            *out++ = ptr;
            PaUtil_WriteRingBuffer(&data->rBufFromRT, &ptr, 1);
        }
    }
    return paContinue;
}

/*
* This routine is called by portaudio when playback is done.
*/
static void StreamFinished( void* userData )
{
    paTestData *data = (paTestData *) userData;
}

/*******************************************************************/
int main(void);
int main(void)
{
    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    paTestData data;
    data.nframes = 0;
    int i;
    printf("PortAudio Test. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);

    /* initialise ring buffers */
    data.rBufToRTData = PaUtil_AllocateMemory(sizeof(float) * FRAMES_PER_BUFFER);
    if (data.rBufToRTData == NULL){
        return 1;
    }
    PaUtil_InitializeRingBuffer(&data.rBufToRT, sizeof(float), FRAMES_PER_BUFFER, data.rBufToRTData);
    data.rBufFromRTData = PaUtil_AllocateMemory(sizeof(float) * FRAMES_PER_BUFFER);
    if (data.rBufFromRTData == NULL){
        return 1;
    }
    PaUtil_InitializeRingBuffer(&data.rBufFromRT, sizeof(float) , FRAMES_PER_BUFFER, data.rBufFromRTData);
    
    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
    fprintf(stderr,"Error: No default output device.\n");
    goto error;
    }
    outputParameters.channelCount = 1;       /* mono -- Eurorack is typically monophonic but stereo isn't out of the question */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
            &stream,
            NULL, /* no input */
            &outputParameters,
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            patestCallback,
            &data );
    if( err != paNoError ) goto error;

    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    if( err != paNoError ) goto error;

    // initialize the shit - this may not be a useful function once all is said and done
    loopinit();

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    while (true){
        while (PaUtil_GetRingBufferReadAvailable(&data.rBufFromRT) > 0){
            float* ptr;
            PaUtil_ReadRingBuffer(&data.rBufFromRT, &ptr, 1);
            --data.nframes;
        }
        float result;
        while(data.nframes < FRAMES_PER_BUFFER){
            result = loop();
            PaUtil_WriteRingBuffer(&data.rBufToRT, &result, 1);
            ++data.nframes;
        }
    }

    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();
    printf("Test finished.\n");
    
    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}