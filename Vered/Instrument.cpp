#include "Instrument.h"
#include "structs.h"

Instrument::Instrument(): cas(0)
{

}

Instrument::~Instrument()
{

}

void Instrument::setCas(std::vector<Cascade*> cas)
{
	this->cas = cas;
}

void Instrument::appendCas(Cascade* cas)
{
	this->cas.insert(this->cas.end(), cas);
}

void Instrument::eraseCas(int index)
{
	Cascade * rCas = this->cas[index];
	
	this->cas.erase(this->cas.begin() + index);
}

void Instrument::eraseCas(Cascade* cas)
{
	for (int i = 0; i < this->cas.size(); i++)
	{
		if (this->cas[i] == cas)
			this->cas.erase(this->cas.begin() + i);
	}
}

void Instrument::clear()
{
	this->cas.clear();
}


double Instrument::Output(double freq, double time,double release)
{
	double output = 0;
	for (Cascade* c : this->cas)
	{
		if (c != nullptr)
			output += c->modulated(freq, time,release);
	}

	return output;
}

vered::buffer16 Instrument::getBuffer16Stereo(double freq, double base_time,double end_time,int sample_rate)
{
	int samples = (int)(floor(abs(end_time - base_time) * sample_rate)) * 2;
	double delta = 1.0 / sample_rate;
	short* buffer = new short[samples];

	for (int i = 0; i < samples; i += 2)
	{
		buffer[i] = (short)(floor(pow(2, 14) * this->Output(freq, base_time + i * delta,0)));
		buffer[i + 1] = (short)(floor(pow(2, 14) * this->Output(freq, base_time + i * delta, 0)));
	}
	
	vered::buffer16 b = vered::buffer16(buffer,samples,sample_rate);



	return b;
}

short* Instrument::getArray16Stereo(double freq, double base_time,double r_time, int samples, int sample_rate,bool onRelease)
{
	double delta = 1.0 / sample_rate;
	short* buffer = new short[samples];

	if (!onRelease) {
		for (int i = 0; i < samples; i += 2)
		{
			buffer[i] = (short)(floor(pow(2, 11) * this->Output(freq, base_time + i * delta, r_time)));
			buffer[i + 1] = (short)(floor(pow(2, 11) * this->Output(freq, base_time + i * delta, r_time)));
		}
	}
	else
	{
		for (int i = 0; i < samples; i += 2)
		{
			buffer[i] = (short)(floor(pow(2,11) * this->Output(freq, base_time, r_time + i * delta)));
			buffer[i + 1] = (short)(floor(pow(2, 11) * this->Output(freq, base_time, r_time + i * delta)));
		}
	}



	return buffer;
}

