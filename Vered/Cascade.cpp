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
	if (this->carrier != nullptr)
		return this->carrier->modulated(freq, time, release, this->tail);
	else
		return 0;
}

double Cascade::modulated(double freq, double time,double release)
{
	if (this->carrier != nullptr)
		return this->carrier->modulated(freq, time, release, this->in, this->tail);
	else
		return 0;
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
	
	if (!valid || cas == nullptr || cas == this) {
		throw("Invalid cascade appended");
	}

	this->in.push_back(cas);

}

void Cascade::switchOps(Operator* back, Operator* front, Operator* out)
{
	if (back == this->tail) {
		this->tail = front;
	}
	if (front == this->carrier)
		this->carrier = back;

	if (out != nullptr)
		out->in = back;

	front->in = back->in;
	back->in = front;
	
}

void Cascade::deleteOp(Operator* op, Operator* out)
{
	if (out != nullptr)
	{
		out->in = op->in;
	}

	if (op == this->tail)
		this->tail = out;
	if (op == this->carrier)
		this->carrier = op->in;

	delete op;
}

void Cascade::popCas(Cascade* cas, bool del)
{
	int i = 0;
	for (i = 0; i < this->in.size() && this->in[i] != cas; i++);

	if (i != this->in.size())
	{
		//if(del)
		//	delete this->in[i];
		this->in.erase(this->in.begin() + i);
	}
}