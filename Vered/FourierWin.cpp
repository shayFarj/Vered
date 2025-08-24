#include "FourierWin.h"
#include "imgui.h"
#include "implot.h"

FourierWin::FourierWin(Instrument* inst):calc(1000,0,1000)
{
	
	this->inst = inst;
}

FourierWin::~FourierWin()
{

}

void FourierWin::render()
{
	ImGui::Begin("Fourier");
	if (!this->calc.onWork())
	{
		ImGui::InputDouble("Playing Frequency", &this->freq);
		ImGui::InputDouble("Starting Frequency",&this->freq_start);
		ImGui::InputDouble("Difference Between Components",&this->freq_step);
		ImGui::InputInt("Number Of Components",&this->freq_steps);

		ImGui::InputDouble("Time Base", &this->time_base);
		ImGui::InputDouble("Release Time Base", &this->r_time_base);
		ImGui::InputDouble("Time Step", &this->time_step);
		ImGui::InputInt("Threads", &this->threads);

		if (ImGui::Button("Begin Calculation"))
		{
			if (this->freq_steps % this->threads == 0) {
				this->calc.syncAndReset(this->freq_steps, this->freq_start, this->freq_step);
				this->calc.init(*this->inst, this->freq, this->time_steps, this->time_step, this->time_base, this->r_time_base, this->threads);
			}
		}
		if (this->freq_steps % this->threads != 0)
			ImGui::Text("Number of threads should devide number of components with zero reminder");
	}
	if (this->calc.isReady())
	{

		ImGui::Text("It's ready!");
		if (ImGui::Button("Display Chart"))
		{
			this->showGraph = !this->showGraph;
		}
		if (this->showGraph)
		{
			if (ImPlot::BeginPlot("Fourier Chart")) {
				ImPlot::PlotLine("Fourier", this->calc.refRange(), this->calc.refData(), this->calc.getDataLen());
				ImPlot::EndPlot();
			}
		}
	}
	ImGui::End();
}

