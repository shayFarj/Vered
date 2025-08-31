#pragma once
#include "Envelope.h"
#include <vector>
#include <string>
class Cascade;
class Operator
{
public:
	

	Operator(int mult, double mod_index,int fb,double fb_mult,int detune,Envelope env);
	Operator();

	Operator(Operator& rhs);
	Operator& operator=(Operator& rhs);
	~Operator();

	
	double modulated(double freq,double time,double release, Operator* end= nullptr);
	double modulated(double freq, double time, double release,std::vector<Cascade*>& cList, Operator* end = nullptr);

	double Output(double freq, double time, double release);

	void getData(std::string& content);
private:
	friend class Cascade;
	friend class CasBoard;

	Operator* in = nullptr;

	int mult;
	int fb;
	int detune;
	double fb_mult;
	double mod_index;

	double cos_phase(double freq, double time,double release,Operator * end = nullptr);
	double cos_phase(double freq, double time, double release, std::vector<Cascade*>& cList, Operator* end = nullptr);

	double sin_phase(double freq, double time,double release, Operator * end = nullptr);
	double sin_phase(double freq, double time, double release,std::vector<Cascade*>& cList, Operator* end = nullptr);

	double sin_phase_fb(double freq, double time, double release,int fb_left, Operator* end = nullptr);
	double cos_phase_fb(double freq, double time, double release,int fb_left, Operator* end = nullptr);

	double sin_phase_fb(double freq, double time, double release, int fb_left, std::vector<Cascade*>& cList, Operator* end = nullptr);
	double cos_phase_fb(double freq, double time, double release, int fb_left, std::vector<Cascade*>& cList, Operator* end = nullptr);

	double inline freqOp(double freq);

	Envelope env;

};

