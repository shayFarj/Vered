#pragma once
#include <vector>
#include "Cascade.h"
#include "structs.h"
class Instrument
{
public:
	Instrument();
	~Instrument();

	double Output(double freq,double time,double release);
	void setCas(std::vector<Cascade*> cas);
	void appendCas(Cascade* cas);
	void eraseCas(int index);

	 vered::buffer16 getBuffer16Stereo(double freq, double base_time, double end_time, int sample_rate);
	 short* getArray16Stereo(double freq, double base_time,double r_time, int samples, int sample_rate, bool onRelease = false);

private:
	std::vector<Cascade*> cas;
};

