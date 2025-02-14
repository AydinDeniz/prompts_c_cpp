
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portaudio.h>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512

typedef struct {
    float echoBuffer[44100];
    int echoIndex;
} AudioData;

static int audioCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData) {
    AudioData *data = (AudioData*)userData;
    float *in = (float*)inputBuffer;
    float *out = (float*)outputBuffer;

    for (unsigned int i = 0; i < framesPerBuffer; i++) {
        float sample = in[i];
        float echo = data->echoBuffer[data->echoIndex];
        data->echoBuffer[data->echoIndex] = sample * 0.6f + echo * 0.4f;
        data->echoIndex = (data->echoIndex + 1) % 44100;
        out[i] = sample + echo * 0.5f;
    }
    return paContinue;
}

int main() {
    PaStream *stream;
    AudioData data = {{0}, 0};

    Pa_Initialize();
    Pa_OpenDefaultStream(&stream, 1, 1, paFloat32, SAMPLE_RATE,
                         FRAMES_PER_BUFFER, audioCallback, &data);
    Pa_StartStream(stream);

    printf("Real-time audio processing running... Press Enter to stop.\n");
    getchar();

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    return 0;
}
