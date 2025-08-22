#pragma once
#include <string>

class Envelope
{
public:

	Envelope(double a_x,double a_y, double s_x,double s_y,double d_x,double d_y,double r);
	Envelope(Envelope& rhs);
	Envelope();

	Envelope& operator=(Envelope& rhs);

	~Envelope();

	double calc(double time,double release);

	void setAttack(double x, double y);
	void setDecay(double x, double y);
	void setSustain(double x, double y);
	void setRelease(double r);

	double& getA_X();
	double& getA_Y();

	double& getS_X();
	double& getS_Y();

	double& getD_X();
	double& getD_Y();

	double& getR();

	void getData(std::string&content);

private:
	void initAttack(double x, double y);
	void initDecay(double x, double y);
	void initSustain(double x, double y);

	double Interpol(double a_y, double a_x, double b_x, double b_y, double t);
	double a_x =0;
	double a_y=0;

	double s_x=0;
	double s_y=0;
	
	double d_x=0;
	double d_y=0;

	double r=0; 
};

