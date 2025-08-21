#include "Operator.h"
#include "Cascade.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

Operator::Operator(int mult, double mod_index,Envelope env) : mult(mult), mod_index(mod_index), in(nullptr),env(env)
{

}

Operator::Operator(): mult(1),mod_index(0.0001),in(nullptr),env(0,1,2,1,3,1,1)
{
	
}

Operator::~Operator()
{

}



double Operator::Output(double freq, double time,double release)
{
	return this->env.calc(time,release)*sin(2 * M_PI * (freq * this->mult * (time + release)));
}

double Operator::cos_phase(double freq, double time,double release,Operator * end)
{
	if (this->in != nullptr && this != end)
		return this->env.calc(time, release) * this->mod_index * (freq * this->mult) * cos(2 * M_PI * (freq * this->mult * (release + time) + this->in->sin_phase(freq, time,release,end)));
	else
		return this->env.calc(time, release) * this->mod_index * (freq * this->mult) * cos(2 * M_PI * (freq * this->mult * (time + release)));
}

double Operator::sin_phase(double freq, double time,double release,Operator * end)
{
	if (this->in != nullptr && this != end)
		return this->env.calc(time, release) * this->mod_index * (freq * this->mult) * sin(2 * M_PI * (freq * this->mult * (time + release) + this->in->cos_phase(freq, time,release,end)));
	else
		return this->env.calc(time, release) * this->mod_index * (freq * this->mult) * sin(2 * M_PI * (freq * this->mult * (time + release)));
}

double Operator::sin_phase(double freq, double time, double release,std::vector<Cascade*>& cList, Operator* end)
{
	if (this->in != nullptr && this != end)
		return this->env.calc(time, release) * this->mod_index * (freq * this->mult) * sin(2 * M_PI * (freq * this->mult * (time + release) + this->in->cos_phase(freq, time, release,cList, end)));
	else
	{
		double inPhase = 0;

		for (Cascade* c : cList)
		{
			inPhase += c->cos_phase(freq, time, release);
		}

		return this->env.calc(time, release) * this->mod_index * (freq * this->mult) * sin(2 * M_PI * (freq * this->mult * (time + release) + inPhase));
	}
}

double Operator::cos_phase(double freq, double time, double release, std::vector<Cascade*>& cList, Operator* end)
{
	if (this->in != nullptr && this != end)
		return this->env.calc(time, release) * this->mod_index * (freq * this->mult) * cos(2 * M_PI * (freq * this->mult * (time + release) + this->in->sin_phase(freq, time, release,cList, end)));
	else
	{
		double inPhase = 0;

		for (Cascade* c : cList)
		{
			inPhase += c->sin_phase(freq, time, release);
		}

		return this->env.calc(time, release) * this->mod_index * (freq * this->mult) * cos(2 * M_PI * (freq * this->mult * (time + release) + inPhase));
	}
}

double Operator::modulated(double freq, double time,double release,Operator* end)
{
	if (this->in != nullptr && this != end)
		return this->env.calc(time, release) * sin(2 * M_PI * (freq * this->mult * (release + time) + this->in->cos_phase(freq, time,release,end)));
	else
		return this->env.calc(time, release) * this->Output(freq, time,release);
}

double Operator::modulated(double freq, double time, double release, std::vector<Cascade*>& cList, Operator* end)
{
	if (this->in != nullptr && this != end)
		return this->env.calc(time, release) * sin(2 * M_PI * (freq * this->mult * (release + time) + this->in->cos_phase(freq, time, release,cList, end)));
	else

	{
		double inPhase = 0;

		for (Cascade* c : cList)
		{
			inPhase += c->cos_phase(freq, time, release);
		}

		return this->env.calc(time, release) * sin(2 * M_PI * (freq * this->mult * (release + time) + inPhase));
	}
}

void Operator::getData(std::string& content)
{
	content += std::to_string(this->mult);
	content += ":";
	content += std::to_string(this->mod_index);
	content += ":";
	this->env.getData(content);
}