#include "Cascade.h"
#include <iostream>

Cascade::Cascade()
{

}

Cascade::~Cascade()
{
}

double Cascade::Output(double freq, double time,double release)
{
	return this->carrier->modulated(freq,time,release,this->tail->in);
}

double Cascade::modulated(double freq, double time,double release)
{
	return this->carrier->modulated(freq, time,release);
}

void Cascade::Append(Operator* carrier)
{
	std::cout <<"Tail : " << this->tail << " Carrier : " << this->carrier << std::endl;
	if (this->carrier != nullptr)
	{
		carrier->in = this->carrier;
		this->carrier = carrier;
	}
	else
	{
		if (this->in != nullptr)
		{
			carrier->in = this->in->carrier;
		}
		this->carrier = carrier;
		this->tail = carrier;
	}
}