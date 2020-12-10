#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>
#include "level.h"

class game_data
{
private:
	bool load_levels_from_file(const std::string& levels_path);

public:

	// general
	int fps, game_state;
	
	// loading
	float el;
	const float sps;
	int curr_img;
	std::vector<std::string> images;

	// levels		   id of level
	std::unordered_map<int, Level> levels;
	int curr_active_level, curr_selected_level;

	// other stuff

	// constructor
	game_data() : fps(0), game_state(0),
		el(0.0f), sps(4.0f), curr_img(0), images(),
		levels(), curr_active_level(-1), curr_selected_level(-1)
	{

		if (!load_levels_from_file(level_path))
			std::cerr << "Unable to load levels from: " << level_path << std::endl;

		// initialize collections here
		;
	}
};
