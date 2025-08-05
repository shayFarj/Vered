#pragma once
#include "Operator.h"
#include "Cascade.h"
#include "Instrument.h"
#include "FourierCalc.h"
#include "SoundSource.h"
#include "KeysInput.h"
#include "LiveMixer.h"

class MainWin
{
public:
	MainWin();
	~MainWin();

	void render();
private:

	LiveMixer mixer;

	Cascade cas;
	Cascade cas2;

	Operator op3;
	Operator op1;
	Operator op2;

	Instrument inst;

	FourierCalc fc_1;
	FourierCalc fc_2;
	FourierCalc fc_3;

	bool showGraph = false;
	bool havePlayed = false;
	bool resetKeyTime = false;

	int bCount = 0;
};

