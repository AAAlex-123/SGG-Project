#pragma once
#include <vector>

class game_data
{
public:

	// general
	int fps, game_state;
	
	// loading
	float el;
	const float sps;
	int curr_img;
	std::vector<std::string> images;

	// other stuff

	// constructor
	game_data() : fps(0), game_state(0),
		el(0.0f), sps(1.0f), curr_img(0), images()
	{
		// initialize collections here
		;
	}
};
