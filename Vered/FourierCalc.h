#pragma once
#include <atomic>
#include "Instrument.h"

class FourierCalc
{
public:
	FourierCalc(int data_len,int freq_start, int freq_end);
	~FourierCalc();


	double* getStatus();
	bool onWork();
	bool isReady();

	void resetData();
	void resetData(int data_len, int freq_start, int freq_end);

	double* copyData();
	double* getRange();
	int getStatusLen();
	int getDataLen();

	void init(Instrument& inst, int freq, int time_steps, double time_step, double time_base, int threads = 2);

private:
	double * status = new double[2];
	int status_len = 2;
	
	std::atomic<bool> onwork = false;
	std::atomic<bool> ready = false;
	std::atomic<bool> interrupted = false;
	
	double* data;
	double freq_step;
	int data_len;
	int start_freq;

	friend void work(FourierCalc& calc, int index, Instrument& inst, int freq, int time_steps, double time_step, double time_base,int devision);
};

