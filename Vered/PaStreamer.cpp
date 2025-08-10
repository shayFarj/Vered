#include "PaStreamer.h"
#include "KeysInput.h"
#include <iostream>
#define _USE_MATH_DEFINES

#include <math.h>

PaStream* PaStreamer::stream;
int PaStreamer::sampleRate = 48000;
Instrument PaStreamer::inst;

double PaStreamer::prevTime = 0;
double PaStreamer::phase = 0;
bool PaStreamer::first = true;

PaStreamer::PaStreamer()
{
    
}

PaStreamer::~PaStreamer()
{
    Pa_Sleep(1);
	Pa_StopStream(PaStreamer::stream);
	Pa_CloseStream(PaStreamer::stream);
    Pa_Terminate();
}

void PaStreamer::init(Instrument inst)
{
    PaError err = Pa_Initialize();

    if (err != paNoError)
        std::cout << "PA ERROR : " << Pa_GetErrorText(err) << std::endl;
    else
        std::cout << "wsg bro" << std::endl;

    PaError err2 = Pa_OpenDefaultStream(&PaStreamer::stream,
        0,          /* no input channels */
        2,          /* stereo output */
        paFloat32,  /* 32 bit floating point output */
        PaStreamer::sampleRate,
        1,        /* frames per buffer, i.e. the number
                           of sample frames that PortAudio will
                           request from the callback. Many apps
                           may want to use
                           paFramesPerBufferUnspecified, which
                           tells PortAudio to pick the best,
                           possibly changing, buffer size.*/
        PaStreamer::callback, /* this is your callback function */
        nullptr);

    if (err2 != paNoError)
        std::cout << "PA ERROR : " << Pa_GetErrorText(err2) << std::endl;
    else
        std::cout << "wsg bro" << std::endl;

    PaStreamer::inst = inst;

    PaError err3 = Pa_StartStream(PaStreamer::stream);

    if (err3 != paNoError)
        std::cout << Pa_GetErrorText(err3) << std::endl;
}

int PaStreamer::callback(const void* input,
    void* output,
    unsigned long frameCount,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    static float time = 0;

    const double(&press)[24] = KeysInput::getKeysPress();
    const double(&release)[24] = KeysInput::getKeysRelease();

    //const double(&press)[24] = KeysInput::getKeysPress();
    //const double(&release)[24] = KeysInput::getKeysRelease();

    double delta = 1.0 / PaStreamer::sampleRate;

    float* out = (float*) output;

    for (int i = 0; i < frameCount; i++)
    {

        double frame = 0;
        for (int j = 0; j < 24; j++)
        {
            double channel = 0;
            double p1 = press[j];

            double r1 = release[j];

            if (r1 > 0)
            {
                channel = PaStreamer::inst.Output(110.0 * pow(2, j / 12.0), p1, r1 + i * delta) * 0.2;
            }
            else
            {
                if (p1 != 0)
                    channel = PaStreamer::inst.Output(110.0 * pow(2, j / 12.0), p1 + i * delta, 0) * 0.2;
            }



            frame += channel;

        }

        *out = frame;
        out++;
        *out = frame;
        out++;

    }

    //for (int i = 0; i < frameCount; i++)
    //{

    //    float frame = 0;
    //    

    //    //frame = cos(2 * M_PI * 440 * (time + delta * i));
    //    frame = PaStreamer::inst.Output(220, (time + delta * i), 0);


    //    *out = frame;
    //    out++;
    //    *out = frame;
    //    out++;

    //}

    PaStreamer::prevTime = time;

    time += delta * frameCount;

    KeysInput::update(delta * frameCount);

    return 0;
}