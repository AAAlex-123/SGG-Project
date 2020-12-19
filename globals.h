#pragma once
#include "scancodes.h"
#include <string>
typedef graphics::scancode_t key;

const float PI = 3.14159265359f;

struct Keyset
{
	key up, down, left, right, rleft, rright, fire;
	Keyset(key up, key down, key left, key right, key rleft, key rright, key fire)
		: up(up), down(down), left(left), right(right), rleft(rleft), rright(rright), fire(fire) {}
};

// assets and other paths
const std::string asset_path = ".\\assets\\";
const std::string font = asset_path + "orange juice 2.0.ttf";
const std::string image_path = ".\\assets\\";
const std::string level_path = ".\\levels.txt";
const std::string sound_path = ".\\sounds\\";

float get_canvas_height();
float get_canvas_width();
