#pragma once
#include "projectile.h"
#include "entity.h"
#include "visual_effect.h"
#include <list>

using namespace std;

struct GameData {

	list<Entity*>* enemyLs = new list<Entity*>;
	list<Entity*>* playerLs = new list<Entity*>; //uniform access to either 1 or 2 players
	list<Projectile*>* enemyProjLs = new list<Projectile*>;
	list<Projectile*>* playerProjLs = new list<Projectile*>;
	list<VisualEffect*>* effectsLs = new list<VisualEffect*>;
	list<Entity*>* enemyQueue = new list<Entity*>;

	int fps;
	int game_state;

	//void spawn();
	
	//Updates all objects within the list. Template class must be derived from Drawing.
	template<class T>
	void update(float ms, list<T*>*);
	
	// Draws all objects within the list. Template class must be derived from Drawing.
	template <class T>
	void draw(list<T*>* ls);

	//Checks collisions between 2 lists. Template classes must both be derived from GameObject.
	template<class T1,class T2>
	void checkCollisions(list<T1*>*, list<T2*>*);

	//Spawns a projectile for every eligible object in the list. Template class must be derived from Entity.
	template<class T>
	void fire(list<T*>*) const;

	//Checks if any object within the list must be destroyed, and deletes it. Template class must be derived from Drawing.
	template<class T>
	void checkAndDelete(list<T*>*);

	~GameData();
};
