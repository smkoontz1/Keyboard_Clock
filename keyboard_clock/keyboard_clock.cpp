// keyboard_clock.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include "LogitechLEDLib.h"
#include "keyboard_clock.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <map>
#include <Windows.h>

std::map<char, LogiLed::KeyName> char_to_keyname
{
	{'A', LogiLed::KeyName::A},
	{'B', LogiLed::KeyName::B},
	{'C', LogiLed::KeyName::C},
	{'D', LogiLed::KeyName::D},
	{'E', LogiLed::KeyName::E},
	{'F', LogiLed::KeyName::F},
	{'G', LogiLed::KeyName::G},
	{'H', LogiLed::KeyName::H},
	{'I', LogiLed::KeyName::I},
	{'J', LogiLed::KeyName::J},
	{'K', LogiLed::KeyName::K},
	{'L', LogiLed::KeyName::L},
	{'M', LogiLed::KeyName::M},
	{'N', LogiLed::KeyName::N},
	{'O', LogiLed::KeyName::O},
	{'P', LogiLed::KeyName::P},
	{'Q', LogiLed::KeyName::Q},
	{'R', LogiLed::KeyName::R},
	{'S', LogiLed::KeyName::S},
	{'T', LogiLed::KeyName::T},
	{'U', LogiLed::KeyName::U},
	{'V', LogiLed::KeyName::V},
	{'W', LogiLed::KeyName::W},
	{'X', LogiLed::KeyName::X},
	{'Y', LogiLed::KeyName::Y},
	{'Z', LogiLed::KeyName::Z},
	{'1', LogiLed::KeyName::ONE},
	{'2', LogiLed::KeyName::TWO},
	{'3', LogiLed::KeyName::THREE},
	{'4', LogiLed::KeyName::FOUR},
	{'5', LogiLed::KeyName::FIVE},
	{'6', LogiLed::KeyName::SIX},
	{'7', LogiLed::KeyName::SEVEN},
	{'8', LogiLed::KeyName::EIGHT},
	{'9', LogiLed::KeyName::NINE},
	{'0', LogiLed::KeyName::ZERO},
};

int main()
{
	// Initialize the LED SDK
	bool LedInitialized = LogiLedInitWithName("Keyboard Clock!");

	if (!LedInitialized)
	{
		std::cout << "LogiLedInit() failed." << std::endl;

		return 0;
	}

	std::cout << "LED SDK Initialized" << std::endl;

	LogiLedSetTargetDevice(LOGI_DEVICETYPE_ALL);

	// Save current lighting
	LogiLedSaveCurrentLighting();

	// Set all devices to Black
	LogiLedSetLighting(0, 0, 0);

	// Set some keys on keyboard
	int secs = clock() / CLOCKS_PER_SEC;
	bool key_pressed = 0;
	std::string hour = "";
	std::string min = "";
	
	std::cout << "Press \"Esc\" to stop...";

	while (!key_pressed)
	{
		while ((clock() / CLOCKS_PER_SEC - secs) < 5);
		update_time(hour, min);
		update_keyboard_leds(hour, min);
		if (GetKeyState(VK_ESCAPE) && 0x8000)
		{
			key_pressed = 1;
		}
		secs = clock() / CLOCKS_PER_SEC;
	}

	LogiLedRestoreLighting();

	std::cout << "LED SDK Shutting down" << std::endl;

	LogiLedShutdown();

	return 0;
}

void update_time(std::string& hr, std::string& min)
{
	time_t now = time(0);
	struct tm now_timeinfo;

	localtime_s(&now_timeinfo, &now);

	int hour = now_timeinfo.tm_hour;

	// Convert to 12 hour time
	if (hour > 11)
	{
		hour = hour - 12;
	}

	// Convert to strings
	std::string cur_hr = get_hour_string(hour);
	std::string cur_min = std::to_string(now_timeinfo.tm_min);

	// Compare strings and update
	if (hr.compare(cur_hr) != 0)
	{
		hr = cur_hr;
	}

	if (min.compare(cur_min) != 0)
	{
		min = cur_min;
	}
}

void update_keyboard_leds(std::string& hr_str, std::string& min_str)
{
	// TODO: Make this not suck
	LogiLedSetLighting(0, 0, 0);
	
	for (int i = 0; i < hr_str.length(); i++)
	{
		LogiLedSetLightingForKeyWithKeyName(char_to_keyname[hr_str.at(i)], 100, 0, 0);
	}

	if (min_str.length() < 2)
	{
		std::cout << "WTF?" << std::endl;
	}
	else
	{
		LogiLedSetLightingForKeyWithKeyName(char_to_keyname[min_str.at(0)], 0, 0, 100);
		LogiLedSetLightingForKeyWithKeyName(char_to_keyname[min_str.at(1)], 0, 0, 100);
	}
}

std::string get_hour_string(int& hr)
{
	switch (hr)
	{
	case 0:
		return "TWELVE";
	case 1:
		return "ONE";
	case 2:
		return "TWO";
	case 3:
		return "THREE";
	case 4:
		return "FOUR";
	case 5:
		return "FIVE";
	case 6:
		return "SIX";
	case 7:
		return "SEVEN";
	case 8:
		return "EIGHT";
	case 9:
		return "NINE";
	case 10:
		return "TEN";
	case 11:
		return "ELEVEN";
	case 12:
		return "TWELVE";
	default:
		return "ERR";
	}
}