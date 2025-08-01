#include "KeysInput.h"
#include <string>
#include <iostream>

int KeysInput::keyboard[24];

double KeysInput::keys_press[24];
double KeysInput::keys_release[24];

bool KeysInput::firstUse = false;

double KeysInput::delta = 0;


std::chrono::time_point<std::chrono::steady_clock> KeysInput::prevTime;

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
	const char* keys = "zsxdcvgbhnjmq2w3er5t6y7u";
	
	for (int i = 0; i < 24; i++)
	{
		KeysInput::keyboard[i] = KeysInput::charToImgui(keys[i]);
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