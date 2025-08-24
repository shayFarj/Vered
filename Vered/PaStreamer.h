#pragma once
#include "imgui.h"
#include "portaudio.h"
#include "Instrument.h"

static class PaStreamer
{
public:
	PaStreamer();
	~PaStreamer();

	static void init(Instrument * inst);

	static void pauseStream();
	static void unpauseStream();
	static int getOctave();
	static void setOctave(int octave);

private:
	static Instrument * inst;

	static int sampleRate;

	static int octave;

	static PaStream * stream;
	
	static int callback(const void* input,
		void* output,
		unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void* userData);

	static double phase;
	static double prevTime;

	static bool first;
};

