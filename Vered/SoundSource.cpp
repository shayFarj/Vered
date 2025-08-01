#include "SoundSource.h"
#include <iostream>
#include "structs.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <thread>



SoundSource::SoundSource(int queueSize): queueSize(queueSize)
{
	

	this->device = alcOpenDevice(nullptr); // default device
	if (!this->device)
	{
		throw("failed to get sound device");
	}
	


	this->context = alcCreateContext(this->device, nullptr);  // create context
	if (!this->context)
		throw("Failed to set sound context");

	if (!alcMakeContextCurrent(this->context))   // make context current
		throw("failed to make context current");

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(this->device, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(this->device, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(this->device) != AL_NO_ERROR)
		name = alcGetString(this->device, ALC_DEVICE_SPECIFIER);
	printf("Opened \"%s\"\n", name);	

	this->aBuffer = new ALuint[this->queueSize];
	this->bBuffer = new ALuint[this->queueSize];

	alGenBuffers(this->queueSize, this->aBuffer);
	alGenBuffers(this->queueSize, this->bBuffer);
	alGenSources(1, &this->source);
	alSourcef(this->source, AL_PITCH, this->pitch);
	alSourcef(this->source, AL_GAIN, this->gain);
	alSource3f(this->source, AL_POSITION, this->pos[0], this->pos[1], this->pos[2]);
	alSource3f(this->source, AL_VELOCITY, this->vel[0], this->vel[1], this->vel[2]);
	alSourcei(this->source, AL_LOOPING, false);

}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &this->source);
	alDeleteBuffers(this->queueSize, this->aBuffer);
}


void SoundSource::queueBuffer(vered::buffer16 * buffer16)
{
	this->bQueue.push(buffer16);
}

void SoundSource::stream()
{

	ALint processed = 0;
	alGetSourcei(this->source, AL_BUFFERS_PROCESSED, &processed);

	ALint playing = 0;

	alGetSourcei(this->source, AL_SOURCE_STATE, &playing);

	if (this->firstQue && this->queueSize*2 == this->bQueue.size())
	{
		for (int i = 0; i < this->queueSize; i++)
		{
			vered::buffer16* buffer = this->bQueue.front();

			alBufferData(this->aBuffer[i], AL_FORMAT_STEREO16, buffer->buffer, buffer->samples, buffer->sample_rate);
			delete buffer;
			this->bQueue.pop();
		}
		
		alSourceQueueBuffers(this->source, this->queueSize, this->aBuffer);
		
		alSourcePlay(this->source);
		this->firstQue = false;
		return;
	}

	if (!this->firstQue)
	{
		
		std::cout << processed << std::endl;

		while (processed > 0)
		{
			ALuint pBuffer;

			alSourceUnqueueBuffers(this->source, 1, &pBuffer);

			processed--;


			if (this->bQueue.size() != 0)
			{

				std::cout << "one will be waiting" << std::endl;
				vered::buffer16* buffer = this->bQueue.front();

				alBufferData(pBuffer, AL_FORMAT_STEREO16, buffer->buffer, buffer->samples, buffer->sample_rate);
				delete buffer;

				this->queued++;

				this->bQueue.pop();
				alSourceQueueBuffers(this->source, 1, &pBuffer);
			}
			else
				std::cout << "no one's waiting" << std::endl;

		}

		
		//	this->queued = 0;
		//}


		if (playing != AL_PLAYING && playing != AL_PAUSED)
		{
			ALint atQue = 0;
			alGetSourcei(this->source, AL_BUFFERS_QUEUED, &atQue);

			std::cout << "atQue : " << atQue << std::endl;

			if (atQue == this->queueSize) {
				
				std::cout << this->queued << " : Too much que!" << std::endl;
				this->queued = 0;
			}
		}
	}
}

void SoundSource::playBuffer(vered::buffer16 buffer16)
{ 
	
	//short* buffer = new short[48000 * 2];
	//for (int i = 0; i < 48000 *2; i++)
	//	buffer[i] = (short)floor(pow(2, 14) * sin(2*M_PI*i* (1.0 / 48000) * 440 / 8));

	//alBufferData(this->aBuffer, AL_FORMAT_MONO16, buffer, 48000 *2, 48000);

	//alSourcei(this->source, AL_BUFFER, (ALint)this->aBuffer);


	//alSourcePlay(this->source);

	//delete[] buffer;

	/*short* buffer = buffer16.buffer;

	alBufferData(this->aBuffer, AL_FORMAT_STEREO16, buffer, buffer16.samples, buffer16.sample_rate);

	alSourceQueueBuffers(this->source, 1, &this->aBuffer);

	alSourcePlay(this->source);

	delete[] buffer;*/
}