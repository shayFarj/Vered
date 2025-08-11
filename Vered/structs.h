#pragma once
#include "Cascade.h"
#include <iostream>
namespace vered
{
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

	struct casCell
	{
	private:


	public:
		casCell (Cascade* cas) : cas(cas)
		{

		}

		void Fire()
		{
			for (int i = 0; i < this->in.size(); i++)
				this->in[i]->dEnsureOutput(this);
			for (int i = 0; i < this->out.size(); i++)
				this->out[i]->dEnsureInput(this);
		}

		void cEnsureOutput(casCell* cCell)
		{
			int i = 0;
			for (; i < cCell->in.size(); i++)
				if (cCell->in[i] == this) break;
		

			if (i == cCell->in.size()) {
				cCell->in.push_back(this);
				cCell->cas->appendCas(this->cas);
			}
				

			int j = 0;

			for (; j < this->out.size(); j++)
				if (this->out[j] == cCell) break;

			if (j == this->out.size()) this->out.push_back(cCell);
			
		}

		void dEnsureOutput(casCell* cCell)
		{
			int i = 0;
			for (; i < cCell->in.size(); i++)
				if (cCell->in[i] == this) break;


			if (i != cCell->in.size()) {
				cCell->in.erase(cCell->in.begin() + i);
				cCell->cas->popCas(this->cas);
			}

			int j = 0;

			for (; j < this->out.size(); j++)
				if (this->out[j] == cCell) break;

			if (j != this->out.size()) {
				this->out.erase(this->out.begin() + j);
			}
		}
		
		void cEnsureInput(casCell* cCell)
		{
			int i = 0;
			for (; i < cCell->out.size(); i++)
				if (cCell->out[i] == this) break;


			if (i == cCell->out.size()) cCell->out.push_back(this);

			int j = 0;

			for (; j < this->in.size(); j++)
				if (this->in[j] == cCell) break;

			if (j == this->in.size()) {
				this->in.push_back(cCell);
				this->cas->appendCas(cCell->cas);
			}
			else
				std::cout << "Ensured Input Connected Already!" << std::endl;
		}
		
		void dEnsureInput(casCell* cCell)
		{
			int i = 0;
			for (; i < cCell->out.size(); i++)
				if (cCell->out[i] == this) break;


			if (i != cCell->out.size()) cCell->out.erase(cCell->out.begin() + i);

			int j = 0;

			for (; j < this->in.size(); j++)
				if (this->in[j] == cCell) break;

			if (j != this->in.size()) {
				this->in.erase(this->in.begin() + j);
				this->cas->popCas(cCell->cas);
			}
			else
				std::cout << "Ensured Input Disconnected Already!" << std::endl;
		}

		Cascade* cas = nullptr;
		
		std::vector<casCell*> in;
		std::vector<casCell*> out;

		char color[3] = {0,0,0};

		int column = -1;
		int row = -1;
	};
}