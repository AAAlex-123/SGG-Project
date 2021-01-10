#pragma once
#include "graphics.h"
#include "game_data.h"
#include <fstream>
#include <iostream>
#include <regex>

/**
* A file dedicated to storing numerous constants and functions that main.cpp uses
*/

// constants
int WINDOW_WIDTH  = 1200;
int WINDOW_HEIGHT = 600;
const float CANVAS_WIDTH  = 400.0f;
const float CANVAS_HEIGHT = 500.0f;

// constants to match window dimensions to canvas dimensions; used for mouse calculations
float w2c = (WINDOW_HEIGHT - CANVAS_HEIGHT) < (WINDOW_WIDTH - CANVAS_WIDTH)
	? CANVAS_HEIGHT / WINDOW_HEIGHT
	: CANVAS_WIDTH / WINDOW_WIDTH;
float c2w = 1.0f / w2c;

// functions
void initialize();

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
		gd->image_names.push_back(match[1].str());
		contents = match.suffix();
	}

	// close stream, delete file
	in.close();
	remove(temp_file_name.c_str());

	return true;
}
