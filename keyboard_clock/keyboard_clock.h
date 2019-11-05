#pragma once

#include <string>

void update_time(std::string& hr, std::string& min, std::string& sec);
void update_keyboard_leds(std::string& keys, bool is_numpad, int red, int blue, int green);
std::string get_hour_string(int& hr);
