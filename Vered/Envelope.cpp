#include "Envelope.h"
#include <iostream>
Envelope::Envelope(double a_x, double a_y, double s_x, double s_y, double d_x, double d_y, double r)
{
	this->initAttack(a_x, a_y);
	this->initSustain(s_x, s_y);
	this->setDecay(d_x, d_y);
	this->setRelease(r);
}

Envelope::~Envelope()
{

}

double Envelope::calc(double time,double release)
{
	double amp = 0;

	if (time <= a_x)
	{
		amp = this->Interpol(0, 0, this->a_x, this->a_y, time);
	}
	if (time > a_x && time <= s_x)
	{
		amp = this->Interpol(this->a_x, this->a_y,this->s_x,this->s_y, time);
	}
	if (time > s_x && time <= d_x)
	{
		amp = this->Interpol(this->s_x, this->s_y, this->d_x, this->d_y, time);
	}
	if (time > d_x)
		amp = d_y;
	if (release > 0)
		return amp - release*this->r < 0 ? 0 : amp - release * this->r;
	else
		return amp;

}

void Envelope::initAttack(double x, double y)
{
	x = x > 0 ? x : 0;

	if (y > 1) y = 1;
	if (y < 0) y = 0;

	this->a_x = x;
	this->a_y = y;
}

void Envelope::initSustain(double x, double y)
{
	x = x > this->a_x ? x : this->a_x;

	if (y > 1) y = 1;
	if (y < 0) y = 0;

	this->s_x = x;
	this->s_y = y;
}



void Envelope::setAttack(double x,double y)
{
	x = x > 0 ? x : 0;
	x = x < this->s_x ? x : this->s_x;

	if (y > 1) y = 1;
	if (y < 0) y = 0;

	this->a_x = x;
	this->a_y = y;
}

void Envelope::setSustain(double x, double y)
{
	x = x > this->a_x ? x : this->a_x;
	x = x < this->d_x ? x : this->d_x;

	if (y > 1) y = 1;
	if (y < 0) y = 0;

	this->s_x = x;
	this->s_y = y;
}

void Envelope::setDecay(double x, double y)
{
	x = x > this->s_x ? x : this->s_x;

	if (y > s_y) y = s_y;
	if (y < 0) y = 0;

	this->d_x = x;
	this->d_y = y;
}

void Envelope::setRelease(double r)
{
	this->r = r * (r < 0 ? -1 : 1);
}

double Envelope::Interpol(double a_x,double a_y,double b_x,double b_y,double t)
{
	if (a_x != b_x)
		return a_y + ((a_y - b_y) / (a_x - b_x)) * (t - a_x);
	else
		return b_y;
}

