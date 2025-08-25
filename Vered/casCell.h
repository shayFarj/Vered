#pragma once
#include "Cascade.h"
namespace vered {
	class casCell
	{
	public:

		casCell(Cascade* cas);
		//casCell(const casCell& rhs);
		//casCell& operator=(const casCell& rhs);
		~casCell();

		void setPos(int column, int row);

		void Drain();

		void Fire();

		void Flood();

		void cEnsureOutput(casCell* cCell);
		void cEnsureInput(casCell* cCell);

		void dEnsureOutput(casCell* cCell);
		void dEnsureInput(casCell* cCell);

		std::vector<casCell*> in;
		std::vector<casCell*> out;

		Cascade* cas;

		int color[3] = { 64,64,64 };

		int column = -1;
		int row = -1;
	private:
		void FloodOutput(int r, int g, int b);
		void FloodInput(int r, int g, int b);
	};

}