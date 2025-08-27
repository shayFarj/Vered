#include "casCell.h"
#include <random>


namespace vered {
	casCell::casCell(Cascade* cas, int row, int column) :cas(cas), row(row),column(column)
	{

	}

	casCell::casCell(const casCell& rhs)
	{
		this->cas = new Cascade(*rhs.cas);
		this->row = rhs.row;
		this->column = rhs.column;

		this->color[0] = rhs.color[0];
		this->color[1] = rhs.color[1];
		this->color[2] = rhs.color[2];
	}

	casCell& casCell::operator=(const casCell& rhs)
	{
		if (this != &rhs)
		{
			this->cas = new Cascade(*rhs.cas);
			this->row = rhs.row;
			this->column = rhs.column;

			this->color[0] = rhs.color[0];
			this->color[1] = rhs.color[1];
			this->color[2] = rhs.color[2];
		}
		return *this;
	}

	casCell::~casCell()
	{
		delete this->cas;
	}

	void casCell::Fire()
	{
		for (casCell* cCell : this->in)
			cCell->dMyOutput(this);

		for (casCell* cCell : this->out)
			cCell->dMyInput(this);

	}

	void casCell::Flood()
	{
		this->color[0] = std::rand() % 255;
		this->color[1] = std::rand() % 255;
		this->color[2] = std::rand() % 255;

		for (casCell* cCell : this->in)
			cCell->Flood(this->color[0], this->color[1], this->color[2], this);
		for (casCell* cCell : this->out)
			cCell->Flood(this->color[0], this->color[1], this->color[2], this);

	}

	void casCell::Flood(int r, int g, int b, casCell* irregular)
	{
		this->color[0] = r;
		this->color[1] = g;
		this->color[2] = b;

		for (casCell* cCell : this->in)
			if(cCell != irregular)
				cCell->Flood(this->color[0], this->color[1], this->color[2], this);
		for (casCell* cCell : this->out)
			if (cCell != irregular)
				cCell->Flood(this->color[0], this->color[1], this->color[2], this);

	}

	void casCell::setPos(int row, int column)
	{
		this->row = row;
		this->column = column;
	}

	int casCell::findInput(casCell* cCell)
	{
		int i = 0;
		for (; i < this->in.size() && this->in[i] != cCell; i++);
		return i == this->in.size() ? -1 : i;
	}

	int casCell::findOutput(casCell* cCell)
	{
		int i = 0;
		for (; i < this->out.size() && this->out[i] != cCell; i++);
		return i == this->out.size() ? -1 : i;
	}

	void casCell::dMyInput(casCell* cCell)
	{
		if (cCell != nullptr && cCell != this && cCell->cas != this->cas) {
			int oIndex = cCell->findOutput(this);
			if (oIndex != -1) {
				cCell->out.erase(cCell->out.begin() + oIndex);
			}

			int iIndex = this->findInput(cCell);
			if (iIndex != -1) {
				this->in.erase(this->in.begin() + iIndex);
				this->cas->popCas(cCell->cas);
			}
		}
	}

	void casCell::dMyOutput(casCell* cCell)
	{
		if (cCell != nullptr && cCell != this && cCell->cas != this->cas) {
			int oIndex = this->findOutput(cCell);
			if (oIndex != -1) {
				this->out.erase(this->out.begin() + oIndex);
			}

			int iIndex = cCell->findInput(this);
			if (iIndex != -1) {
				cCell->in.erase(cCell->in.begin() + iIndex);
				cCell->cas->popCas(this->cas);
			}
		}
	}
	void casCell::cMyInput(casCell* cCell)
	{
		if (cCell != nullptr && cCell != this && cCell->cas != this->cas) {
			int oIndex = cCell->findOutput(this);
			if (oIndex == -1)
				cCell->out.push_back(this);

			int iIndex = this->findInput(cCell);
			if (iIndex == -1) {
				this->in.push_back(cCell);
				this->cas->appendCas(cCell->cas);
			}
		}
	}

	void casCell::Drain()
	{
		this->color[0] = 64;
		this->color[1] = 64;
		this->color[2] = 64;
	}

	void casCell::cMyOutput(casCell* cCell)
	{
		if (cCell != nullptr && cCell != this && cCell->cas != this->cas) {
			int oIndex = this->findOutput(cCell);
			if (oIndex == -1)
				this->out.push_back(cCell);

			int iIndex = cCell->findInput(this);
			if (iIndex == -1) {
				cCell->in.push_back(this);
				cCell->cas->appendCas(this->cas);
			}
		}
	}

	bool casCell::isAfter(casCell* cCell)
	{
		if (cCell != nullptr)
			return this->column - cCell->column == -1;

		return false;
	}

	bool casCell::isBefore(casCell* cCell)
	{
		if (cCell != nullptr)
			return this->column - cCell->column == 1;

		return false;
	}

}