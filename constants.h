#pragma once
#include "graphics.h"

// global variables
graphics::Brush br;
// constants
const short WINDOW_WIDTH = 1200;
const short WINDOW_HEIGHT = 600;
const float CANVAS_WIDTH = 1200.0f;
const float CANVAS_HEIGHT = 600.0f;

// enum
enum game_states
{
	CREDITS = 0,
	MENU = 1,
	GAME = 2,
	END = 3,
	OPTIONS = 4,
	HELP = 8,
	EXIT = 9,
};

// function declarations
void initialize();

void setColor(graphics::Brush&, const float*);
void setColor(graphics::Brush&, char c);

// assets
const std::string asset_path = "assets\\";
const std::string font = asset_path + "orange juice 2.0.ttf";

// key short names
const graphics::scancode_t K_UP = graphics::scancode_t::SCANCODE_UP;
const graphics::scancode_t K_DOWN = graphics::scancode_t::SCANCODE_DOWN;
const graphics::scancode_t K_LEFT = graphics::scancode_t::SCANCODE_LEFT;
const graphics::scancode_t K_RIGHT = graphics::scancode_t::SCANCODE_RIGHT;
const graphics::scancode_t K_W = graphics::scancode_t::SCANCODE_W;
const graphics::scancode_t K_A = graphics::scancode_t::SCANCODE_A;
const graphics::scancode_t K_S = graphics::scancode_t::SCANCODE_S;
const graphics::scancode_t K_D = graphics::scancode_t::SCANCODE_D;
const graphics::scancode_t K_T = graphics::scancode_t::SCANCODE_T;
const graphics::scancode_t K_F = graphics::scancode_t::SCANCODE_F;
const graphics::scancode_t K_G = graphics::scancode_t::SCANCODE_G;
const graphics::scancode_t K_H = graphics::scancode_t::SCANCODE_H;
const graphics::scancode_t K_I = graphics::scancode_t::SCANCODE_I;
const graphics::scancode_t K_J = graphics::scancode_t::SCANCODE_J;
const graphics::scancode_t K_K = graphics::scancode_t::SCANCODE_K;
const graphics::scancode_t K_L = graphics::scancode_t::SCANCODE_L;

