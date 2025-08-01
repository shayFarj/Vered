#pragma once
#include "Operator.h"
class Cascade
{
public:
	Cascade();
	~Cascade();

	double Output(double freq, double time,double release);
	double modulated(double freq, double time,double release);

	void Append(Operator* carrier);

private:
	Operator* carrier = nullptr;
	Operator* tail = nullptr;

	Cascade * in = nullptr;


};

