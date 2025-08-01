#include "LiveMixer.h"
#include "KeysInput.h"
#include <chrono>
#include <iostream>

LiveMixer::LiveMixer(const LiveMixer& rhs) :
	inst(rhs.inst),
	keys_press(KeysInput::getKeysPress()),
	keys_release(KeysInput::getKeysRelease()),
	delta(KeysInput::getDelta())
{

}

void LiveMixer::setInstrument(Instrument* inst)
{
	this->inst = inst;
}

LiveMixer::LiveMixer(Instrument * inst) : 
	inst(inst),
	keys_press(KeysInput::getKeysPress()),
	keys_release(KeysInput::getKeysRelease()),
	delta(KeysInput::getDelta())
{}

LiveMixer::~LiveMixer()
{

}

vered::buffer16 LiveMixer::getBufferMono16(int sample_rate)
{
	if (this->delta > 0)
		std::cout << "";

	int samples = (int)(floor(this->delta * sample_rate)) * 2;
	samples -= samples % 4;

	short* buffer = new short[samples];

	for (int i = 0; i < samples; i++)
		buffer[i] = 0;


	for (int i = 0; i < 24; i++)
	{
		double pDelta = this->keys_press[i] - this->prev_press[i];

		short* iBuffer;

		if (pDelta != 0)
		{
			iBuffer = this->inst->getArray16Stereo(220.0 * pow(2, i / 12.0), this->prev_press[i],0, samples, sample_rate);	
		}
		else
		{
			iBuffer = this->inst->getArray16Stereo(220.0 * pow(2, i / 12.0), this->prev_press[i], this->prev_release[i], samples, sample_rate, true);
		}

		for (int j = 0; j < samples; j++)
			buffer[j] += iBuffer[j];
	}

	for (int i = 0; i < 24; i++)
	{
		this->prev_press[i] = this->keys_press[i];
		this->prev_release[i] = this->keys_release[i];
	}

	return vered::buffer16(buffer, samples, sample_rate);
}