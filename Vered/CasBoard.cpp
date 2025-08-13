#include "CasBoard.h"
#include "imgui.h"

CasBoard::CasBoard(const char * title):title(title)
{
	
}

CasBoard::~CasBoard()
{

}

void CasBoard::render()
{
	ImGui::Begin(this->title.c_str());

	if (this->ops.size() > 0) {
		if (ImGui::BeginTable("Cascades", this->ops.size(), ImGuiTableFlags_Borders))
		{
			ImGui::TableNextRow();
			for (int column = 0; column < this->ops.size(); column++)
			{
				ImGui::TableSetColumnIndex(column);
				ImGui::Text("Operator %d", column);


				if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
					this->sOp = column;
				}

				if (column == this->sOp)
					ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(0, 255, 0, 32));
			}
			ImGui::EndTable();
		}

		if (ImGui::Button("Advance Operator"))
		{
			bool ifInRange = this->sOp >= 0 && this->sOp < this->ops.size() - 1;
			bool ifNeck = this->sOp == this->ops.size() - 2;

			if (ifInRange)
			{
				Operator* back = this->ops[this->sOp];
				Operator* front = this->ops[this->sOp + 1];
				if (!ifNeck)
				{
					Operator* out = this->ops[this->sOp + 2];

					this->cas->switchOps(back, front, out);
				}
				else
				{
					this->cas->switchOps(back, front, nullptr);
				}

				this->ops[this->sOp] = front;
				this->ops[this->sOp + 1] = back;
				this->sOp++;
			}
		}

		if (ImGui::Button("Recede Operator"))
		{
			bool ifInRange = this->sOp > 0 && this->sOp < this->ops.size();
			bool ifHead = this->sOp == this->ops.size() - 1;

			if (ifInRange)
			{
				Operator* back = this->ops[this->sOp - 1];
				Operator* front = this->ops[this->sOp];
				if (!ifHead)
				{
					Operator* out = this->ops[this->sOp + 1];

					this->cas->switchOps(back, front, out);
				}
				else
				{
					this->cas->switchOps(back, front, nullptr);
				}

				this->ops[this->sOp] = back;
				this->ops[this->sOp - 1] = front;
				this->sOp--;
			}
		}

		std::string chain = "";
		Operator* iter = this->cas->carrier;
		while (iter != nullptr)
		{
		
			chain += std::to_string((unsigned long long)iter) + " ";
			iter = iter->in;
		}
		ImGui::Text(chain.c_str());
	}



	else
	{
		if (this->cas == nullptr) ImGui::Text("No Cascade");
		else ImGui::Text("Cascade Is Empty");
	}

	if (this->cas != nullptr) {
		if (ImGui::Button("Append Operator"))
		{
			this->ops.push_back(new Operator());
			this->cas->Append(this->ops[this->ops.size() - 1]);
		}
	}

	ImGui::End();
}

void CasBoard::refreshOps()
{
	this->ops.clear();
	if (this->cas != nullptr) {
		Operator* iter = this->cas->carrier;
		while (iter != nullptr) {
			this->ops.insert(this->ops.begin(), iter);
			iter = iter->in;
		}
	}
}

void CasBoard::setCas(Cascade* cas)
{
	this->cas = cas;
	refreshOps();
}