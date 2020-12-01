#include "main.h"
#include "game_data.h"
// #include everything lmao.h
#include <iostream>

// global variables
graphics::Brush br;

// sgg functions
void update(float ms)
{
	game_data* gd = (game_data*)graphics::getUserData();

	gd->fps = (int)(1000.0f / ms);
	switch (gd->game_state)
	{
	case game_states::MENU: {
		if (graphics::getKeyState(graphics::scancode_t::SCANCODE_S))
		{
			gd->game_state = game_states::GAME;

			// ...

		}

		gd->game_state = ((game_states::OPTIONS * graphics::getKeyState(graphics::scancode_t::SCANCODE_O)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_O)));
		gd->game_state = ((game_states::CREDITS * graphics::getKeyState(graphics::scancode_t::SCANCODE_C)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_C)));
		gd->game_state = ((game_states::HELP * graphics::getKeyState(graphics::scancode_t::SCANCODE_H)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_H)));
		gd->game_state = ((game_states::EXIT * graphics::getKeyState(graphics::scancode_t::SCANCODE_E)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_E)));
		
		// ...

		break;
	}
	case game_states::GAME: {
		/*cars[i]->update(
			graphics::getKeyState(cars[i]->k_up),
			graphics::getKeyState(cars[i]->k_down),
			graphics::getKeyState(cars[i]->k_left),
			graphics::getKeyState(cars[i]->k_right),
			ms / 1000.0f,
			gd->t->get_unit_size()
		);*/

		// ...

		break;
	}
	case game_states::END: {
		gd->game_state = ((game_states::EXIT * graphics::getKeyState(graphics::scancode_t::SCANCODE_E)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_E)));

		// ...

		break;
	}
	case game_states::OPTIONS: {
		gd->game_state = ((game_states::MENU * graphics::getKeyState(graphics::scancode_t::SCANCODE_B)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_B)));
		
		// ...
		
		break;
	}

	case game_states::EXIT: {
		graphics::destroyWindow();
		exit(0);
	}
	case game_states::HELP:
	case game_states::CREDITS: {
		gd->game_state = ((game_states::MENU * graphics::getKeyState(graphics::scancode_t::SCANCODE_B)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_B)));
		break;
	}
	default: {
		std::cerr << "Invalid game state" << std::endl;
		gd->game_state = game_states::EXIT;
	}
	}
}

void draw()
{
	game_data* gd = (game_data*)graphics::getUserData();

	br.texture = "";
	graphics::resetPose();
	setColor(br, 'G');
	graphics::drawText(CANVAS_WIDTH / 150, 2 * CANVAS_HEIGHT / 20, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 35, "FPS: " + std::to_string(gd->fps), br);
	switch (gd->game_state)
	{
	case game_states::MENU: {
		setColor(br, new float[3]{ 0.0f, 0.0f, 1.0f });
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		graphics::drawText(CANVAS_WIDTH / 100, CANVAS_HEIGHT / 20, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 30, "<<<  Exit", br);
		graphics::drawText(73 * CANVAS_WIDTH / 100, CANVAS_HEIGHT / 20, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 30, "Press H for help", br);
		graphics::drawText(CANVAS_WIDTH / 4, CANVAS_HEIGHT / 4, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "Welcome!", br);
		graphics::drawText(CANVAS_WIDTH / 8, 3 * CANVAS_HEIGHT / 5, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "Press S to start!", br);
		graphics::drawText(CANVAS_WIDTH / 20, 3 * CANVAS_HEIGHT / 4, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "Press O for options!", br);
		graphics::drawText(CANVAS_WIDTH / 20, 6 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "Press C for credits!", br);

		// ...

		break;
	}
	case game_states::GAME: {

		// ...

		break;
	}
	case game_states::END: {
		setColor(br, new float[3]{ 0.0f, 0.0f, 1.0f });
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });

		// ...
		break;
	}
	case game_states::OPTIONS: {
		setColor(br, new float[3]{ 0.0f, 0.0f, 1.0f });
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });

		graphics::drawText(CANVAS_WIDTH / 100, CANVAS_HEIGHT / 20, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 30, "<<< go Back", br);

		// ...
		break;
	}
	case game_states::HELP: {
		setColor(br, new float[3]{ 0.0f, 0.0f, 1.0f });
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		graphics::drawText(CANVAS_WIDTH / 100, CANVAS_HEIGHT / 20, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 30, "<<<  go Back", br);
		graphics::drawText(CANVAS_WIDTH / 6, 1.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "Because mouse and text are", br);
		graphics::drawText(CANVAS_WIDTH / 6, 2.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "hard to combine, every input", br);
		graphics::drawText(CANVAS_WIDTH / 6, 3.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "is done with the keyboard :)", br);
		graphics::drawText(CANVAS_WIDTH / 6, 5.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "In most screens every single", br);
		graphics::drawText(CANVAS_WIDTH / 6, 6.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "uppercase letter relates to", br);
		graphics::drawText(CANVAS_WIDTH / 6, 7.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "an action hinted by its word", br);
		graphics::drawText(CANVAS_WIDTH / 6, 9.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "For example, 'go Back' hints", br);
		graphics::drawText(CANVAS_WIDTH / 6, 10.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "that pressing 'B' goes back", br);
		graphics::drawText(2 * CANVAS_WIDTH / 5, 12 * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "Try it!", br);
		break;
	}
	case game_states::EXIT: {
		break;
	}
	case game_states::CREDITS: {
		setColor(br, new float[3]{ 0.0f, 0.0f, 1.0f });
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });

		graphics::drawText(1 * CANVAS_WIDTH / 100, CANVAS_HEIGHT / 20, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 30, "<<<  go Back", br);
		graphics::drawText(40 * CANVAS_WIDTH / 100, 1 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Made by:", br);
		graphics::drawText(22 * CANVAS_WIDTH / 100, 2 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 12, "Dimitris Tsirmpas", br);
		graphics::drawText(45 * CANVAS_WIDTH / 100, 3 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 18, "and", br);
		graphics::drawText(25 * CANVAS_WIDTH / 100, 4 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 12, "Alex Mandelias", br);
		graphics::drawText(30 * CANVAS_WIDTH / 100, 5 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 18, "with the help of the", br);
		graphics::drawText(33 * CANVAS_WIDTH / 100, 6 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 12, "SGG library", br);
		break;
	}
	default: {
		std::cerr << "Invalid game state" << std::endl;
	}
	}
}

int main(int argc, char** argv)
{
	graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "1942ripoff");

	graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

	graphics::setDrawFunction(draw);
	graphics::setUpdateFunction(update);

	if (!graphics::setFont(font))
		std::cerr << "Unable to load font from: " << font << std::endl;

	initialize();

	graphics::startMessageLoop();

	return 0;
}

// function definitions
void initialize()
{
	game_data* gd = new game_data();

	graphics::setUserData(gd);

	// ...
}

// void reset() { ; }
// ...

// nothing to see below here
float get_canvas_width() { return CANVAS_WIDTH; }
float get_canvas_height() { return CANVAS_HEIGHT; }

void setColor(graphics::Brush& br, const float* rgb)
{
	br.fill_color[0] = rgb[0];
	br.fill_color[1] = rgb[1];
	br.fill_color[2] = rgb[2];
	br.outline_color[0] = rgb[0];
	br.outline_color[1] = rgb[1];
	br.outline_color[2] = rgb[2];
}

void setColor(graphics::Brush& br, char c)
{
	switch (c) {
	case 'R':
		setColor(br, new float[3]{ 1.0f, 0.0f, 0.0f });
		break;
	case 'G':
		setColor(br, new float[3]{ 0.0f, 1.0f, 0.0f });
		break;
	case 'B':
		setColor(br, new float[3]{ 0.0f, 0.0f, 1.0f });
		break;
	case 'P':
		setColor(br, new float[3]{ 1.0f, 0.0f, 1.0f });
		break;
	case 'Y':
		setColor(br, new float[3]{ 1.0f, 1.0f, 0.0f });
		break;
	case 'O':
		setColor(br, new float[3]{ 1.0f, 0.5f, 0.0f });
		break;
	case 'N':
		setColor(br, new float[3]{ 0.6f, 0.2f, 0.2f });
		break;
	case 'W':
		setColor(br, new float[3]{ 1.0f, 1.0f, 1.0f });
		break;
	case 'A':
		setColor(br, new float[3]{ 0.5f, 0.5f, 0.5f });
		break;
	case 'L':
		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		break;
	}
}
