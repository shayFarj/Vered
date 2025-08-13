#include "InstBoard.h"
#include <imgui.h>
#include "structs.h"
#include <iostream>

InstBoard::InstBoard()
{
	this->cells.push_back(std::vector<vered::casCell>());
}
InstBoard::~InstBoard()
{

}

void InstBoard::render()
{
	

	ImGui::Begin("Instrument Board");

	if (ImGui::BeginTable("Cascades", this->cells.size(), ImGuiTableFlags_Borders))
	{
		for (int row = 0; row < this->maxCol + 1; row++)
		{
			if (row == 0)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < this->cells.size(); column++)
				{
					ImGui::TableSetColumnIndex(column);
					if (column == 0)
						ImGui::Text("Output layer");
					else
						ImGui::Text("Layer %d", column);

					if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
						this->sCol = column;
					}

					if(column == this->sCol)
						ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(0, 255, 0, 32));
				}

			}
			else {
				ImGui::TableNextRow();
				for (int column = 0; column < this->cells.size(); column++)
				{
					ImGui::TableSetColumnIndex(column);
					if(this->cells[column].size() > row - 1)
						ImGui::Text("Cascade  %d : %d", row, column);
					else
						ImGui::Text("Empty");

					if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
						if (this->cCol1 == column && this->cRow1 == row - 1 || this->cCol2 == column && this->cRow2 == row - 1)
						{
							int swap = this->cCol2;
							this->cCol2 = this->cCol1;
							this->cCol1 = swap;

							swap = this->cRow2;
							this->cRow2 = this->cRow1;
							this->cRow1 = swap;

							bool ifExistsR = (this->cCol1 < this->cells.size()) && (this->cRow1 < this->cells[cCol1].size());
							bool ifExistsB = (this->cCol2 < this->cells.size()) && (this->cRow2 < this->cells[cCol2].size());
							
							if(ifExistsR)
								this->cBoardR.setCas(this->cells[this->cCol1][this->cRow1].cas);
							else
								this->cBoardR.setCas(nullptr);
							if(ifExistsB)
								this->cBoardB.setCas(this->cells[this->cCol2][this->cRow2].cas);
							else
								this->cBoardB.setCas(nullptr);

						}
						else {
							
							this->cCol1 = column;
							this->cRow1 = row - 1;

							bool ifExistsR = (this->cCol1 < this->cells.size()) && (this->cRow1 < this->cells[cCol1].size());

							if(ifExistsR)
								this->cBoardR.setCas(this->cells[this->cCol1][this->cRow1].cas);
							else
								this->cBoardR.setCas(nullptr);
						}
					
					}
					if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
						if (this->cCol2 == column && this->cRow2 == row - 1 || this->cCol1 == column && this->cRow1 == row - 1)
						{

							int swap = this->cCol2;
							this->cCol2 = this->cCol1;
							this->cCol1 = swap;

							swap = this->cRow2;
							this->cRow2 = this->cRow1;
							this->cRow1 = swap;

							bool ifExistsR = (this->cCol1 < this->cells.size()) && (this->cRow1 < this->cells[cCol1].size());
							bool ifExistsB = (this->cCol2 < this->cells.size()) && (this->cRow2 < this->cells[cCol2].size());

							if(ifExistsB)
								this->cBoardB.setCas(this->cells[this->cCol2][this->cRow2].cas);
							else
								this->cBoardB.setCas(nullptr);

							if(ifExistsR)
								this->cBoardR.setCas(this->cells[this->cCol1][this->cRow1].cas);
							else
								this->cBoardR.setCas(nullptr);
						}
						else {
							this->cCol2 = column;
							this->cRow2 = row - 1;

							bool ifExistsB = (this->cCol2 < this->cells.size()) && (this->cRow2 < this->cells[cCol2].size());
							if (ifExistsB)
								this->cBoardB.setCas(this->cells[this->cCol2][this->cRow2].cas);
							else
								this->cBoardB.setCas(nullptr);
						}
					}

					if(this->cCol1 == column && this-> cRow1 == row - 1)
						ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(255, 0, 0, 128));
					
					if(this->cCol2 == column && this-> cRow2 == row - 1)
						ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(0, 0, 255, 128));
					

				}
			}
		}
		ImGui::EndTable();
	}

	if (ImGui::Button("Add layer"))
	{
		this->cells.push_back(std::vector<vered::casCell>());
	}

	if (ImGui::Button("Add to layer"))
	{
		if (this->sCol != -1) {
			Cascade* cas = new Cascade;
			*cas = Cascade();

			vered::casCell nCell = vered::casCell(cas);

			this->cells[this->sCol].push_back(nCell);

			int size = this->cells[this->sCol].size();
			if (size > this->maxCol)
				this->maxCol = size;
			
		}
		else
		{
			this->errMsg = "";
			this->errMsg += "Select layer first.";
		}
	}

	if (ImGui::Button("Connect Cascades"))
	{
		bool ifSelected = this->cCol1 != -1 && this->cCol2 != -1 && this->cRow1 != -1 && this->cRow2 != -1;
		bool ifNeighbors = abs(this->cCol1 - this->cCol2) == 1;
		bool ifExists = (this->cCol1 < this->cells.size()) && (this->cCol2 < this->cells.size()) && (this->cRow1 < this->cells[cCol1].size()) && (this->cRow2 < this->cells[cCol2].size());
		if (ifExists && ifNeighbors && ifSelected)
		{
			int closeCol = this->cCol1 < this->cCol2 ? this->cCol1 : this->cCol2;
			int closeRow = this->cCol1 < this->cCol2 ? this->cRow1 : this->cRow2;

			int farCol = this->cCol1 > this->cCol2 ? this->cCol1 : this->cCol2;
			int farRow = this->cCol1 > this->cCol2 ? this->cRow1 : this->cRow2;

			this->cells[closeCol][closeRow].cEnsureInput(&this->cells[farCol][farRow]);
		}
	}

	if (ImGui::Button("Disconnect Cascades"))
	{
		if (this->cCol1 != -1 && this->cCol2 != -1 && this->cRow1 != -1 && this->cRow2 != -1)
		{
			bool ifSelected = this->cCol1 != -1 && this->cCol2 != -1 && this->cRow1 != -1 && this->cRow2 != -1;
			bool ifNeighbors = abs(this->cCol1 - this->cCol2) == 1;
			bool ifExists = (this->cCol1 < this->cells.size()) && (this->cCol2 < this->cells.size()) && (this->cRow1 < this->cells[cCol1].size()) && (this->cRow2 < this->cells[cCol2].size());
			if (ifExists && ifNeighbors && ifSelected)
			{
				int closeCol = this->cCol1 < this->cCol2 ? this->cCol1 : this->cCol2;
				int closeRow = this->cCol1 < this->cCol2 ? this->cRow1 : this->cRow2;

				int farCol = this->cCol1 > this->cCol2 ? this->cCol1 : this->cCol2;
				int farRow = this->cCol1 > this->cCol2 ? this->cRow1 : this->cRow2;

				this->cells[closeCol][closeRow].dEnsureInput(&this->cells[farCol][farRow]);
			}
		}
	}

	if (ImGui::Button("Delete red cascade"))
	{
		bool ifSelected = this->cCol1 != -1 && this->cRow1 != -1;
		bool ifExists = (this->cCol1 < this->cells.size()) && (this->cRow1 < this->cells[cCol1].size());

		if (ifExists&&ifSelected)
		{
			this->cells[this->cCol1][this->cRow1].Fire();

			this->cells[this->cCol1].erase(this->cells[this->cCol1].begin() + this->cRow1);
		}

		this->maxCol = this->cells[0].size();
		for (int i = 1; i < this->cells.size(); i++)
			this->maxCol = this->maxCol > this->cells[i].size() ? this->maxCol : this->cells[i].size();
	}

	if (ImGui::Button("Delete blue cascade"))
	{
		bool ifSelected = this->cCol2 != -1 && this->cRow2 != -1;
		bool ifExists = (this->cCol2 < this->cells.size()) && (this->cRow2 < this->cells[cCol2].size());

		if (ifExists)
		{
			this->cells[this->cCol2][this->cRow2].Fire();

			this->cells[this->cCol2].erase(this->cells[this->cCol2].begin() + this->cRow2);
		}

		this->maxCol = this->cells[0].size();
		for (int i = 1; i < this->cells.size(); i++)
			this->maxCol = this->maxCol > this->cells[i].size() ? this->maxCol : this->cells[i].size();
	}

	if(ImGui::Button)

	ImGui::Text(this->errMsg.c_str());

	ImGui::End();

	this->cBoardB.render();
	this->cBoardR.render();
}