#pragma once
#include "MainWin.h"
#include "imgui.h"
#include "implot.h"
#include "Cascade.h"
#include "Operator.h"
#include <atomic>
#include "FourierCalc.h"
#include <iostream>
#include <AL/al.h>
#include "string"

MainWin::MainWin(): fc_1(4000,0,4000), fc_2(4000, 0, 4000), fc_3(4000, 0, 4000),mixer(nullptr),source(50)
{
	// modulation index 0.001 correlates to 40 total level in deflemask
	Envelope e1(0,0,2,0,3,0,0);
	Envelope e2(1, 0.5, 3, 0.5, 3 + 0.05, 0.5, 0.5);


	this->op1 = Operator(1, 0.001,e1);
	this->op2 = Operator(4, 1,e2);
	
	this->cas = Cascade();
	this->cas.Append(&op1);
	this->cas.Append(&op2);
	this->inst.appendCas(&this->cas);

	this->mixer.setInstrument(&this->inst);

}


MainWin::~MainWin()
{

}

void MainWin::render()
{
	if (!resetKeyTime) {
		KeysInput::refreshPrevTime();
		this->resetKeyTime = true;
	}

	

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


	
	/*if (!fc_1.onWork() && !fc_2.onWork() && !fc_3.onWork())
	{
		if (ImGui::Button("begin"))
		{
			fc_1.init(this->inst,220,1000,0.0001,0,10);
			fc_2.init(this->inst, 440, 1000, 0.0001, 0, 10);
			fc_3.init(this->inst, 880, 1000, 0.0001, 0, 10);
		}
	}
	if (fc_1.isReady() && fc_2.isReady() && fc_3.isReady())
	{

		ImGui::Text("It's ready!");
		if (ImGui::Button("Display Chart"))
		{
			this->showGraph = true;
		}
		if (this->showGraph)
		{
			ImPlot::BeginPlot("Fourier");
			ImPlot::PlotLine("Fourier_220", fc_1.getRange(), fc_1.copyData(), fc_1.getDataLen());
			ImPlot::PlotLine("Fourier_440", fc_2.getRange(), fc_2.copyData(), fc_2.getDataLen());
			ImPlot::PlotLine("Fourier_880", fc_3.getRange(), fc_3.copyData(), fc_3.getDataLen());
			ImPlot::EndPlot();
		}
	}*/

	KeysInput::update();


	//
	
	vered::buffer16* pBuffer = new vered::buffer16(nullptr,0,0);
	*pBuffer = this->mixer.getBufferMono16(48000);//this->inst.getBuffer16Stereo(55, 0, 1, 48000);

	double* x_range = new double[pBuffer->samples];
	double* wav = new double[pBuffer->samples];


	for (int i = 0; i < pBuffer->samples; i++) {
		x_range[i] = i;
		wav[i] = (double)pBuffer->buffer[i];
	}

	ImPlot::BeginPlot("wave");
	ImPlot::PlotLine("wav", x_range, wav, pBuffer->samples);
	ImPlot::EndPlot();

	this->source.queueBuffer(pBuffer);
	this->source.stream();

	

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	
	ImGui::End();
}