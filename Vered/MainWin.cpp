#pragma once
#include "MainWin.h"
#include "imgui.h"
#include "implot.h"
#include "Cascade.h"
#include "Operator.h"
#include <atomic>
#include "FourierCalc.h"
#include <iostream>
#include "string"
#include "PaStreamer.h"

MainWin::MainWin(): fc_1(4000,0,4000), fc_2(4000, 0, 4000), fc_3(4000, 0, 4000),mixer(nullptr)
{
	// modulation index 0.001 correlates to 40 total level in deflemask
	Envelope e1(0,1,2,1,3,1,0);
	Envelope e2(0, 1, 1.5, 1, 2, 0, 0.5);
	
	this->op1 = Operator(1, 0.001, e1);
	this->op2 = Operator(2, 0.0005, e1);
	this->op3 = Operator(1, 1, e2);

	this->op4 = Operator(1, 0.001, e1);
	this->op5 = Operator(2, 0.0005, e1);
	this->op6 = Operator(1, 1, e2);
	


	this->cas = Cascade();
	this->cas.Append(&op2);
	//this->cas.Append(&op2);
	this->cas.Append(&op3);
	
	this->inst1 = Instrument();

	this->inst1.appendCas(&this->cas);

	this->mixer.setInstrument(&inst1);


	PaStreamer::init(inst1);

}


MainWin::~MainWin()
{

}

void MainWin::render()
{

	

	//if (!havePlayed)
	//{
	//	this->source.playBuffer(this->inst.getBuffer16Stereo(440,0,5,48000));
	//	this->havePlayed = true;
	//}
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.38, 0.0, 0.0, 1));
	ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.38, 0.0, 0.0, 0.5));

	ImGui::Begin("Test");


	const double(&press)[24] = KeysInput::getKeysPress();
	
	std::string msg = "Press : ";
	for (int i = 0; i < 24; i++)
	{
		msg += " ";
		msg += std::to_string(press[i]);
	}

	const double(&release)[24] = KeysInput::getKeysRelease();
	std::string msg2 = "Released : ";
	for (int i = 0; i < 24; i++)
	{
		msg2 += " ";
		msg2 += std::to_string(release[i]);
	}

	ImGui::Text(msg.c_str());
	ImGui::Text(msg2.c_str());

	
	//if (!fc_1.onWork() && !fc_2.onWork() /*&& !fc_3.onWork()*/)
	//{
	//	if (ImGui::Button("begin"))
	//	{
	//		fc_1.init(this->inst1,880,1000,0.0001,0,10);
	//		fc_2.init(this->inst2, 880, 1000, 0.0001, 0, 10);
	//		//fc_3.init(this->inst, 880, 1000, 0.0001, 0, 10);
	//	}
	//}
	//if (fc_1.isReady() && fc_2.isReady() /*&& fc_3.isReady()*/)
	//{

	//	ImGui::Text("It's ready!");
	//	if (ImGui::Button("Display Chart"))
	//	{
	//		this->showGraph = true;
	//	}
	//	if (this->showGraph)
	//	{
	//		if (ImPlot::BeginPlot("Fourier")) {
	//			ImPlot::PlotLine("Instrument1", fc_1.getRange(), fc_1.copyData(), fc_1.getDataLen());
	//			ImPlot::PlotLine("Instrument2", fc_2.getRange(), fc_2.copyData(), fc_2.getDataLen());
	//			//ImPlot::PlotLine("Fourier_880", fc_3.getRange(), fc_3.copyData(), fc_3.getDataLen());
	//			ImPlot::EndPlot();
	//		}
	//	}
	//}

	//KeysInput::update();


	//
	
	vered::bufferDouble* pBuffer = new vered::bufferDouble(nullptr,0,0);
	*pBuffer = this->mixer.getDemoBufferMono(48000);//this->inst.getBuffer16Stereo(55, 0, 1, 48000);

	//double* wav = this->mixer.getDemoArrayMono(48000);

	double* x_range = new double[pBuffer->samples];
	double* wav = new double[pBuffer->samples];


	for (int i = 0; i < pBuffer->samples; i++) {
		x_range[i] = i;
		wav[i] = (double)pBuffer->buffer[i];
	}

	if (ImGui::BeginTable("table1",5, ImGuiTableFlags_Borders))
	{
		for (int row = 0; row < 4; row++)
		{
			ImGui::TableNextRow();
			for (int column = 0; column < 3; column++)
			{
				ImGui::TableSetColumnIndex(column);
				ImGui::Text("Row %d Column %d", row, column);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
					ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(255, 0, 0, 255));
					
				}
				
			}
		}
		ImGui::EndTable();
	}

	if (ImPlot::BeginPlot("wave"))
	{
		ImPlot::PlotLine("ee", x_range, wav, pBuffer->samples);
		ImPlot::EndPlot();
		//this->source.queueBuffer(pBuffer);
		//this->source.stream();
	}
	

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	
	ImGui::End();
}