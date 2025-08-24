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
	
	FourierWin fWin;

	bool havePlayed = false;
	bool resetKeyTime = false;

	int bCount = 0;
};

