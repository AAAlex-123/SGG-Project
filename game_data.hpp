#include "projectile.h"
#include "entity.h"
#include "visual_effect.h"
#include "level.h"
#include "Powerup.h"
#include "Player.h"
#include "button.h"

/**
 * A file containing the template methods' implementations
 */

template <class T>
void GameData::update(float ms, std::list<T*>* const ls)
{
	for (Drawing* dr : *ls)
		dr->update(ms);
}

template <class T>
void GameData::draw(std::list<T*>* const ls)
{
	for (Drawing* dr : *ls)
		dr->draw();
}

template <class T1, class T2>
void GameData::checkCollisions(std::list<T1*>* const ls1, std::list<T2*>* const ls2)
{
	for (GameObject* o1 : *ls1)
		for (GameObject* o2 : *ls2)
			o1->collides(o2);
}

template <class T>
void GameData::fire(std::list<T*>* const ls) const
{
	for (Entity* en : *ls)
	{
		bool isPlayer = false;
		if (en->hasFired())
		{
			graphics::playSound(sound_path + "fire.mp3", 0.04f, false);

			// check if projectile was launched by a player
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

template <class T>
void GameData::checkAndDelete(std::list<T*>* const ls)
{
	for (auto iter = ls->begin(); iter != ls->end(); ++iter)
	{
		if (!**iter)
		{
			delete_(this, *iter);
			iter = ls->erase(iter);
			// if the last item is deleted then `iter` is the end iterator so
			// `++iter` increments the end iterator, before checking the `for` condition
			if (iter == ls->end())
				break;
		}
	}
	if (playerLs->size() == 0)
		game_state = GAME_STATE::GAME_LOSE;
}

// Workaround as C++ doesn't permit method specialization
template<class T>
inline void delete_(const GameData* const gd, const T* const obj)
{
	delete obj;
}

template<>
inline void delete_(const GameData* const gd, const Entity* const obj)
{
	gd->effectsLs->push_back(obj->getDestructionVisualEffect());
	gd->stats->plane_shot(obj);
	delete obj;
}

template<class T>
void GameData::clearList(std::list<T*>* const ls)
{
	for (T* obj : *ls)
		delete obj;
}
