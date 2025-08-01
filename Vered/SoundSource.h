#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <queue>
#include "structs.h"
#include <chrono>

class SoundSource
{
public:
	SoundSource(int queueSize);
	~SoundSource();

	void playBuffer(vered::buffer16 buffer);
	void queueBuffer(vered::buffer16 * buffer);
	void stream();
private:

	bool firstQue = true;

	int queueSize;

	int queued = 0;

	std::queue<vered::buffer16*> bQueue;
	std::queue<ALuint> toBeFilled;

	ALuint * aBuffer;
	ALuint* bBuffer;

	ALCcontext* context;
	ALCdevice* device;

	ALuint source;
	float pitch = 1.f;
	float gain = 1.f;
	float pos[3] = { 0,0,0 };
	float vel[3] = { 0,0,0 };

	std::chrono::time_point<std::chrono::steady_clock> one;
	std::chrono::time_point<std::chrono::steady_clock> two;
};

