#pragma once
#include "projectile.h"
#include "entity.h"
#include "visual_effect.h"
#include "level.h"
#include <list>
#include <unordered_map>

// lmao imagine using using
using namespace std;

struct GameData {
private:
	template<class T>
	void static deleteList(list<T*>*);
	bool load_levels_from_file(const string& filename);
	int score;

public:
	// collections
	list<Entity*> *enemyLs, *playerLs;
	list<Projectile*> *enemyProjLs, *playerProjLs;
	list<VisualEffect*> *effectsLs;

	// general
	int fps;
	int game_state;
	
	// loading
	float el;
	const float sps;
	int curr_img;
	std::vector<std::string> images;

	// levels
	std::unordered_map<int, Level*> levels;
	int curr_active_level, curr_selected_level;
	
	// constructor and destructor because why not
	GameData();
	~GameData();
	
	// Level stuff
	inline void updateLevel(float ms) { levels[curr_selected_level]->update(ms); }
	inline void spawn()
	{
		if (levels[curr_selected_level]->can_spawn())
			enemyLs->push_back(levels[curr_selected_level]->spawn());
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
	static void checkAndDelete(list<T*>*);
	
	void addScore(int scored) {
			score += scored;
	}

	int getScore() {
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
		}
	}
}

template <class T>
void GameData::checkAndDelete(list<T*>* ls) {
	for (auto iter = ls->begin(); iter != ls->end(); ++iter) {
		if (!**iter) {
			delete (*iter);
			iter = ls->erase(iter);
			// if the last item is deleted, `iter == ls->end()`
			// so `++iter` increments the end iterator before checking the `for` condition
			if (iter == ls->end())
				break;
		}
	}
}

template<class T>
void GameData::deleteList(list<T*>* ls) {
	for (T* obj : *ls)
		delete obj;
	delete ls;
}
