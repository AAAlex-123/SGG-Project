#include "button.h"
#include "game_data.h"

Button::Button(GameData* gd, float x, float y, float size, std::string* icon, int targetGS)
	: Drawing(x, y, 0, 0, size, size, icon), gd(gd), targetGS(targetGS), clicked(false), ms(graphics::MouseState())
{}

void Button::update(float not_used_lmao)
{
	clicked = false;
	if (gd->game_state == targetGS)
	{
		graphics::getMouseState(ms);
		if (ms.button_left_pressed)
		{
			clicked = (((abs(mouse_x((float)ms.cur_pos_x) - x) <= width)) && (abs(mouse_y((float)ms.cur_pos_y) - y) <= height));
		}
	}
}

void Button::draw()
{
	if (gd->game_state == targetGS) {
		Drawing::draw();
	}
}

void Button::execute() const
{
	if (gd->game_state == targetGS && clicked)
	{
		execute_();
	}
}

// GameStateChangingButton
GameStateChangingButton::GameStateChangingButton(GameData* gd, float x, float y, float size, std::string* icon, int targetGS, int newGS)
	: Button(gd, x, y, size, icon, targetGS), newGS(newGS)
{}

void GameStateChangingButton::execute_() const
{
	gd->game_state = newGS;
}
