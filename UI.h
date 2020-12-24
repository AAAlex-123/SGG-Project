#pragma once
#include "globals.h"
#include "entity.h"
#include "game_data.h"

class UI {
private:
	bool isMult;
	const float width, height;
	graphics::Brush health_br, std_br, bg_br;
	const GameData* const gd;
	int new_fps = 40;

	void drawPlayerPanel(const Entity* player, float x_pos);
	void drawDestroyedPanel(float x_pos);
	void initializeBrushes();

public:
	UI(GameData* gd);

	void draw();
};
