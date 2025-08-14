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

	double sin_phase(double freq, double time, double release);
	double cos_phase(double freq, double time, double release);

	void appendCas(Cascade* cas);
	void popCas(Cascade* cas,bool del = false);

private:
	friend class CasBoard;

	void switchOps(Operator * back,Operator * front, Operator* out);
	void deleteOp(Operator* op, Operator* out);

	Operator* carrier = nullptr;
	Operator* tail = nullptr;

	std::vector<Cascade*> in;


};

