#include "InstBoard.h"
#include <imgui.h>
#include "imgui_stdlib.h"
#include "casCell.h"
#include <iostream>
#include "Files.h";
#include <fstream>

InstBoard::InstBoard():cBoardB("Blue Cascade"),cBoardR("Red Cascade"),inst(new Instrument())
{

}
InstBoard::~InstBoard()
{
	
}

void InstBoard::render()
{
	ImGui::Begin("Instrument Board");
	if (this->cells.size() > 0) {
		if (ImGui::BeginTable("Cascades", this->cells.size())) {

			this->displayHeader();

			for (int i = 0; i < this->maxCol; i++)
				this->displayRow(i);

			ImGui::EndTable();
		}
	}

	if (ImGui::Button("Add Layer"))
		this->addLayer();

	if (ImGui::Button("Add To Layer"))
		this->addToLayer(this->sCol);

	if (ImGui::Button("Delete Blue Cascade"))
		this->deleteCell(this->cRowB, this->cColB);

	if (ImGui::Button("Delete Red Cascade"))
		this->deleteCell(this->cRowR, this->cColR);
	
	if (ImGui::Button("Connect Cascades"))
		this->connectCells(this->cColR, this->cRowR, this->cColB, this->cRowB);

	if (ImGui::Button("Disconnect Cascades"))
		this->disconnectCells(this->cColR, this->cRowR, this->cColB, this->cRowB);

	if (ImGui::Button("Flood Red Cascade"))
		this->floodCell(this->cRowR, this->cColR);

	if (ImGui::Button("Flood Blue Cascade"))
		this->floodCell(this->cRowB, this->cColB);



	ImGui::End();

	this->cBoardB.render();
	this->cBoardR.render();
}

Instrument* InstBoard::getInst()
{
	return this->inst;
}

void InstBoard::displayRow(int row)
{
	ImGui::TableNextRow();
	for (int column = 0; column < this->cells.size(); column++)
	{
		ImGui::TableSetColumnIndex(column);
		if (row < this->cells[column].size()) {
			ImGui::Text("Cascade %d : %d", this->cells[column][row]->column, this->cells[column][row]->row);

			int r = this->cells[column][row]->color[0];
			int g = this->cells[column][row]->color[1];
			int b = this->cells[column][row]->color[2];

			ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(r, g, b, 64));

			if(ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				bool clickingOnExisted = (column == this->cColB && row == this->cRowB) || (column == this->cColR && row == this->cRowR);

				if (clickingOnExisted) {
					this->swapCursors();

				}
				else {
					this->cColR = column;
					this->cRowR = row;
					this->cBoardR.setCas(this->cells[column][row]->cas);
				}

			}
			else {

				if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				{
					bool clickingOnExisted = (column == this->cColB && row == this->cRowB) || (column == this->cColR && row == this->cRowR);

					if (clickingOnExisted) {
						this->swapCursors();
						
					}
					else {
						this->cColB = column;
						this->cRowB = row;
						this->cBoardB.setCas(this->cells[column][row]->cas);
					}
				}
			}

			if(column == this->cColB && row == this->cRowB)
				ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(0,0, 255, 128));

			if(column == this->cColR && row == this->cRowR)
				ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(255,0, 0, 128));


		}
		else
			ImGui::Text("Empty");
	}
}

void InstBoard::swapCursors()
{
	this->cColB += this->cColR;
	this->cColR = this->cColB - this->cColR;
	this->cColB -= this->cColR;

	this->cRowB += this->cRowR;
	this->cRowR = this->cRowB - this->cRowR;
	this->cRowB -= this->cRowR;

	if (this->cellExists(this->cRowB, this->cColB))
	{
		this->cBoardB.setCas(this->cells[this->cColB][this->cRowB]->cas);
	}
	else
	{
		this->cBoardB.setCas(nullptr);
	}

	if (this->cellExists(this->cRowR, this->cColR))
	{
		this->cBoardR.setCas(this->cells[this->cColR][this->cRowR]->cas);
	}
	else
	{
		this->cBoardR.setCas(nullptr);
	}
}

void InstBoard::displayHeader()
{
	ImGui::TableNextRow();
	for (int column = 0; column < this->cells.size(); column++)
	{
		ImGui::TableSetColumnIndex(column);
		if (column == 0)
			ImGui::Text("Output Layer");
		else
			ImGui::Text("Layer %d", column);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			this->sCol = column;
		}

		if (this->sCol == column)
			ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(0, 255, 0, 32));
			
	}
}

bool InstBoard::cellExists(int row, int column)
{
	return column > -1 && row > -1 && column < this->cells.size() && row < this->cells[column].size();
}

bool InstBoard::isNeighbors(int c1, int c2)
{
	return abs(c1 - c2) == 1;
}

void InstBoard::addLayer()
{
	this->cells.push_back(std::vector <vered::casCell*>());
}

void InstBoard::disconnectCells(int c1, int r1, int c2, int r2)
{
	if (this->cellExists(r1,c1) && this->cellExists(r2, c2) && this->isNeighbors(c1, c2))
	{
		int closerC = c1 > c2 ? c2 : c1;
		int farC = c1 > c2 ? c1 : c2;

		int closeR = c1 > c2 ? r2 : r1;
		int farR = c1 > c2 ? r1 : r2;

		if (this->cells[closerC][closeR]->isAfter(this->cells[farC][farR])) {
			PaStreamer::pauseStream();
			this->cells[closerC][closeR]->dMyInput(this->cells[farC][farR]);
			PaStreamer::unpauseStream();
		}
	}
}

void InstBoard::connectCells(int c1, int r1, int c2, int r2)
{
	if (this->cellExists(r1, c1) && this->cellExists(r2, c2) && this->isNeighbors(c1, c2))
	{
		int closerC = c1 > c2 ? c2 : c1;
		int farC = c1 > c2 ? c1 : c2;

		int closeR = c1 > c2 ? r2 : r1;
		int farR = c1 > c2 ? r1 : r2;

		if (this->cells[closerC][closeR]->isAfter(this->cells[farC][farR])) {
			PaStreamer::pauseStream();
			this->cells[closerC][closeR]->cMyInput(this->cells[farC][farR]);
			PaStreamer::unpauseStream();
		}
	}
}

void InstBoard::refreshMaxCol()
{
	if (this->cells.size() >= 1) {
		this->maxCol = this->cells[0].size();
		for (int i = 1; i < this->cells.size(); i++)
			this->maxCol = this->maxCol < this->cells[i].size() ? this->cells[i].size() : this->maxCol;
	}
}

void InstBoard::addToLayer(int column)
{
	if (column < this->cells.size() && column > -1)
	{
		int row = this->cells[column].size();
		this->cells[column].push_back(new vered::casCell(new Cascade(), row, column));

		
		if (column == 0) {
			PaStreamer::pauseStream();
			this->inst->appendCas(this->cells[column][row]->cas);
			PaStreamer::unpauseStream();
		}
		
		if (this->cells[column].size() > this->maxCol)
			this->maxCol = this->cells[column].size();
	}
}

void InstBoard::floodCell(int row, int column)
{
	if (this->cellExists(row, column))
		this->cells[column][row]->Flood();
}

void InstBoard::deleteCell(int row, int column)
{
	if (this->cellExists(row, column))
	{
		PaStreamer::pauseStream();

		if (column == 0)
			this->inst->eraseCas(this->cells[column][row]->cas);

		this->cells[column][row]->Fire();

		if (this->cRowB == row && this->cColB == column)
		{
			this->cBoardB.setCas(nullptr);
			this->cRowB = -1;
			this->cColB = -1;
		}
		if (this->cRowR == row && this->cColR == column)
		{
			this->cBoardR.setCas(nullptr);
			this->cRowR = -1;
			this->cColR = -1;
		}

		delete this->cells[column][row];

		this->cells[column].erase(this->cells[column].begin() + row);

		
		for (int i = row; i < this->cells[column].size(); i++)
			this->cells[column][i]->row -= 1;

		PaStreamer::unpauseStream();
		this->refreshMaxCol();
	}
}
