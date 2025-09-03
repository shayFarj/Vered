#pragma once
#include "Cascade.h"
#include <iostream>
#include <random>

namespace vered
{

	const int pos_params = 2;
	const int env_params = 7;
	const int op_params = 5 + pos_params + env_params;
	const std::string version_name = "v1";

	struct buffer16
	{
		
	public:
		buffer16(short* buffer, int samples, int sample_rate) : buffer(buffer), samples(samples), sample_rate(sample_rate)
		{
		}

		short* buffer;
		int samples;
		int sample_rate;
	};

	struct bufferDouble
	{
		bufferDouble(double* buffer, int samples, int sample_rate) : buffer(buffer), samples(samples), sample_rate(sample_rate)
		{

		}

		double* buffer;
		int samples;
		int sample_rate;
	};

	//struct casCell
	//{
	//private:
	//	
	//	void FloodOutput(int r,int g,int b)
	//	{
	//		this->color[0] = r;
	//		this->color[1] = g;
	//		this->color[2] = b;

	//		for (int i = 0; i < this->out.size(); i++)
	//			this->out[i]->FloodOutput(r,g,b);
	//	}

	//	void FloodInput(int r, int g, int b)
	//	{
	//		this->color[0] = r;
	//		this->color[1] = g;
	//		this->color[2] = b;

	//		for (int i = 0; i < this->in.size(); i++)
	//			this->in[i]->FloodInput(r,g,b);
	//	}

	//public:
	//	casCell (Cascade* cas) : cas(cas)
	//	{

	//	}

	//	~casCell() {
	//		
	//	}

	//	void setPos(int column, int row)
	//	{
	//		this->row = row;
	//		this->column = column;
	//	}

	//	void Drain()
	//	{
	//		this->color[0] = 64;
	//		this->color[1] = 64;
	//		this->color[2] = 64;
	//	}

	//	void Fire()
	//	{
	//		for (int i = 0; i < this->in.size(); i++)
	//			this->in[i]->dEnsureOutput(this);
	//		for (int i = 0; i < this->out.size(); i++)
	//			this->out[i]->dEnsureInput(this);
	//	}

	//	void Flood()
	//	{

	//		this->color[0] = rand() % 255;
	//		this->color[1] = rand() % 255;
	//		this->color[2] = rand() % 255;

	//		for (int i = 0; i < this->in.size(); i++)
	//			this->in[i]->FloodInput(this->color[0],this->color[1],this->color[2]);
	//		for (int i = 0; i < this->out.size(); i++)
	//			this->out[i]->FloodOutput(this->color[0], this->color[1], this->color[2]);
	//	}

	//	void cEnsureOutput(casCell* cCell)
	//	{
	//		int i = 0;
	//		for (; i < cCell->in.size(); i++)
	//			if (cCell->in[i] == this) break;
	//	

	//		if (i == cCell->in.size()) {
	//			cCell->in.push_back(this);
	//			cCell->cas->appendCas(this->cas);
	//		}
	//			

	//		int j = 0;

	//		for (; j < this->out.size(); j++)
	//			if (this->out[j] == cCell) break;

	//		if (j == this->out.size()) this->out.push_back(cCell);
	//		
	//	}

	//	void dEnsureOutput(casCell* cCell)
	//	{
	//		int i = 0;
	//		for (; i < cCell->in.size(); i++)
	//			if (cCell->in[i] == this) break;


	//		if (i != cCell->in.size()) {
	//			cCell->in.erase(cCell->in.begin() + i);
	//			cCell->cas->popCas(this->cas);
	//		}

	//		int j = 0;

	//		for (; j < this->out.size(); j++)
	//			if (this->out[j] == cCell) break;

	//		if (j != this->out.size()) {
	//			this->out.erase(this->out.begin() + j);
	//		}
	//	}
	//	
	//	void cEnsureInput(casCell* cCell)
	//	{
	//		int i = 0;
	//		for (; i < cCell->out.size(); i++)
	//			if (cCell->out[i] == this) break;


	//		if (i == cCell->out.size()) cCell->out.push_back(this);

	//		int j = 0;

	//		for (; j < this->in.size(); j++)
	//			if (this->in[j] == cCell) break;

	//		if (j == this->in.size()) {
	//			this->in.push_back(cCell);
	//			this->cas->appendCas(cCell->cas);
	//		}
	//		/*else
	//			std::cout << "Ensured Input Connected Already!" << std::endl;*/
	//	}
	//	
	//	void dEnsureInput(casCell* cCell)
	//	{
	//		int i = 0;
	//		for (; i < cCell->out.size(); i++)
	//			if (cCell->out[i] == this) break;


	//		if (i != cCell->out.size()) cCell->out.erase(cCell->out.begin() + i);

	//		int j = 0;

	//		for (; j < this->in.size(); j++)
	//			if (this->in[j] == cCell) break;

	//		if (j != this->in.size()) {
	//			this->in.erase(this->in.begin() + j);
	//			this->cas->popCas(cCell->cas);
	//		}
	//		/*else
	//			std::cout << "Ensured Input Disconnected Already!" << std::endl;*/
	//	}

	//	Cascade* cas = nullptr;
	//	
	//	std::vector<casCell*> in;
	//	std::vector<casCell*> out;

	//	int color[3] = {64,64,64};

	//	int column = -1;
	//	int row = -1;
	//};
}