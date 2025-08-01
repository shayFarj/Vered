#include "Operator.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

Operator::Operator(int mult, double mod_index,Envelope env) : mult(mult), mod_index(mod_index), in(nullptr),env(env)
{

}

Operator::Operator(): mult(1),mod_index(0.0001),in(nullptr),env(0,1,2,1,3,1,0)
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

double Operator::modulated(double freq, double time,double release,Operator* end)
{
	if (this->in != nullptr && this != end)
		return this->env.calc(time, release) * sin(2 * M_PI * (freq * this->mult * (release + time) + this->in->cos_phase(freq, time,release,end)));
	else
		return this->env.calc(time, release) * this->Output(freq, time,release);
}

