#include "casCell.h"



namespace vered {
	casCell::casCell(Cascade* cas) : cas(cas)
	{

	}

	casCell::casCell(const casCell& rhs)
	{
		this->cas = new Cascade(*rhs.cas);
		this->color[0] = rhs.color[0];
		this->color[1] = rhs.color[1];
		this->color[2] = rhs.color[2];

		this->column = rhs.column;
		this->row = rhs.row;
	}

	//casCell& casCell::operator=(const casCell& rhs)
	//{
	//	if (this != &rhs)
	//	{
	//		this->cas = new Cascade(*rhs.cas);

	//		this->color[0] = rhs.color[0];
	//		this->color[1] = rhs.color[1];
	//		this->color[2] = rhs.color[2];

	//		this->column = rhs.column;
	//		this->row = rhs.row;
	//	}
	//	return *this;
	//}

	casCell::~casCell()
	{
	}

	void casCell::setPos(int column, int row)
	{
		this->row = row;
		this->column = column;
	}

	void casCell::Drain()
	{
		this->color[0] = 64;
		this->color[1] = 64;
		this->color[2] = 64;
	}

	void casCell::Fire()
	{
		for (int i = 0; i < this->in.size(); i++)
			this->in[i]->dEnsureOutput(this);
		for (int i = 0; i < this->out.size(); i++)
			this->out[i]->dEnsureInput(this);
	}

	void casCell::Flood()
	{

		this->color[0] = rand() % 255;
		this->color[1] = rand() % 255;
		this->color[2] = rand() % 255;

		for (int i = 0; i < this->in.size(); i++)
			this->in[i]->FloodInput(this->color[0], this->color[1], this->color[2]);
		for (int i = 0; i < this->out.size(); i++)
			this->out[i]->FloodOutput(this->color[0], this->color[1], this->color[2]);
	}

	void casCell::FloodOutput(int r, int g, int b)
	{
		this->color[0] = r;
		this->color[1] = g;
		this->color[2] = b;

		for (int i = 0; i < this->out.size(); i++)
			this->out[i]->FloodOutput(r, g, b);
	}

	void casCell::FloodInput(int r, int g, int b)
	{
		this->color[0] = r;
		this->color[1] = g;
		this->color[2] = b;

		for (int i = 0; i < this->in.size(); i++)
			this->in[i]->FloodInput(r, g, b);
	}

	void casCell::cEnsureOutput(casCell* cCell)
	{
		int i = 0;
		for (; i < cCell->in.size(); i++)
			if (cCell->in[i] == this) break;


		if (i == cCell->in.size()) {
			cCell->in.push_back(this);
			cCell->cas->appendCas(this->cas);
		}


		int j = 0;

		for (; j < this->out.size(); j++)
			if (this->out[j] == cCell) break;

		if (j == this->out.size()) this->out.push_back(cCell);

	}

	void casCell::dEnsureOutput(casCell* cCell)
	{
		int i = 0;
		for (; i < cCell->in.size(); i++)
			if (cCell->in[i] == this) break;


		if (i != cCell->in.size()) {
			cCell->in.erase(cCell->in.begin() + i);
			cCell->cas->popCas(this->cas);
		}

		int j = 0;

		for (; j < this->out.size(); j++)
			if (this->out[j] == cCell) break;

		if (j != this->out.size()) {
			this->out.erase(this->out.begin() + j);
		}
	}

	void casCell::cEnsureInput(casCell* cCell)
	{
		int i = 0;
		for (; i < cCell->out.size(); i++)
			if (cCell->out[i] == this) break;


		if (i == cCell->out.size()) cCell->out.push_back(this);

		int j = 0;

		for (; j < this->in.size(); j++)
			if (this->in[j] == cCell) break;

		if (j == this->in.size()) {
			this->in.push_back(cCell);
			this->cas->appendCas(cCell->cas);
		}
		/*else
			std::cout << "Ensured Input Connected Already!" << std::endl;*/
	}

	void casCell::dEnsureInput(casCell* cCell)
	{
		int i = 0;
		for (; i < cCell->out.size(); i++)
			if (cCell->out[i] == this) break;


		if (i != cCell->out.size()) cCell->out.erase(cCell->out.begin() + i);

		int j = 0;

		for (; j < this->in.size(); j++)
			if (this->in[j] == cCell) break;

		if (j != this->in.size()) {
			this->in.erase(this->in.begin() + j);
			this->cas->popCas(cCell->cas);
		}
		/*else
			std::cout << "Ensured Input Disconnected Already!" << std::endl;*/
	}
}