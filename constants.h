#pragma once
#include "graphics.h"

// constants
const short WINDOW_WIDTH = 1200;
const short WINDOW_HEIGHT = 600;
const float CANVAS_WIDTH = 400.0f;
const float CANVAS_HEIGHT = 500.0f;

// enum
enum game_states
{
	TEST = -2,
	LOAD = -1,
	CREDITS = 0,
	MENU = 1,
	GAME = 2,
	END = 3,
	OPTIONS = 4,
	OP_LEVEL = 5,
	HELP = 8,
	EXIT = 9,
};

// function declarations
void initialize();
// true == success
bool load_images_from_file(const std::string& image_path);

// key short names
const graphics::scancode_t K_UP = graphics::scancode_t::SCANCODE_UP;
const graphics::scancode_t K_DOWN = graphics::scancode_t::SCANCODE_DOWN;
const graphics::scancode_t K_LEFT = graphics::scancode_t::SCANCODE_LEFT;
const graphics::scancode_t K_RIGHT = graphics::scancode_t::SCANCODE_RIGHT;
const graphics::scancode_t K_W = graphics::scancode_t::SCANCODE_W;
const graphics::scancode_t K_A = graphics::scancode_t::SCANCODE_A;
const graphics::scancode_t K_S = graphics::scancode_t::SCANCODE_S;
const graphics::scancode_t K_D = graphics::scancode_t::SCANCODE_D;
