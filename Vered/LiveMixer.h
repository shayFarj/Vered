#pragma once
#include "Instrument.h"
#include "structs.h"
#include <chrono>
class LiveMixer
{
public:
	LiveMixer(const LiveMixer& rhs);
	LiveMixer(Instrument * inst);
	~LiveMixer();

	void setInstrument(Instrument* inst);
	vered::buffer16 getBufferMono16(int sample_rate);
	vered::bufferDouble getDemoBufferMono(int sampleRate);

private:
	const double(&keys_press)[24];
	const double(&keys_release)[24];
	const double& delta;

	double prev_press[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double prev_release[24] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	Instrument * inst;


	bool firstUse = false;
};

