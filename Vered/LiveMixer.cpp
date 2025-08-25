#include "LiveMixer.h"
#include "KeysInput.h"
#include <chrono>
#include <iostream>
#include "PaStreamer.h"
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

vered::bufferDouble LiveMixer::getDemoBufferMono(int sampleRate)
{
	int samples = sampleRate/100;
	double* arr = new double[samples];

	double* out = arr;

	double delta = 1.0 / sampleRate;

	for (int i = 0; i < samples; i++)
	{

		double frame = 0;
		for (int j = 0; j < 24; j++)
		{
				double channel = 0;
				double p1 = this->keys_press[j];

				double r1 = this->keys_release[j];

				if (r1 > 0)
				{
					channel = this->inst->Output(130.81 * pow(2,PaStreamer::getOctave())* pow(2, j / 12.0), p1, r1 + i * delta);
				}
				else
				{
					if(this->keys_press[j] != 0)
						channel = this->inst->Output(130.81 * pow(2, PaStreamer::getOctave()) * pow(2, j / 12.0), p1 + i * delta, 0);
				}



				frame += channel;
			
		}

		arr[i] = frame;
	
	}

	return vered::bufferDouble(arr,samples,sampleRate);

}

vered::buffer16 LiveMixer::getBufferMono16(int sample_rate)
{

	int samples = (int)(floor(0.001 * sample_rate)) * 2;
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
			iBuffer = this->inst->getArray16Stereo(130.81 * pow(2, i / 12.0), this->prev_press[i],0, samples, sample_rate);
		}
		else
		{
			iBuffer = this->inst->getArray16Stereo(130.81 * pow(2, i / 12.0), this->prev_press[i], this->prev_release[i], samples, sample_rate, true);
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