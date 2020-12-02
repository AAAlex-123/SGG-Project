#include "projectile.h"

StraightPath* Projectile::movement = new StraightPath();

void Projectile::update(){
	movement->move(this->x,this->y,this->vel);
}

void Projectile::hit(){
	;
}

