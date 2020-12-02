#pragma once
#include "gameobject.h"
#include "shooting.h"
#include "projectile.h"

class Player : public GameObject, public Shooting{
private:

public:
	virtual void update() override;
	virtual void hit(GameObject& o2) override; //maybe play a sound idk lol

};
