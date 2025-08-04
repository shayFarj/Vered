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
	static void update(double delta);


private:
	static int keyboard[24];

	static double keys_press[24];
	static double keys_release[24];
	
	static int charToImgui(char key);
	static void updatePressed();
	static bool hasReleased(int index);
	static bool isPressed(int index);
	
	static bool firstUse;

	static std::chrono::time_point<std::chrono::steady_clock> prevTime;

	static double delta;

	static bool cPressed[24];
	static bool pPressed[24];

	static const char * keys;
};

