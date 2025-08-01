#pragma once
#include "Envelope.h"


class Operator
{
public:
	

	Operator(int mult, double mod_index,Envelope env);
	Operator();

	~Operator();

	
	double modulated(double freq,double time,double release, Operator* end= nullptr);
	double Output(double freq, double time, double release);


private:
	friend class Cascade;

	Operator* in = nullptr;

	int mult;
	double mod_index;

	double cos_phase(double freq, double time,double release,Operator * end = nullptr);
	double sin_phase(double freq, double time,double release, Operator * end = nullptr);

	Envelope env;

};

