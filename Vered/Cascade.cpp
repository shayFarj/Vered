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
	return this->carrier->modulated(freq,time,release,this->tail);
}

double Cascade::modulated(double freq, double time,double release)
{
	return this->carrier->modulated(freq, time,release,this->in,this->tail);
}

double Cascade::sin_phase(double freq, double time, double release)
{
	return this->carrier->sin_phase(freq, time, release, this->in, this->tail);
}

double Cascade::cos_phase(double freq, double time, double release)
{
	return this->carrier->cos_phase(freq, time, release, this->in, this->tail);
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
		//if (this->in != nullptr)
		//{
		//	carrier->in = this->in->carrier;
		//}
		this->carrier = carrier;
		this->tail = carrier;
	}
}

void Cascade::appendCas(Cascade* cas)
{
	bool valid = true;
	for (int i = 0; i < this->in.size(); i++)
	{
		valid = this->in[i] != cas;
		if (!valid) break;
	}
	
	if (!valid && cas == nullptr && cas != this) {
		throw("Invalid cascade appended");
	}

	this->in.push_back(cas);

}

void Cascade::popCas(Cascade* cas)
{
	int i = 0;
	for (i = 0; i < this->in.size() && this->in[i] != cas; i++);

	if (i != this->in.size())
	{
		delete this->in[i];
		this->in.erase(this->in.begin() + i);
	}
}