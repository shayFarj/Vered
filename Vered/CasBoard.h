#pragma once
#include "Cascade.h"
#include <string>
class CasBoard
{
public:
	CasBoard(const char * name);
	~CasBoard();

	void render();
	void setCas(Cascade* cas);

private:

	Cascade* cas = nullptr;
	std::string title;

	std::vector<Operator*> ops;
	
	void refreshOps();

	int sOp = -1;

};

