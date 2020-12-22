#include "UI.h"
#include <string>

UI::UI(Entity* player, GameData* gd) :
	player1(player),player2(nullptr),isMult(false),width(get_canvas_width()),height(get_canvas_height()),gd(gd)
{
	initializeBrushes();
}

UI::UI(Entity* player1,Entity* player2, GameData* gd) :
	player1(player1), player2(player2), isMult(true), width(get_canvas_width()), height(get_canvas_height()), gd(gd)
{
	initializeBrushes();
}


inline void UI::initializeBrushes() {
	health_br.fill_color[0] = 1.f;
	health_br.fill_color[1] = 0.f;
	health_br.fill_color[2] = 0.f;

	health_br.fill_secondary_color[0] = 0.f;
	health_br.fill_secondary_color[1] = 1.f;
	health_br.fill_secondary_color[2] = 0.f;

	health_br.gradient_dir_u = 1.f;
	health_br.gradient_dir_v = 0.f;

	bg_br.outline_opacity = 0.f;
}


void UI::drawPlayerPanel(Entity* player, float x_pos){

	//draw Player 
	graphics::drawText(x_pos, height - 80, 20 , "Player: ", std_br);
	bg_br.texture = *(player->getSprite());
	graphics::setOrientation(90.f);
	graphics::drawRect(x_pos + 100, height - 80, 20, 40, bg_br);
	//draw Projectile
	graphics::setOrientation(0.f);
	graphics::drawText(x_pos, height - 50, 20 , "Bullet: ", std_br);
	bg_br.texture = *(player->getProjectile()->getSprite());
	graphics::drawRect(x_pos + 100, height - 50, 20, 40, bg_br);
	//reset brush
	bg_br.texture = "";

	//draw healthbar
	health_br.outline_opacity = 0.f;
	health_br.fill_opacity = 1.f;
	health_br.gradient = true;
	graphics::drawRect(x_pos - (1.f - ((float)player->getHealth() / (float)player->getMaxHealth()) * 150 / 2), height - 20, (float)player->getHealth() / (float)player->getMaxHealth() * 150, 20 , health_br);
	graphics::drawText(x_pos, height - 15, 20 , "HP", std_br);
	health_br.outline_opacity = 1.f;
	health_br.fill_opacity = 0.f;
	health_br.gradient = false;
	graphics::drawRect(x_pos + 75, height - 20 , 150, 20 , health_br);
}

void UI::draw(){
	graphics::setOrientation(0.f);
	//draw UI box
	bg_br.fill_color[0] = 0.8f;
	bg_br.fill_color[1] = 0.4f;
	bg_br.fill_color[2] = 0.1f;
	graphics::drawRect(width/2,height - 40,width,110,bg_br);

	std_br.fill_color[0] = 0.f;
	std_br.fill_color[1] = 1.f;
	std_br.fill_color[2] = 0.f;

	//draw Score
	graphics::drawText(width / 2 - 75, 60, 30, "Score: " + std::to_string(gd->getScore()), std_br);
	//draw FPS
	if ((int)graphics::getGlobalTime() % 50 == 0) //change fps every 0.5 seconds
		new_fps = gd->fps;
	graphics::drawText(width / 75, 2 * width / 20, ((width + height) / 2) / 35, "FPS: " + std::to_string(new_fps), std_br);
	//draw Level
	graphics::drawText(width / 75, 2 * width / 20 + 20, ((width + height) / 2) / 35, "Level: " + std::to_string(gd->curr_active_level), std_br);
	//reset 
	std_br.fill_color[0] = 0.f;
	std_br.fill_color[1] = 0.f;
	std_br.fill_color[2] = 0.f;
	//draw Panels
	drawPlayerPanel(player1, 0);
	if (isMult)
		drawPlayerPanel(player2, width - 175);
}

