#pragma once
#include "graphics.h"
#include "game_data.h"
#include <fstream>
#include <iostream>
#include <regex>

//A file dedicated to store the numerous constants and functions main.cpp uses

// constants
short WINDOW_WIDTH = 1200;
short WINDOW_HEIGHT = 600;
const float CANVAS_WIDTH = 400.0f;
const float CANVAS_HEIGHT = 500.0f;
// maps window to canvas or the inverse; used for mouse calculations
float w2c = (WINDOW_HEIGHT - CANVAS_HEIGHT) < (WINDOW_WIDTH - CANVAS_WIDTH)
	? CANVAS_HEIGHT / WINDOW_HEIGHT
	: CANVAS_WIDTH / WINDOW_WIDTH;
float c2w = 1.0f / w2c;

// function declarations
void initialize();
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

bool load_images_from_file(const std::string& image_path)
{
	GameData* gd = reinterpret_cast<GameData*> (graphics::getUserData());

	std::string temp_file_name = "_mytemp.txt";

	// create temp file and stream to that temp file
	system(("dir " + image_path + " > " + temp_file_name).c_str());
	std::ifstream in(temp_file_name);

	if (!in)
	{
		std::cerr << "Error opening file '" << temp_file_name << "'" << std::endl;
		return false;
	}

	// get all assets from file
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	std::smatch match;
	std::regex r("(\\w+\\.png)");

	while (std::regex_search(contents, match, r))
	{
		gd->images.push_back(match[1].str());
		contents = match.suffix();
	}

	// close stream, delete file
	in.close();
	remove(temp_file_name.c_str());

	return true;
}
