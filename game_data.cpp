#include "game_data.h"

GameData::GameData() : fps(0), game_state(0),
	el(0.0f), sps(4.0f), curr_img(0), images(),
	levels(), curr_active_level(-1), curr_selected_level(-1)
{

	if (!load_levels_from_file(level_path))
		std::cerr << "Unable to load levels from: " << level_path << std::endl;

	// initialize collections here
	;
}

bool game_data::load_levels_from_file(const std::string& levels_path) {
	// create stream to levels_path
	std::ifstream in(levels_path);

	if (!in) {
		std::cerr << "Error opening file '" << levels_path << "'" << std::endl;
		return false;
	}

	// get all lines from file
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	std::smatch match;

	// regex pattern 1 ...
	std::string sr1(".*");

	// regex objects ...
	std::regex r1;

	int curr_level = 0, curr_wave = 0, line = 0;

	while (true)
	{
		// construct regexes ...
		r1 = std::regex(std::to_string(line) + "   " + sr1);

		// match with 1st ...
		if (std::regex_search(contents, match, r1))
		{
			// ...
		}
		
		// no match
		else { break; }
		++line;
		contents = match.suffix();
	}

	// close stream
	in.close();
	return true;
}


template <class T>
void GameData::update(float ms, list<T*>* ls) {
	for (Drawing* dr : ls) {
		dr->update(ms);
	}
}

template <class T>
void GameData::draw(list<T*>* ls) {
	for (Drawing* dr : ls) {
		dr->draw();
	}
}

template <class T1,class T2>
void GameData::checkCollisions(list<T1*>* ls1, list<T2*>* ls2) {
	for (GameObject* o1 : ls1)
		for (GameObject* o2 : ls2)
			o1->collides(o2);
}

template <class T>
void GameData::fire(list<T*>* ls) const{
	bool isPlayer = false;
	for (Entity* en : ls) {
		//check if projectile was launched by a player
		for (Entity* pl : playerLs) 
			if (pl == en) return true;
		
		if (en->hasFired && isPlayer)
			playerProjLs->push_back(en->getProjectile());
		else if (en->hasFired && !isPlayer)
			enemyProjLs->push_back(en->getProjectile());

	}
}

template <class T>
void GameData::checkAndDelete(list<T*>* ls) {
	for (iter = ls.begin(); iter != ls.end(); ++iter) {
		if (!*iter) {
			delete *iter;
			iter = ls->erase(iter);
		}
	}
}

GameData::~GameData() {
		delete[] enemyLs;      delete enemyLs;
		delete[] playerLs;     delete playerLs;
		delete[] enemyProjLs;  delete enemyProjLs;
		delete[] playerProjLs; delete playerProjLs;
		delete[] effectsLs;    delete effectsLs;
		delete[] enemyQueue;   delete enemyQueue;
}
