#include "globals.h"
#include <fstream>
#include <regex>
#include <iostream>

#include "game_data.h"
#include "graphics.h"
#include "level.h"

bool load_images_from_file(const std::string& image_path)
{
	game_data* gd = (game_data*)graphics::getUserData();

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

void setColor(graphics::Brush& br, const float* rgb)
{
	br.fill_color[0] = rgb[0];
	br.fill_color[1] = rgb[1];
	br.fill_color[2] = rgb[2];
	br.outline_color[0] = rgb[0];
	br.outline_color[1] = rgb[1];
	br.outline_color[2] = rgb[2];
}

void setColor(graphics::Brush& br, char c)
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
