#include "game_data.h"

template <class T>
void GameData::update(float ms, list<T*>* ls) {
	for (Drawing* dr : ls) {
		dr->update(ms);
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
		delete[] enemyLs; delete enemyLs;
		delete[] playerLs; delete playerLs;
		delete[] enemyProjLs; delete enemyProjLs;
		delete[] playerProjLs; delete playerProjLs;
		delete[] effectsLs; delete effectsLs;
		delete[] enemyQueue; delete enemyQueue;
}
