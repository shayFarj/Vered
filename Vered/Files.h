#pragma once
#include "Instrument.h"
#include "structs.h"
#include <vector>
static class Files
{
public:

	Files();
	~Files();

	static void saveTable(std::vector<std::vector<vered::casCell>> table,const char* filepath);
	static std::vector<std::vector<vered::casCell>> loadTable(const char* filepath);

private:
	
	//static void saveInst(Instrument inst);
	//static Instrument* loadInst(const char* filepath);

	
};

