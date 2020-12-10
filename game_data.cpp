#include "game_data.h"
#include <fstream>
#include <regex>
#include <iostream>
#include "GObjFactory.h"

bool game_data::load_levels_from_file(const std::string& levels_path)
{
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
			this->levels[curr_level] = Level(curr_level, match[2].str());
		}

		// new enemy
		else if (std::regex_search(contents, match, r2))
		{
			this->levels[curr_level].add_enemy(stof(match[1].str()), &GObjFactory::createEntity(stoi(match[2].str()), stof(match[3].str()), stof(match[4].str()), stof(match[5].str()) * PI / 180));
		}

		else { break; }
		++line;
		contents = match.suffix();
	}

	// close stream
	in.close();

	return true;
}
