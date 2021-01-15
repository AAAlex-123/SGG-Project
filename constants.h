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
int WINDOW_WIDTH = 1200;
int WINDOW_HEIGHT = 600;
const float CANVAS_WIDTH = 400.0f;
const float CANVAS_HEIGHT = 500.0f;

// constants to match window dimensions to canvas dimensions; used for mouse calculations
float w2c = ((float)WINDOW_HEIGHT - CANVAS_HEIGHT) < ((float)WINDOW_WIDTH - CANVAS_WIDTH)
	? CANVAS_HEIGHT / (float)WINDOW_HEIGHT
	: CANVAS_WIDTH / (float)WINDOW_WIDTH;
float c2w = 1.0f / w2c;

enum class MUSIC
{
	MENU_MUSIC,
	BATTLE_MUSIC,
	LOSE_MUSIC,
	WIN_MUSIC,
};

// functions

void update(float);
void draw();
void resize(int, int);
int main();
void initialize();
void close();
//A thread function that spawns enemies and updates all objects in the game
void updateAndSpawn(GameData* const starting_gd, float* const ms);
//A thread function that checks for collisions between objects and creates projectiles when needed
void checkAndFire(GameData* const starting_gd, float* const ms);

bool load_images_from_file(const std::string& image_directory_path, const std::string& icon_directory_path)
{
	GameData* gd = reinterpret_cast<GameData*> (graphics::getUserData());

	std::string temp_file_name = "_mytemp.txt";

	// create temp file and list there the contents of .\assets\images and .\assets\icons so they can be read line by line
	// C:\...\1942ripoff>dir .\assets\images .\assets\icons /b /s > _mytemp.txt
	system(("dir " + image_directory_path + " " + icon_directory_path + "/b /s > " + temp_file_name).c_str());
	std::ifstream in(temp_file_name);

	if (!in)
	{
		std::cerr << "Error opening file '" << temp_file_name << "'" << std::endl;
		return false;
	}

	// get all assets from the icons and images folders
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	std::smatch match;
	std::regex r = std::regex("\\assets(\\\\(?:icons|images).*?\\.png)");

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
