#pragma once
#include <vector>
#include "structs.h"
#include "Instrument.h"
#include <string>

class InstBoard
{
public:

	InstBoard();
	~InstBoard();

	void render();

	

private:
	std::vector<std::vector<vered::casCell>> cells;

	void newCas(int column);

	int cRow1 = -1;
	int cCol1 = -1;

	int cRow2 = -1;
	int cCol2 = -1;

	int maxCol = 0;

	int sCol = -1;

	std::string errMsg ="";

	Instrument inst = Instrument();
};

