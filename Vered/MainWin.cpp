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

MainWin::MainWin(): mixer(nullptr),fWin(this->iBoard.getInst())
{
	// modulation index 0.001 correlates to 40 total level in deflemask
	this->mixer.setInstrument(iBoard.getInst());

	

	PaStreamer::init(iBoard.getInst());

}


MainWin::~MainWin()
{

}

void MainWin::shutdown()
{
	PaStreamer::pauseStream();
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

	this->iBoard.render();

	ImGui::Begin("Mixer");


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
	
	vered::bufferDouble* pBuffer = new vered::bufferDouble(nullptr,0,0);
	*pBuffer = this->mixer.getDemoBufferMono(48000);//this->inst.getBuffer16Stereo(55, 0, 1, 48000);

	//double* wav = this->mixer.getDemoArrayMono(48000);

	double* x_range = new double[pBuffer->samples];
	double* wav = new double[pBuffer->samples];


	for (int i = 0; i < pBuffer->samples; i++) {
		x_range[i] = i;
		wav[i] = (double)pBuffer->buffer[i];
	}


	if (ImPlot::BeginPlot("Mix"))
	{
		ImPlot::PlotLine("Mix", x_range, wav, pBuffer->samples);
		ImPlot::EndPlot();
		//this->source.queueBuffer(pBuffer);
		//this->source.stream();
	}

	delete[] x_range;
	delete[] wav;
	
	ImGui::Text("Current Octave:");
	ImGui::Text(std::to_string(PaStreamer::getOctave()).c_str());

	if (ImGui::Button("Raise Octave"))
	{
		PaStreamer::setOctave(PaStreamer::getOctave() + 1);
	}

	if (ImGui::Button("Low Octave"))
	{
		PaStreamer::setOctave(PaStreamer::getOctave() - 1);
	}

	this->fWin.render();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	
	ImGui::End();
}