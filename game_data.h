#pragma once
#include "projectile.h"
#include "entity.h"
#include "visual_effect.h"
#include "level.h"
#include "constants.h"
#include <list>
#include <vector>
#include <unordered_map>

// lmao imagine using using
using namespace std;

struct GameData {
private:
	bool load_levels_from_file(const std::string& levels_path);

public:
	list<Entity*>* enemyLs = new list<Entity*>;
	list<Entity*>* playerLs = new list<Entity*>; //uniform access to either 1 or 2 players or more if we do socket stuff (:
	list<Projectile*>* enemyProjLs = new list<Projectile*>;
	list<Projectile*>* playerProjLs = new list<Projectile*>;
	list<VisualEffect*>* effectsLs = new list<VisualEffect*>;
	list<Entity*>* enemyQueue = new list<Entity*>;

	// general
	int fps;
	int game_state;
	
	// loading
	float el;
	const float sps;
	int curr_img;
	std::vector<std::string> images;

	// levels
	std::unordered_map<int, Level> levels;
	int curr_active_level, curr_selected_level;
	
	// constructor because why not
	GameData();
	
	//Updates all objects within the list. Template class must be derived from Drawing.
	template <class T>
	void update(float ms, list<T*>*);
	
	// Draws all objects within the list. Template class must be derived from Drawing.
	template <class T>
	void draw(list<T*>* ls);

	//Checks collisions between 2 lists. Template classes must both be derived from GameObject.
	template <class T1,class T2>
	void checkCollisions(list<T1*>*, list<T2*>*);

	//Spawns a projectile for every eligible object in the list. Template class must be derived from Entity.
	template <class T>
	void fire(list<T*>*) const;

	//Checks if any object within the list must be destroyed, and deletes it. Template class must be derived from Drawing.
	template <class T>
	void checkAndDelete(list<T*>*);

	~GameData();
};

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

		if (en->hasFired) {
			//check if projectile was launched by a player
			for (Entity* pl : *playerLs)
				isPlayer |= pl == en;	// fancy

			if (isPlayer) {
				playerProjLs->push_back(&en->getProjectile());
			} else {
				enemyProjLs->push_back(&en->getProjectile());
			}
		}
	}
}

template <class T>
void GameData::checkAndDelete(list<T*>* ls) {
	for (auto iter = ls->begin(); iter != ls->end(); ++iter) {
		if (!*iter) {
			delete *iter;
			iter = ls->erase(iter);
		}
	}
}

template<class T>
void GameData::deleteList(list<T*>* ls) {
	for (T* obj : *ls)
		delete obj;
	delete ls;
}
