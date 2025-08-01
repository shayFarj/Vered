#include "FourierCalc.h"
#include <thread>
#include <iostream>
#include "Instrument.h"
#define _USE_MATH_DEFINES
#include <math.h>

void work(FourierCalc& calc, int index, Instrument& inst,int freq, int time_steps, double time_step, double time_base,int division)
{
	int partLen = calc.data_len / division;

	for (int i = partLen * index; i < partLen + partLen*index && !calc.interrupted; i++)
	{
		double cFreq = calc.start_freq + i * calc.freq_step;
		double sum = 0;

		for (double j = time_base; j <= time_step * time_steps && !calc.interrupted;j += time_step)
		{
			sum += inst.Output(freq,j,0) * cos(- 2 * M_PI * cFreq * j);
		}
		sum /= time_steps;

		calc.data[i] = sum;

		//ive added one to i so it reaches 1 (fair)
		calc.status[index] = (double(i + 1 - partLen * index)) / partLen;
	}

	bool finished = true;
	for (int i = 0; i < division; i++)
	{
		if (calc.status[i] != 1)
			finished = false;
	}

	calc.onwork.store(!finished);
	calc.ready.store(finished);

}

FourierCalc::FourierCalc(int data_len,int start_freq,int end_freq) : data_len(data_len), freq_step(((double)(abs(end_freq - start_freq))) / data_len),start_freq(start_freq < end_freq ? start_freq : end_freq)
{
	this->data = new double[data_len];
}

FourierCalc::~FourierCalc()
{
	delete[] this->data;
	this->interrupted.store(true);
}

double* FourierCalc::getRange()
{
	double* range = new double[this->data_len];
	for (int i = 0; i < this->data_len; i++)
	{
		range[i] = this->start_freq + i * this->freq_step;
	}
	return range;
}

int FourierCalc::getDataLen()
{
	return this->data_len;
}

int FourierCalc::getStatusLen()
{
	return this->status_len;
}

double* FourierCalc::copyData()
{
	if (!this->isReady()) return nullptr;

	double* cData = new double[this->data_len];

	for (int i = 0; i < this->data_len; i++)
		cData[i] = this->data[i];

	return cData;
}

void FourierCalc::resetData()
{
	delete[] this->data;

	this->data = new double[this->data_len];
	this->data_len = this->data_len;
}

void FourierCalc::resetData(int data_len,int start_freq,int end_freq)
{
	delete[] this->data;

	this->start_freq = start_freq < end_freq ? start_freq : end_freq;
	this->freq_step = ((double)(abs(start_freq - end_freq))) / data_len;

	this->data = new double[data_len];
	this->data_len = data_len;
}

bool FourierCalc::onWork()
{
	return this->onwork.load();
}

bool FourierCalc::isReady()
{
	return this->ready.load();
}

double* FourierCalc::getStatus()
{
	double* cStatus = new double[2];

	for (int i = 0; i < 2; i++)
	{
		cStatus[i] = this->status[i];
	}

	return cStatus;

}

void FourierCalc::init(Instrument& inst, int freq, int time_steps, double time_step, double time_base, int threads)
{
	if (this->data_len % threads != 0)
		throw std::exception("FourierCalc : Data can't devide by number of threads");

	delete[] this->status;
	this->status = new double[threads];
	this->status_len = threads;

	auto selfRef = std::ref(*this);
	auto instRef = std::ref(inst);

	std::thread* t = new std::thread[threads];

	for (int i = 0; i < threads; i++)
	{
		t[i] = std::thread(&work, selfRef, i, instRef, freq, time_steps, time_step, time_base, threads);
	}

	this->onwork.store(true);

	for (int i = 0; i < threads; i++)
	{
		t[i].detach();
	}

}
