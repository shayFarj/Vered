#include "Cascade.h"
#include <iostream>

Cascade::Cascade()
{

}

Cascade::~Cascade()
{
	Operator* iter = this->carrier;
	while (iter != nullptr)
	{
		Operator* opIn = iter->in;
		delete iter;
		iter = opIn;
	}
	this->carrier = nullptr;
	this->tail = nullptr;
}

bool Cascade::isCarrierNull()
{
	return this->carrier == nullptr;
}

Cascade::Cascade(const Cascade& rhs)
{
	if (rhs.carrier != nullptr) {
		this->carrier = new Operator(*rhs.carrier);

		Operator* iter = this->carrier;
		Operator* rIter = rhs.carrier->in;
		while (rIter != nullptr)
		{
			iter->in = new Operator(*rIter);
			iter = iter->in;

			rIter = rIter->in;
		}
		this->tail = iter;
	}

}

Cascade& Cascade::operator=(const Cascade& rhs)
{
	if (this != &rhs) {
		if (rhs.carrier != nullptr) {
			this->carrier = new Operator(*rhs.carrier);

			Operator* iter = this->carrier;
			Operator* rIter = rhs.carrier->in;
			while (rIter != nullptr)
			{
				iter->in = new Operator(*rIter);
				iter = iter->in;

				rIter = rIter->in;
			}
			this->tail = iter;
		}
	}
	return *this;
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
	if(this->carrier != nullptr)
		return this->carrier->sin_phase(freq, time, release, this->in, this->tail);
}

double Cascade::cos_phase(double freq, double time, double release)
{
	if(this->carrier != nullptr)
		return this->carrier->cos_phase(freq, time, release, this->in, this->tail);
}

void Cascade::Prepend(Operator* tail)
{
	if (this->carrier != nullptr)
	{
		tail->in = this->tail->in;
		this->tail->in = tail;
		this->tail = tail;
	}
	else
	{
		this->carrier = tail;
		this->tail = tail;
	}
}

void Cascade::Append(Operator* carrier)
{
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
	int index = this->findCas(cas);

	
	if (!(index != -1 || cas == nullptr || cas == this)) {
		this->in.push_back(cas);
	}
}

std::string Cascade::getChainStr()
{
	std::string chain = "";
	Operator* iter = this->carrier;
	while (iter != nullptr)
	{

		chain += std::to_string((unsigned long long)iter) + " ";
		iter = iter->in;
	}
	return chain;
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

void Cascade::popCas(Cascade* cas)
{
	int index = findCas(cas);
	if (index != -1)
	{
		this->in.erase(this->in.begin() + index);
	}
}

int Cascade::findCas(Cascade* cas)
{
	int i = 0;
	for (; i < this->in.size() && this->in[i] != cas; i++);

	return i == this->in.size() ? -1 : i;
}

void Cascade::getData(std::string& content,std::string& pos)
{

	Operator* iter = this->carrier;
	while (iter != nullptr)
	{
		content += pos;

		iter->getData(content);
		iter = iter->in;
	}
}