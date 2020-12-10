#include "level.h"

Level::Level(int id, std::string desc) : _id(id), _desc(desc), waves(), _total_time(0.0f)
{}

void Level::operator=(const Level& rhs)
{
	_id = rhs._id;
	_desc = rhs._desc;
}

std::string Level::to_file_string()
{
	return "";
}
