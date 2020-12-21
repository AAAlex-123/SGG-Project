#include "UI.h"
#include <string>

UI::UI(Entity* player) :
	player1(player),player2(nullptr),isMult(false),width(get_canvas_width()),height(get_canvas_height())
{
	initializeBrushes();
}

UI::UI(Entity* player1,Entity* player2) :
	player1(player1), player2(player2), isMult(true), width(get_canvas_width()), height(get_canvas_height())
{
	initializeBrushes();
}


inline void UI::initializeBrushes() {
	health_br.fill_color[0] = 1.f;
	health_br.fill_color[1] = 0;
	health_br.fill_color[2] = 0;

	health_br.fill_secondary_color[0] = 0;
	health_br.fill_secondary_color[1] = 1.f;
	health_br.fill_secondary_color[2] = 0;

	health_br.gradient_dir_u = 1;
	health_br.gradient_dir_v = 0;

	bg_br.outline_opacity = 0.f;
}


void UI::drawPlayerPanel(Entity* player, int x_pos){

	//draw Player 
	graphics::drawText(x_pos, height - 100, 30 , "Player: ", std_br);
	bg_br.texture = *(player->getSprite());
	graphics::setOrientation(90.f);
	graphics::drawRect(x_pos + 200, height - 100, 40, 80, bg_br);
	//draw Projectile
	graphics::setOrientation(0.f);
	graphics::drawText(x_pos, height - 50, 30 , "Bullet: ", std_br);
	bg_br.texture = *(player->getProjectile()->getSprite());
	graphics::drawRect(x_pos + 200, height - 50, 20, 40, bg_br);
	//reset brush
	bg_br.texture = "";

	//draw healthbar
	health_br.outline_opacity = 0.f;
	health_br.fill_opacity = 1.f;
	health_br.gradient = true;
	graphics::drawRect(x_pos - (1.f - ((float)player->getHealth() / (float)player->getMaxHealth()) * 300 / 2), height - 30, (float)player->getHealth() / (float)player->getMaxHealth() * 300, 30 , health_br);
	graphics::drawText(x_pos, height - 20, 30 , "HP", std_br);
	health_br.outline_opacity = 1.f;
	health_br.fill_opacity = 0.f;
	health_br.gradient = false;
	graphics::drawRect(x_pos + 300 / 2, height - 30 , 300, 30 , health_br);
}

void UI::draw(GameData* gd){
	bg_br.fill_color[0] = 0.8f;
	bg_br.fill_color[1] = 0.4f;
	bg_br.fill_color[2] = 0.1f;
	//draw UI box
	graphics::drawRect(width/2,height - 50,width,150,bg_br);
	std_br.fill_color[0] = 0.f;
	std_br.fill_color[1] = 1.f;
	std_br.fill_color[2] = 0.f;

	//draw Score
	graphics::drawText(width / 2 - 150, 60, 50, "Score: " + std::to_string(gd->getScore()), std_br);
	//draw FPS
	if ((int)graphics::getGlobalTime() % 50 == 0) //change fps every 0.5 seconds
		new_fps = gd->fps;
	graphics::drawText(width / 150, 2 * width / 20, ((width + height) / 2) / 35, "FPS: " + std::to_string(new_fps), std_br);
	//draw Level
	graphics::drawText(width / 150, 2 * width / 20 + 20, ((width + height) / 2) / 35, "Level: " + std::to_string(gd->curr_active_level), std_br);
	//reset 
	std_br.fill_color[0] = 0.f;
	std_br.fill_color[1] = 0.f;
	std_br.fill_color[2] = 0.f;
	//draw Panels
	drawPlayerPanel(player1, 0);
	if (isMult)
		drawPlayerPanel(player2, width - 350);
}

