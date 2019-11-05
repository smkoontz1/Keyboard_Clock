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
#include <WinUser.h>
#include <conio.h>

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
	{'0', LogiLed::KeyName::ZERO}
};

std::map<char, LogiLed::KeyName> char_to_numpad
{
	{'1', LogiLed::KeyName::NUM_ONE},
	{'2', LogiLed::KeyName::NUM_TWO},
	{'3', LogiLed::KeyName::NUM_THREE},
	{'4', LogiLed::KeyName::NUM_FOUR},
	{'5', LogiLed::KeyName::NUM_FIVE},
	{'6', LogiLed::KeyName::NUM_SIX},
	{'7', LogiLed::KeyName::NUM_SEVEN},
	{'8', LogiLed::KeyName::NUM_EIGHT},
	{'9', LogiLed::KeyName::NUM_NINE},
	{'0', LogiLed::KeyName::NUM_ZERO},
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
	int clock_secs = clock() / CLOCKS_PER_SEC;
	std::string hour = "";
	std::string min = "";
	std::string sec = "";
	
	std::cout << "Press \"Esc\" to stop..." << std::endl;

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		update_time(hour, min, sec);
		
		// Clear board
		LogiLedSetLighting(0, 0, 0);

		// Set Esc to red
		LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::ESC, 100, 0, 0);

		// Set hour to blue
		update_keyboard_leds(hour, false, 0, 0, 255);
		
		// Set minute to orange
		update_keyboard_leds(min, false, 229, 83, 0);
		
		// Set second to white
		update_keyboard_leds(sec, true, 255, 255, 255);
		
		std::cout << "Hour: " << hour << " Min: " << min << " Sec: " << sec << std::endl;
		
		// Wait 1 second
		while ((clock() / CLOCKS_PER_SEC - clock_secs) < 1);
		clock_secs = clock() / CLOCKS_PER_SEC;
	}

	LogiLedRestoreLighting();

	std::cout << "LED SDK Shutting down" << std::endl;

	LogiLedShutdown();

	return 0;
}

void update_time(std::string& hr, std::string& min, std::string& sec)
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
	std::string cur_sec = std::to_string(now_timeinfo.tm_sec);

	if (cur_min.length() == 1)
	{
		cur_min = "0" + cur_min;
	}

	if (cur_sec.length() == 1)
	{
		cur_sec = "0" + cur_sec;
	}

	// Compare strings and update
	if (hr.compare(cur_hr) != 0)
	{
		hr = cur_hr;
	}

	if (min.compare(cur_min) != 0)
	{
		min = cur_min;
	}

	sec = cur_sec;
}

void update_keyboard_leds(std::string& keys, bool is_numpad, int red, int blue, int green)
{
	float red_perc = ((static_cast<float>(red) / 255) * 100);
	float blue_perc = ((static_cast<float>(blue) / 255) * 100);
	float green_perc = ((static_cast<float>(green) / 255) * 100);

	for (int i = 0; i < keys.length(); i++)
	{
		LogiLed::KeyName key;
		
		if (is_numpad)
		{
			key = char_to_numpad[keys.at(i)];
		}
		else
		{
			key = char_to_keyname[keys.at(i)];
		}

		LogiLedSetLightingForKeyWithKeyName(key, static_cast<int>(red_perc), static_cast<int>(blue_perc), static_cast<int>(green_perc));
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