#pragma once
#include <imgui.h>
#include <chrono>
static class KeysInput
{
public:
	KeysInput();
	~KeysInput();

	static void setKeys(const char* keys);
	static void refreshPrevTime();
	
	static const double (&getKeysRelease())[24];
	static const double (&getKeysPress())[24];
	static const double & getDelta();

	static void update();


private:
	static int keyboard[24];

	static double keys_press[24];
	static double keys_release[24];
	
	static int charToImgui(char key);
	
	static bool firstUse;

	static std::chrono::time_point<std::chrono::steady_clock> prevTime;

	static double delta;
};

