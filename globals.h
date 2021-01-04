#pragma once
#include "graphics.h"
#include "scancodes.h"
#include <string>

typedef graphics::scancode_t key;

const float PI = 3.14159265359f;

// assets and other paths
const std::string asset_path = ".\\assets";
const std::string font = asset_path + "\\standard_font.ttf";
const std::string image_path = asset_path + "\\images\\";
const std::string icon_path = asset_path + "\\icons\\";
const std::string sound_path = asset_path + "\\sounds\\";
const std::string music_path = asset_path + "\\music\\";
const std::string level_path = ".\\levels.txt";
const std::string wave_path = ".\\waves.txt";

// enum
enum game_states
{
	TEST = -2,
	LOAD = -1,
	CREDITS = 0,
	MENU = 1,
	GAME = 2,
	GAME_LOSE = 10,
	GAME_WIN = 11,
	OPTIONS = 4,
	OP_LEVEL = 5,
	OP_PLAYER = 6,
	LEVEL_TRANSITION = 7,
	HELP = 8,
	EXIT = 9,
	PAUSE = 12,
	RESET = 13,
};

float get_canvas_height();
float get_canvas_width();

float mouse_x(float);
float mouse_y(float);

void setColor(graphics::Brush&, const float*);
void setColor(graphics::Brush&, char c);

struct Keyset
{
	key up, down, left, right, rleft, rright, fire;
	Keyset(key up, key down, key left, key right, key rleft, key rright, key fire)
		: up(up), down(down), left(left), right(right), rleft(rleft), rright(rright), fire(fire) {}
};
