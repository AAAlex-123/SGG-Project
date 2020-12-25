#pragma once
#include "globals.h"
#include <list>
#include <vector>
#include <iostream>
#include <unordered_map>

// needed because templates are defined in this header file
#include "projectile.h"
#include "entity.h"
#include "visual_effect.h"
#include "level.h"
#include "button.h"

// lmao imagine using using
using namespace std;
enum game_states;

class GameData {
private:
	template<class T>
	void static deleteList(list<T*>*);

	bool load_level_data_from_file(const string& level_path, const string& wave_path);
	bool _load_waves_from_file(const string& wave_path);
	bool _load_levels_from_file(const string& wave_path);
	// callback in case reading from file fails
	void _load_hardcoded_levels();

	int score;

public:
	// collections
	list<Entity*>* enemyLs, * playerLs;
	list<Projectile*>* enemyProjLs, * playerProjLs;
	list<VisualEffect*>* effectsLs;
	list<Button*>* buttons;

	// general
	int fps;
	int game_state;

	// loading
	float el;
	const float sps;
	int curr_img;
	vector<string> images;

	// players
	bool isMult = false;

	// levels
	unordered_map<int, Level*> levels;
	unordered_map<string, Wave*> _waves;
	int curr_active_level, curr_selected_level;

	// game
	float bg_offset, height_perc_per_second;
	void updateBackground(float ms);
	void drawBackground(graphics::Brush&);
	
	int curr_playing_level;
	float level_transition_timer;
	float set_level_transition_timer(float timer = 5.0f) { return timer; }
	void next_level() { --curr_playing_level; }
	Level* has_next_level() { 
		Level* return_val = levels[curr_playing_level - 1];
		if (!return_val)
			levels.erase(curr_playing_level - 1);
		return return_val;
	}

	// constructor and destructor because why not
	GameData();
	~GameData();
	
	// Level stuff
	void updateLevel(float ms) { levels[curr_playing_level]->update(ms); }
	void spawn()
	{
		if (levels[curr_playing_level]->can_spawn())
			enemyLs->push_back(levels[curr_playing_level]->spawn());
	}
	
	//Updates all objects within the list. Template class must be derived from Drawing.
	template <class T>
	static void update(float ms, list<T*>*);
	
	// Draws all objects within the list. Template class must be derived from Drawing.
	template <class T>
	static void draw(list<T*>* ls);

	//Checks collisions between 2 lists. Template classes must both be derived from GameObject.
	template <class T1,class T2>
	static void checkCollisions(list<T1*>*, list<T2*>*);

	//Spawns a projectile for every eligible object in the list. Template class must be derived from Entity.
	template <class T>
	void fire(list<T*>*) const;

	//Checks if any object within the list must be destroyed, and deletes it. Template class must be derived from Drawing.
	template <class T>
	void checkAndDelete(list<T*>*);

	//button stuff
	void click_buttons();
	
	void addScore(int scored) {
		score += scored;
	}

	int getScore() const{
		return score;
	}
};

// definition in the same file as declaration because c++ is awesome
template <class T>
void GameData::update(float ms, list<T*>* ls) {
	for (Drawing* dr : *ls) {
		dr->update(ms);
	}
}

template <class T>
void GameData::draw(list<T*>* ls) {
	for (Drawing* dr : *ls) {
		dr->draw();
	}
}

template <class T1,class T2>
void GameData::checkCollisions(list<T1*>* ls1, list<T2*>* ls2) {
	for (GameObject* o1 : *ls1)
		for (GameObject* o2 : *ls2)
			o1->collides(o2);
}

template <class T>
void GameData::fire(list<T*>* ls) const {
	bool isPlayer = false;
	for (Entity* en : *ls) {

		if (en->hasFired()) {
			//check if projectile was launched by a player
			for (Entity* pl : *playerLs)
				isPlayer |= pl == en;	// fancy

			if (isPlayer) {
				playerProjLs->push_back(en->getProjectile());
			} else {
				enemyProjLs->push_back(en->getProjectile());
			}
			effectsLs->push_back(en->getFireVisualEffect());
		}
	}
}

template <class T>
void GameData::checkAndDelete(list<T*>* ls) {
	for (auto iter = ls->begin(); iter != ls->end(); ++iter) {
		if (!**iter) {
			// don't question this
			if ((void*) ls == (void*)enemyLs || (void*)ls == (void*)playerLs) {
				effectsLs->push_back((*iter)->getDestructionVisualEffect());
			}
			delete (*iter);
			iter = ls->erase(iter);
			// if the last item is deleted, `iter == ls->end()`
			// so `++iter` increments the end iterator before checking the `for` condition
			if (iter == ls->end())
				break;
		}
	}
	if (playerLs->size() == 0)
		game_state = game_states::GAME_LOSE;
}

template<class T>
void GameData::deleteList(list<T*>* ls) {
	for (T* obj : *ls)
		delete obj;
	delete ls;
}
