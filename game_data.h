#pragma once
#include "globals.h"
#include <list>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <array>

// needed because templates are defined in this header file
#include "projectile.h"
#include "entity.h"
#include "visual_effect.h"
#include "level.h"
#include "Powerup.h"
#include "button.h"

enum game_states;

class GameData {
private:
	class Stats;
	struct Achievement;

	//Helper method used to delete all data from the provided list, as well as the list itself
	template<class T>
	void static deleteList(std::list<T*>*);

	//Methods used to create levels upon initialization
	bool load_level_data_from_file(const std::string& level_path, const std::string& wave_path);
	bool _load_waves_from_file(const std::string& wave_path);
	bool _load_levels_from_file(const std::string& wave_path);
	//Failsafe in case reading from file fails
	void _load_hardcoded_levels();

	void create_buttons();

	int score;

public:
	GameData();
	~GameData();

	//=========ACHIEVEMENT DATA=======

	static Stats game_stats;
	static const std::array<Achievement *, 4> achievements; //The C++ Standard forbids containers of const elements

	//=========GENERAL DATA===========

	static const std::list<Achievement *> getAchieved();

	// collections
	std::list<Entity*>* enemyLs, * playerLs;
	std::list<Projectile*>* enemyProjLs, * playerProjLs;
	std::list<VisualEffect*>* effectsLs;
	std::list<Powerup*>* powerupLs;
	std::list<Button*>* buttons;

	// general
	int fps;
	int game_state;

	// loading
	float el;
	const float sps;
	int curr_img;
	std::vector<std::string> images;

	// game
	float bg_offset, height_perc_per_second;
	bool isMult = false;

	//Scrolls downwards and loops the background as the game progresses
	void updateBackground(float ms);
	void drawBackground(graphics::Brush&);

	//Data used by UI
	int curr_playing_level;
	float level_transition_timer;
	float set_level_transition_timer(float timer = 5.0f) { return timer; }
	void next_level() { ++curr_playing_level; }
  // lmao move this to gd.cpp
	Level* has_next_level() { 
		Level* return_val = levels[curr_playing_level + 1];
		if (!return_val)
			levels.erase(curr_playing_level + 1);
		return return_val;
	}
	void addScore(int scored);
	int getScore() const;

	//buttons
	void click_buttons();

	//=======LEVELS==========

	std::unordered_map<int, Level*> levels;
	std::unordered_map<std::string, Wave*> _waves;
	int curr_active_level, curr_selected_level;


	//Returns the next level to be used to spawn enemies
	Level* get_next_level();

	//Loads the next level
	void updateLevel(float ms) { levels[curr_playing_level]->update(ms); }

	//Spawns new enemies
	void spawn();


	//======COLLECTION FUNCTIONS=========

	//Updates all objects within the list. Template class must be derived from Drawing.
	template <class T>
	static void update(float ms, std::list<T*>*);

	// Draws all objects within the list. Template class must be derived from Drawing.
	template <class T>
	static void draw(std::list<T*>* ls);

	//Checks collisions between 2 lists. Template classes must both be derived from GameObject.
	template <class T1, class T2>
	static void checkCollisions(std::list<T1*>*, std::list<T2*>*);

	//Spawns a projectile for every eligible object in the list. Template class must be derived from Entity.
	template <class T>
	void fire(std::list<T*>*) const;

	//Checks if any object within the list must be destroyed, and deletes it. Template class must be derived from Drawing.
	template <class T>
	void checkAndDelete(std::list<T*>*);

};

//=======INNER CLASSES========

class GameData::Stats {
private:
	std::array<int, 4> shot_down_arr;
	int find_type(const Entity * const en) const;

public:
	const static int BASIC_PLANE = 0;
	const static int BLACK_PLANE = 1;
	const static int AIRSHIP = 2;
	const static int BALLOON = 3;
	const static int ALL = 100;

	//Called when an enemy is killed, updates internal data
	void plane_shot(const Entity * const en);
	//Get how many planes of a specific type were shot down
	int get_shot_number(int type) const;
	//Get the sum of all planes shot down
	int get_total_shot() const;
};

struct GameData::Achievement {
private:
	const int TYPE, KILLS;
public:
	const std::string name;
	const std::string icon;
	//Returns whether the condition was achieved
	bool is_achieved() const;

	Achievement(std::string name, std::string icon, int type, int kills);
};


//======COLLECTION FUNCTIONS=========
// Method definitions in the same file as declaration because of compiler constraints on template methods. 

template <class T>
void GameData::update(float ms, std::list<T*>* ls) {
	for (Drawing* dr : *ls)
		dr->update(ms);
}

template <class T>
void GameData::draw(std::list<T*>* ls) {
	for (Drawing* dr : *ls)
		dr->draw();
}

template <class T1, class T2>
void GameData::checkCollisions(std::list<T1*>* ls1, std::list<T2*>* ls2) {
	for (GameObject* o1 : *ls1)
		for (GameObject* o2 : *ls2)
			o1->collides(o2);
}

template <class T>
void GameData::fire(std::list<T*>* ls) const {
	bool isPlayer = false;
	for (Entity* en : *ls) {

		if (en->hasFired()) {
			graphics::playSound(sound_path + "fire.mp3", 0.04f, false);

			//check if projectile was launched by a player
			for (Entity* pl : *playerLs)
				isPlayer |= pl == en;

			if (isPlayer)
				playerProjLs->push_back(en->getProjectile());
			else
				enemyProjLs->push_back(en->getProjectile());

			effectsLs->push_back(en->getFireVisualEffect());
		}
	}
}

//Workaround as C++ doesn't permit method specialization
template<class T>
inline void delete_(const GameData * const gd, T* obj) {
	delete obj;
}

template<>
inline void delete_(const GameData * const gd, Entity* obj) {
	gd->effectsLs->push_back(obj->getDestructionVisualEffect());
	GameData::game_stats.plane_shot(obj);
	delete obj;
}

template <class T>
void GameData::checkAndDelete(std::list<T*>* ls) {
	for (auto iter = ls->begin(); iter != ls->end(); ++iter) {
		if (!**iter) {

			delete_(this, *iter);
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
void GameData::deleteList(std::list<T*>* ls) {
	for (T* obj : *ls)
		delete obj;
	delete ls;
}
