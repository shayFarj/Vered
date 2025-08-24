#pragma once
#include "FourierCalc.h"
class FourierWin
{
public:
	FourierWin(Instrument * inst);
	~FourierWin();

	void render();

private:

	Instrument * inst;

	FourierCalc calc;

	bool showGraph = false;

	double freq_start=0;
	double freq_step = 1;
	int freq_steps = 1000;
	double freq = 130.81;
	int threads = 4;

	int time_steps = 1000;
	double time_step = 0.0001;
	double time_base = 0;
	double r_time_base = 0;
};

