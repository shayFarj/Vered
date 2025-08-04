#include "KeysInput.h"
#include <string>
#include <iostream>
#include <Windows.h>

int KeysInput::keyboard[24];

double KeysInput::keys_press[24];
double KeysInput::keys_release[24];

bool KeysInput::firstUse = false;

double KeysInput::delta = 0;
const char* KeysInput::keys;

bool KeysInput::cPressed[24] = { false,false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false, false, false, false };


bool KeysInput::pPressed[24] = { false,false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false, false, false, false };




std::chrono::time_point<std::chrono::steady_clock> KeysInput::prevTime;

void KeysInput::updatePressed()
{
	for (int i = 0; i < 24; i++)
	{
		KeysInput::pPressed[i] = KeysInput::cPressed[i];

		KeysInput::cPressed[i] = (bool)(GetAsyncKeyState((unsigned char)std::toupper(KeysInput::keys[i])) & 0x8000);

	}
}

bool KeysInput::hasReleased(int index)
{
	return (!KeysInput::cPressed[index]) && KeysInput::pPressed[index];
}

bool KeysInput::isPressed(int index)
{
	return KeysInput::cPressed[index];
}

KeysInput::~KeysInput()
{

}

const double & KeysInput::getDelta()
{
	const double & ref = KeysInput::delta;
	return ref;
}

KeysInput::KeysInput()
{
	KeysInput::keys = "ZSXDCVGBHNJMQ2W3ER5T6Y7U";
	
	for (int i = 0; i < 24; i++)
	{
		KeysInput::keyboard[i] = KeysInput::charToImgui(KeysInput::keys[i]);
		KeysInput::keys_press[i] = 0;
		KeysInput::keys_release[i] = 0;
	}

	KeysInput::prevTime = std::chrono::steady_clock::now();
}

void KeysInput::refreshPrevTime()
{
	KeysInput::prevTime = std::chrono::steady_clock::now();
}

int KeysInput::charToImgui(char key)
{
	if (std::isdigit(key))
		return 536 + key - 48;
	if (std::isalpha(key))
		return 546 + std::tolower(key) - 97;

	return 523; //returns backspace
}



void KeysInput::setKeys(const char* keys)
{
	if (strlen(keys) != 24) throw("Unfitting number of keys");

	for (int i = 0; i < 24; i++)
	{
		KeysInput::keyboard[i] = KeysInput::charToImgui(keys[i]);
	}
}

const double (&KeysInput::getKeysPress())[24]
{
	const double (&ref)[24] = KeysInput::keys_press;
	return ref;
}

const double(&KeysInput::getKeysRelease())[24]
{
		const double(&ref)[24] = KeysInput::keys_release;
		return ref;
}

void KeysInput::update()
{
	if (KeysInput::firstUse == false) {
		KeysInput::refreshPrevTime();
		KeysInput::firstUse = true;
	}
	else {
		auto currTime = std::chrono::steady_clock::now();
		KeysInput::delta = (currTime - KeysInput::prevTime).count() / 1000000000.0;

		for (int i = 0; i < 24; i++)
		{
			if (KeysInput::keys_release[i] > 0 && KeysInput::keys_release[i])
				KeysInput::keys_release[i] +=KeysInput::delta;

			if (ImGui::IsKeyDown((ImGuiKey)KeysInput::keyboard[i]))
			{
				if (KeysInput::keys_release[i] != 0)
					KeysInput::keys_press[i] = 0;
				KeysInput::keys_press[i] +=KeysInput::delta;

				KeysInput::keys_release[i] = 0;
			}


			if (ImGui::IsKeyReleased((ImGuiKey)KeysInput::keyboard[i]))
				KeysInput::keys_release[i] +=KeysInput::delta;

		}

		KeysInput::prevTime = currTime;
	}
}

void KeysInput::update(double delta)
{
	KeysInput::delta = delta;

	KeysInput::updatePressed();

	for (int i = 0; i < 24; i++)
	{
		if (KeysInput::keys_release[i] > 0)
			KeysInput::keys_release[i] += KeysInput::delta;

		if (KeysInput::isPressed(i))
		{
			if (KeysInput::keys_release[i] != 0)
				KeysInput::keys_press[i] = 0;
			KeysInput::keys_press[i] += KeysInput::delta;

			KeysInput::keys_release[i] = 0;
		}


		if (KeysInput::hasReleased(i))
			KeysInput::keys_release[i] += KeysInput::delta;

	}

	KeysInput::prevTime = std::chrono::steady_clock::now();
	
}