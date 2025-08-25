#pragma once
#include "Operator.h"
#include <string>

class Cascade
{
public:
	Cascade();
	~Cascade();
	Cascade(const Cascade& rhs);
	Cascade& operator=(const Cascade& rhs);

	double Output(double freq, double time,double release);
	double modulated(double freq, double time,double release);

	void Append(Operator* carrier);
	void Prepend(Operator* tail);

	double sin_phase(double freq, double time, double release);
	double cos_phase(double freq, double time, double release);

	void appendCas(Cascade* cas);
	void popCas(Cascade* cas,bool del = false);

	void getData(std::string& content,std::string& pos);
	std::string getChainStr();

private:
	friend class CasBoard;

	void switchOps(Operator * back,Operator * front, Operator* out);
	void deleteOp(Operator* op, Operator* out);

	Operator* carrier = nullptr;
	Operator* tail = nullptr;

	std::vector<Cascade*> in;


};

