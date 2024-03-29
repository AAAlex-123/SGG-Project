#pragma once
#include "graphics.h"
#include "scancodes.h"
#include <string>

/**
 * Functions and data that should be accessible by every class
 */

typedef graphics::scancode_t key;

const float PI = 3.14159265359f;

// assets and other paths
const std::string asset_path = ".\\assets";
const std::string image_path = asset_path + "\\images\\";
const std::string icon_path = asset_path + "\\icons\\";
const std::string sound_path = asset_path + "\\sounds\\";
const std::string music_path = asset_path + "\\music\\";
const std::string font_file = asset_path + "\\standard_font.ttf";
const std::string level_file = ".\\levels.txt";
const std::string wave_file = ".\\waves.txt";
const std::string hardcoded_level_file = asset_path + "\\hardcoded_levels.txt";
const std::string hardcoded_wave_file = asset_path + "\\hardcoded_waves.txt";

// enum
enum class GAME_STATE
{
	LOAD,
	LOAD_L,
	INFO,
	MENU,
	ACHIEVEMENTS,
	GAME,
	LEVEL_TRANSITION,
	GAME_LOSE,
	GAME_WIN,
	RESET,
	OPTIONS,
	OP_LEVEL,
	OP_PLAYER,
	HELP,
	EXIT,
	PAUSE,
	CREDITS,
	INVALID_STATE,
};

float get_canvas_height();
float get_canvas_width();

float mouse_x(float);
float mouse_y(float);

/**
* A class defining a set of keys the player uses to move
*/
struct Keyset
{
	key up, down, left, right, rleft, rright, fire;
	Keyset(key up, key down, key left, key right, key rleft, key rright, key fire)
		: up(up), down(down), left(left), right(right), rleft(rleft), rright(rright), fire(fire) {}
};

inline void setColor(graphics::Brush& br, const float* rgb)
{
	br.fill_color[0] = rgb[0];
	br.fill_color[1] = rgb[1];
	br.fill_color[2] = rgb[2];
	br.outline_color[0] = rgb[0];
	br.outline_color[1] = rgb[1];
	br.outline_color[2] = rgb[2];
}

inline void setColor(graphics::Brush& br, char c)
{
	switch (c) {
	case 'R':
		setColor(br, new float[3]{ 1.0f, 0.0f, 0.0f });
		break;
	case 'G':
		setColor(br, new float[3]{ 0.0f, 1.0f, 0.0f });
		break;
	case 'B':
		setColor(br, new float[3]{ 0.0f, 0.0f, 1.0f });
		break;
	case 'P':
		setColor(br, new float[3]{ 1.0f, 0.0f, 1.0f });
		break;
	case 'Y':
		setColor(br, new float[3]{ 1.0f, 1.0f, 0.0f });
		break;
	case 'O':
		setColor(br, new float[3]{ 1.0f, 0.5f, 0.0f });
		break;
	case 'N':
		setColor(br, new float[3]{ 0.6f, 0.2f, 0.2f });
		break;
	case 'W':
		setColor(br, new float[3]{ 1.0f, 1.0f, 1.0f });
		break;
	case 'A':
		setColor(br, new float[3]{ 0.5f, 0.5f, 0.5f });
		break;
	case 'L':
		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		break;
	}
}
