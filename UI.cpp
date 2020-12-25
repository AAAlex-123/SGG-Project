#include "UI.h"

UI::UI(GameData* gd) :
	width(get_canvas_width()), height(get_canvas_height()), gd(gd)
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

	std_br.fill_color[0] = 0.f;
	std_br.fill_color[1] = 0.f;
	std_br.fill_color[2] = 0.f;
}



void UI::drawPlayerPanel(const Entity* player, float x_pos) {

	//draw Text
	graphics::drawText(x_pos, height - 80, 20, "Player: ", std_br);
	graphics::drawText(x_pos, height - 50, 20, "Bullets:", std_br);
	graphics::drawText(x_pos, height - 15, 20, "HP", std_br);

	//draw Player icon
	setColor(bg_br, 'W');

	bg_br.texture = *(player->getSprite());
	graphics::setOrientation(90.f);
	graphics::drawRect(x_pos + 100, height - 80, 20, 40, bg_br);

	//draw Projectile icon
	bg_br.texture = *(player->getProjectile()->getSprite());
	graphics::setOrientation(0.f);
	graphics::drawRect(x_pos + 90,  height - 50, 20, 40, bg_br);
	graphics::drawRect(x_pos + 100, height - 50, 20, 40, bg_br);
	graphics::drawRect(x_pos + 110, height - 50, 20, 40, bg_br);

	//draw healthbar
	health_br.outline_opacity = 0.f;
	health_br.fill_opacity = 1.f;
	health_br.gradient = true;
	float perc = (float)player->getHealth() / (float)player->getMaxHealth();
	graphics::drawRect(x_pos - (1.f - perc * 150 / 2), height - 20, perc * 150, 20, health_br);

	health_br.outline_opacity = 1.f;
	health_br.fill_opacity = 0.f;
	health_br.gradient = false;
	graphics::drawRect(x_pos + 75, height - 20, 150, 20, health_br);
}

void UI::drawDestroyedPanel(float x_pos) {

	//draw Text
	graphics::drawText(x_pos, height - 80, 20, "Player: ", std_br);
	graphics::drawText(x_pos, height - 50, 20, "Bullets:", std_br);
	graphics::drawText(x_pos, height - 15, 20, "HP", std_br);

	//draw Player icon
	bg_br.texture = image_path + "skull.png";
	graphics::drawRect(x_pos + 100, height - 80, 30, 30, bg_br);

	//draw empty healthbar
	graphics::drawRect(x_pos + 75, height - 20, 150, 20, health_br);

	//draw X
	bg_br.texture = image_path + "X.png";
	graphics::drawRect((width+x_pos)/2, height - 47, 95, 95, bg_br);
}

void UI::draw() {
	graphics::setOrientation(0.f);
	//draw UI box
	setColor(bg_br, new float[3]{ 0.8f, 0.4f, 0.1f });
	bg_br.texture = "";
	graphics::drawRect(width / 2, height - 40, width, 110, bg_br);

	//draw Score
	graphics::drawText(width / 2 - 75, 60, 30, "Score: " + std::to_string(gd->getScore()), std_br);

	//draw FPS
	if ((int)graphics::getGlobalTime() % 20 == 0) //change fps every 0.2 seconds
		new_fps = gd->fps;
	graphics::drawText(width / 75, 2 * width / 20, ((width + height) / 2) / 35, "FPS " + std::to_string(new_fps), std_br);

	//draw Level
	graphics::drawText(width / 75, 2 * width / 20 + 20, ((width + height) / 2) / 35, "Level " + std::to_string(-gd->curr_playing_level - 1), std_br);

	//draw Panels
	drawPlayerPanel(gd->playerLs->front(), 0);
	if (gd->playerLs->size() == 1)
		drawDestroyedPanel(width - 175);
	else
		drawPlayerPanel(gd->playerLs->back(), width - 175);
}
