#include "CasBoard.h"
#include "imgui.h"
#include "ImPlot.h"
#include "PaStreamer.h"

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

		

		bool ifOpExists = this->sOp >= 0 && this->sOp < this->ops.size();
		if (ifOpExists)
		{
			ImGui::Text("Selected Operator Settings:");
			ImGui::InputInt("MULT", &(this->ops[this->sOp]->mult));
			ImGui::InputDouble("MOD INDEX", &(this->ops[this->sOp]->mod_index));
			ImGui::InputInt("SELF FEEDBACK", &(this->ops[this->sOp]->fb));
			ImGui::InputDouble("FEEDBACK MULT", &(this->ops[this->sOp]->fb_mult));
			ImGui::InputInt("DETUNE (128th of a semitone)", &(this->ops[this->sOp]->detune));

			
			Envelope* env = &this->ops[this->sOp]->env;
			double a_x = env->getA_X();
			double a_y = env->getA_Y();

			double s_x = env->getS_X();
			double s_y = env->getS_Y();

			double d_x = env->getD_X();
			double d_y = env->getD_Y();

			double r = env->getR();

			if (ImPlot::BeginPlot("Envelope"))
			{
				

				

				ImPlot::PlotScatter("Attack", &a_x, &a_y, 1);
				ImPlot::PlotScatter("Sustain", &s_x, &s_y, 1);
				ImPlot::PlotScatter("Decay", &d_x, &d_y, 1);
				
				if (r != 0)
				{
					double r_x = d_x + (d_y / r);


					double r_line_y[2] = { d_y,0 };

					double r_line_x[2] = { d_x,r_x };


					ImPlot::PlotLine("Release", r_line_x, r_line_y, 2);
				}
				ImPlot::EndPlot();
			}

			ImGui::InputDouble("a_x", &a_x);
			ImGui::InputDouble("a_y", &a_y);
			ImGui::InputDouble("d_x", &d_x);
			ImGui::InputDouble("d_y", &d_y);
			ImGui::InputDouble("s_x", &s_x);
			ImGui::InputDouble("s_y", &s_y);
			ImGui::InputDouble("r", &r);

			env->setAttack(a_x, a_y);
			env->setDecay(d_x, d_y);
			env->setSustain(s_x, s_y);
			env->setRelease(r);
		}

		
	}



	else
	{
		if (this->cas == nullptr) ImGui::Text("No Cascade");
		else ImGui::Text("Cascade Is Empty");
	}

	if (this->cas != nullptr) {
		std::string chain = "";
		Operator* iter = this->cas->carrier;
		while (iter != nullptr)
		{

			chain += std::to_string((unsigned long long)iter) + " ";
			iter = iter->in;
		}
		ImGui::Text(chain.c_str());

		if (ImGui::Button("Append Operator"))
		{
			this->ops.push_back(new Operator());
			this->cas->Append(this->ops[this->ops.size() - 1]);
		}
		if (ImGui::Button("Delete Operator"))
		{
			if (this->sOp >= 0 && this->sOp < this->ops.size())
			{
				PaStreamer::pauseStream();
				
				if (this->sOp != this->ops.size() - 1)
					this->cas->deleteOp(this->ops[this->sOp], this->ops[this->sOp + 1]);
				else
					this->cas->deleteOp(this->ops[this->sOp], nullptr);

				this->ops.erase(this->ops.begin() + this->sOp);

				this->sOp--;

				PaStreamer::unpauseStream();
			}
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