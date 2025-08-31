#include "Operator.h"
#include "Cascade.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "PaStreamer.h"

Operator::Operator(int mult, double mod_index,int fb,double fb_mult,int detune,Envelope env) : mult(mult),
mod_index(mod_index),
fb(fb),
fb_mult(fb_mult),
detune(detune), in(nullptr), env(env)
{

}


Operator::Operator(Operator& rhs)
{
	this->in = nullptr;
	this->mod_index = rhs.mod_index;
	this->mult = rhs.mult;
	this->fb = rhs.fb;
	this->fb_mult = rhs.fb_mult;
	this->detune = rhs.detune;
	this->env = rhs.env;
}

Operator& Operator::operator=(Operator& rhs)
{
	if (this != &rhs) {
		this->in = nullptr;
		this->mod_index = rhs.mod_index;
		this->mult = rhs.mult;
		this->fb = rhs.fb;
		this->fb_mult = rhs.fb_mult;
		this->detune = rhs.detune;
		this->env = rhs.env;
	}
	return *this;
}


Operator::Operator(): mult(1),mod_index(0.0001),fb(0), in(nullptr), fb_mult(0),
detune(0), env(0, 1, 2, 1, 3, 1, 1)
{
	
}

Operator::~Operator()
{

}



double Operator::Output(double freq, double time,double release)
{
	double opFreq = this->freqOp(freq);
	return this->env.calc(time,release)*sin(2 * M_PI * (opFreq * (time + release)));
}

double Operator::sin_phase_fb(double freq, double time, double release, int fb_left, Operator* end)
{
	double opFreq = this->freqOp(freq);

	if (fb_left > 0)
	{
		return this->env.calc(time, release) * opFreq * this->mod_index * sin(2 * M_PI * (opFreq * (release + time) + this->cos_phase_fb(freq, time, release, fb_left - 1, end)));
	}
	else
	{
		if (this != end && this->in != nullptr) {

			return this->in->sin_phase(freq, time, release, end);
		}
		else
		{
			return 0;
		}
	}
}

double Operator::cos_phase_fb(double freq, double time, double release, int fb_left, Operator* end)
{
	double opFreq = this->freqOp(freq);

	if (fb_left > 0)
	{
		return this->env.calc(time, release) * opFreq * this->mod_index * cos(2 * M_PI * (opFreq * (release + time) + this->sin_phase_fb(freq, time, release, fb_left - 1, end)));
	}
	else
	{
		if (this != end && this->in != nullptr) {

			return this->in->cos_phase(freq, time, release, end);
		}
		else
		{
			return 0;
		}
	}
}

double Operator::cos_phase_fb(double freq, double time, double release, int fb_left, std::vector<Cascade*>& cList, Operator* end)
{
	double opFreq = this->freqOp(freq);

	if (fb_left > 0)
	{
		return this->env.calc(time, release) * opFreq * this->mod_index * cos(2 * M_PI * (opFreq* (release + time) + this->sin_phase_fb(freq, time, release, fb_left - 1, cList, end)));
	}
	else
	{
		if (this != end && this->in != nullptr) {

			return this->in->cos_phase(freq, time, release, cList, end);
		}
		else
		{
			double inPhase = 0;

			for (Cascade* c : cList)
			{
				inPhase += c->cos_phase(freq, time, release);
			}

			return inPhase;
		}
	}
}

double Operator::sin_phase_fb(double freq, double time, double release, int fb_left, std::vector<Cascade*>& cList, Operator* end)
{
	double opFreq = this->freqOp(freq);

	if (fb_left > 0)
	{
		return this->env.calc(time, release) * opFreq * this->mod_index * sin(2 * M_PI * (opFreq * (release + time) + this->cos_phase_fb(freq, time, release, fb_left - 1, cList, end)));
	}
	else
	{
		if (this != end && this->in != nullptr)
			return this->in->sin_phase(freq, time, release, cList, end);
		else
		{
			double inPhase = 0;

			for (Cascade* c : cList)
			{
				inPhase += c->sin_phase(freq, time, release);
			}

			return inPhase;
		}
	}
}

double Operator::cos_phase(double freq, double time,double release,Operator * end)
{
	double opFreq = this->freqOp(freq);

	if (this->fb > 0) {
		return this->env.calc(time, release) * this->mod_index * opFreq * cos(2 * M_PI * (opFreq * (release + time) + this->fb_mult * this->sin_phase_fb(freq, time, release, this->fb, end)));
	}
	else {
		if (this->in != nullptr && this != end)
			return this->env.calc(time, release) * this->mod_index * opFreq * cos(2 * M_PI * (opFreq * (release + time) + this->in->sin_phase(freq, time, release, end)));
		else
			return this->env.calc(time, release) * this->mod_index * opFreq * cos(2 * M_PI * (opFreq * (time + release)));
	}
}

double Operator::sin_phase(double freq, double time,double release,Operator * end)
{
	double opFreq = this->freqOp(freq);
	if (this->fb > 0) {
		return this->env.calc(time, release) * this->mod_index * opFreq * sin(2 * M_PI * (opFreq * (time + release) + this->fb_mult * this->cos_phase_fb(freq, time, release, this->fb, end)));
	}
	else {
		if (this->in != nullptr && this != end)
			return this->env.calc(time, release) * this->mod_index * opFreq * sin(2 * M_PI * (opFreq * (time + release) + this->in->cos_phase(freq, time, release, end)));
		else
			return this->env.calc(time, release) * this->mod_index * opFreq * sin(2 * M_PI * (opFreq * (time + release)));
	}
}

double Operator::sin_phase(double freq, double time, double release,std::vector<Cascade*>& cList, Operator* end)
{
	double opFreq = this->freqOp(freq);

	if (this->fb > 0) {
		return this->env.calc(time, release) * this->mod_index * opFreq * sin(2 * M_PI * (opFreq * (time + release) + this->fb_mult * this->in->cos_phase_fb(freq, time, release, this->fb, cList, end)));
	}
	else {
		if (this->in != nullptr && this != end)
			return this->env.calc(time, release) * this->mod_index * opFreq * sin(2 * M_PI * (opFreq * (time + release) + this->in->cos_phase(freq, time, release, cList, end)));
		else
		{
			double inPhase = 0;

			for (Cascade* c : cList)
			{
				inPhase += c->cos_phase(freq, time, release);
			}

			return this->env.calc(time, release) * this->mod_index * opFreq * sin(2 * M_PI * (opFreq * (time + release) + inPhase));
		}
	}
}

double Operator::cos_phase(double freq, double time, double release, std::vector<Cascade*>& cList, Operator* end)
{
	double opFreq = this->freqOp(freq);

	if (this->fb > 0) {

		return this->env.calc(time, release) * this->mod_index * opFreq * cos(2 * M_PI * (opFreq * (time + release) + this->fb_mult * this->sin_phase_fb(freq, time, release, this->fb, cList, end)));
	}
	else {
		if (this->in != nullptr && this != end)
			return this->env.calc(time, release) * this->mod_index * opFreq * cos(2 * M_PI * (opFreq * (time + release) + this->in->sin_phase(freq, time, release, cList, end)));
		else
		{
			double inPhase = 0;

			for (Cascade* c : cList)
			{
				inPhase += c->sin_phase(freq, time, release);
			}

			return this->env.calc(time, release) * this->mod_index * opFreq * cos(2 * M_PI * (opFreq * (time + release) + inPhase));
		}
	}
}

double inline Operator::freqOp(double freq)
{
	return this->mult * freq * pow(2, (this->detune / 12.0) / 128.0);
}

double Operator::modulated(double freq, double time,double release,Operator* end)
{
	double opFreq = this->freqOp(freq);


	if (this->fb > 0) {
		return this->env.calc(time, release) * sin(2 * M_PI * (opFreq * (release + time) + this->fb_mult * this->cos_phase_fb(freq, time, release, this->fb, end)));
	}

	else
		if (this != end && this->in != nullptr)
			return this->env.calc(time, release) * sin(2 * M_PI * (opFreq * (release + time) + this->in->cos_phase(freq, time,release,end)));
		else
			return this->env.calc(time, release) * this->Output(freq, time,release);
}

double Operator::modulated(double freq, double time, double release, std::vector<Cascade*>& cList, Operator* end)
{
	double opFreq = this->freqOp(freq);

	if (this->fb > 0) {
		return this->env.calc(time, release) * sin(2 * M_PI * (opFreq * (release + time) + this->fb_mult * this->sin_phase_fb(freq, time, release, this->fb, cList, end)));
	}
	else {
		if (this != end && this->in != nullptr)
			return this->env.calc(time, release) * sin(2 * M_PI * (opFreq * (release + time) + this->in->cos_phase(freq, time, release, cList, end)));
		else

		{
			double inPhase = 0;

			for (Cascade* c : cList)
			{
				inPhase += c->cos_phase(freq, time, release);
			}

			return this->env.calc(time, release) * sin(2 * M_PI * (opFreq * (release + time) + inPhase));
		}
	}
}

void Operator::getData(std::string& content)
{
	content += std::to_string(this->mult);
	content += ":";
	content += std::to_string(this->mod_index);
	content += ":";
	content += std::to_string(this->fb);
	content += ":";
	content += std::to_string(this->fb_mult);
	content += ":";
	content += std::to_string(this->detune);
	content += ":";
	this->env.getData(content);
}