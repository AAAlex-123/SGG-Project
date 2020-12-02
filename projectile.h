#pragma once
#include "gameobject.h"
#include "Path.h"

//Superclass for all projectiles
class Projectile : public GameObject{
public:
	Projectile(float xpos, float ypos, float size, std::string * sprites,int vel,int damage) :
		GameObject(xpos, ypos, size, sprites, 1, vel, damage, 1)
	{};
	virtual void update() override;

};
