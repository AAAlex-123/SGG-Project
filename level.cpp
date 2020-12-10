#include "level.h"

Level::Level(int id, std::string desc) : _id(id), _desc(desc), enemies(), _total_time(0.0f)
{}

void Level::operator=(const Level& rhs)
{
	_id = rhs._id;
	_desc = rhs._desc;
}

Entity& Level::spawn()
{
	Entity* return_value = enemies.front().second;
	enemies.pop();
	return *(return_value);
}

std::string Level::to_file_string()
{
	return "";
}
