#pragma once
#include "Cascade.h"

class CasBoard
{
public:

private:

	Cascade* cas;

	std::vector<Operator*> ops;
	
	void refreshOps();

};

