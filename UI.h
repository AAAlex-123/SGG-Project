#pragma once
#include "entity.h"
#include "graphics.h"
#include "globals.h"
#include "game_data.h"

class UI{

private:
	bool isMult;
	const float width, height;
	Entity* player1; 
	Entity* player2; //might be nullptr depending on initialization so not &
	graphics::Brush health_br, std_br, bg_br;
	const GameData *const gd;
	int new_fps = 40;

	void drawPlayerPanel(Entity* player, float x_pos) ;
	void initializeBrushes();

public:
	UI(Entity* player, GameData* gd);
	UI(Entity* player1, Entity* player2, GameData* gd);

	void draw();
};

