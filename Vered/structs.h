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
	public:
		casCell (Cascade* cas) : cas(cas)
		{

		}

		void cInput(casCell* cCell)
		{
			bool valid = true;
			for (int i = 0; i < this->in.size(); i++)
			{
				valid = this->in[i] != cCell;
				if (!valid) break;
			}

			if (!valid || cCell == nullptr || cCell == this) {
				std::cout << "invalid!" << std::endl;
				return;
			}

			this->in.push_back(cCell);

			this->cas->appendCas(cCell->cas);
			std::cout << "Connected ! " << std::endl;
		}

		void dInput(casCell* cCell) {
			int i = 0;
			for (i = 0; i < this->in.size() && this->in[i] != cCell; i++);

			if (i != this->in.size())
			{
				std::cout << "Disconnected ! " << std::endl;
				this->in.erase(this->in.begin() + i);
			}
			else
			{
				std::cout << "Not connected already ! " << std::endl;
			}
		}
		

		Cascade* cas = nullptr;
		
		std::vector<casCell*> in;
		std::vector<casCell*> out;

		char color[3] = {0,0,0};

		int column = -1;
		int row = -1;
	};
}