#pragma once
#include "globals.h"
#include "Player.h"
#include "game_data.h"

//A class that handles all information given to the player(s) such as health, score, fps and level index
class UI {
private:
	const float width, height;
	graphics::Brush health_br, std_br, bg_br;
	const GameData* const gd;
	int new_fps = 40;

	//Draws a panel with a player's sprite, bullets and healthbar
	void drawPlayerPanel(const Player* const player, float x_pos);	//none of these methods are const as they depend on changing the values of the 3 branches
	//Drawn when a player dies
	void drawDestroyedPanel(float x_pos);
	//Gives default values to the 3 brushes
	void initializeBrushes();

public:
	//The height the UI will occupy to draw its panels
	static const int box_height = 40;

	UI(const GameData* const gd);
	void draw();
};
