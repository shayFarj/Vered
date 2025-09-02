#pragma once
#include <vector>
#include "casCell.h"
#include "Instrument.h"
#include <string>
#include "CasBoard.h"
#include "PaStreamer.h"
class InstBoard
{
public:

	InstBoard();
	~InstBoard();

	void render();
	Instrument* getInst();
	

private:
	void deleteLayer(int column);

	void loadInstFile(const char * filepath);
	void saveInstFile(const char * filepath);

	void swapCursors();

	void displayHeader();
	void displayRow(int row);

	bool cellExists(int row, int column);
	void deleteCell(int row, int column);

	void drainCells();

	void addLayer();
	void addToLayer(int column);

	void connectCells(int c1, int r1, int c2, int r2);
	void disconnectCells(int c1, int r1, int c2, int r2);

	bool isNeighbors(int c1, int c2);

	void floodCell(int row, int column);

	void refreshMaxCol();

	std::vector<std::vector<vered::casCell *>> cells;

	int cRowR = -1;
	int cColR = -1;

	int cRowB = -1;
	int cColB = -1;

	int maxCol = 0;

	int sCol = -1;

	std::string filepath = "";
	std::string fileErr = "";



	Instrument * inst;

	CasBoard cBoardR;
	CasBoard cBoardB;
};

