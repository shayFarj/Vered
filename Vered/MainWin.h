#pragma once
#include "Operator.h"
#include "Cascade.h"
#include "Instrument.h"
#include "FourierCalc.h"
#include "SoundSource.h"
#include "KeysInput.h"
#include "LiveMixer.h"
#include "InstBoard.h"
#include "FourierWin.h"

class MainWin
{
public:
	MainWin();
	~MainWin();

	void render();
	void shutdown();
private:
	InstBoard iBoard;

	LiveMixer mixer;

	Cascade cas;

	Cascade cas2;
	Cascade cas3;

	Operator op4;
	Operator op5;
	Operator op6;


	Operator op3;
	Operator op1;
	Operator op2;

	Instrument inst;

	Instrument inst1;
	Instrument inst2;

	FourierCalc fc_1;
	FourierCalc fc_2;
	FourierCalc fc_3;
	
	FourierWin fWin;

	bool showGraph = false;
	bool havePlayed = false;
	bool resetKeyTime = false;

	int bCount = 0;
};

