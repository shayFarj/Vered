#pragma once
#include "Cascade.h"
namespace vered {
	class casCell
	{
	public:
		casCell(Cascade * cas,int row,int column);
		casCell(const casCell& rhs);
		casCell& operator=(const casCell& rhs);
		~casCell();

		void Flood();
		void Fire();

		void setPos(int row, int column);

		void dMyOutput(casCell* cCell);
		void dMyInput(casCell* cCell);

		void cMyOutput(casCell* cCell);
		void cMyInput(casCell* cCell);

		
		int row;
		int column;

		Cascade* cas;

		std::vector<casCell*> in;
		std::vector<casCell*> out;

		int color[3] = { 64,64,64 };

		bool isAfter(casCell* cCell);
		bool isBefore(casCell* cCell);
	private:
		

		int findInput(casCell* cCell);
		int findOutput(casCell* cCell);

		void Flood(int r, int g, int b,casCell * irregular);
	};

}