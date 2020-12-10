#include "constants.h"
#include <fstream>
#include <regex>
#include <iostream>

#include "game_data.h"
#include "graphics.h"
#include "level.h"
#include "GObjFactory.h"

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

bool load_levels_from_file(const std::string& levels_path)
{
	game_data* gd = (game_data*)graphics::getUserData();

	// create stream to levels_path
	std::ifstream in(levels_path);

	if (!in)
	{
		std::cerr << "Error opening file '" << levels_path << "'" << std::endl;
		return false;
	}

	// get all lines from file
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	std::smatch match;

	//0 straight enemies going down
	std::string sr1("(\\d) ([\\w ]+)");

	//4, 3, 100.0, 50.0, PI
	std::string sr2("(.+?), (\\d+), (.+?), (.+?), (.+)");

	std::regex r1, r2;

	int curr_level = 0, line = 0;

	while (true)
	{
		r1 = std::regex(std::to_string(line) + "   " + sr1);
		r2 = std::regex(std::to_string(line) + "   " + sr2);

		// new level
		if (std::regex_search(contents, match, r1))
		{
			curr_level = stoi(match[1].str());
			gd->levels[curr_level] = Level(curr_level, match[2].str());
		}

		// new enemy
		else if (std::regex_search(contents, match, r2))
		{
			gd->levels[curr_level].add_enemy(stof(match[1].str()), &GObjFactory::createEntity(stoi(match[2].str()), stof(match[3].str()), stof(match[4].str()), stof(match[5].str()) * PI / 180));
		}

		else { break; }
		++line;
		contents = match.suffix();
	}

	// close stream
	in.close();

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
