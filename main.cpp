#include "graphics.h"
#include "scancodes.h"
#include "globals.h"
#include "constants.h"
#include "game_data.h"
#include "drawing.h"
#include "visual_effect.h"
#include "entity.h"
#include "GObjFactory.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <regex>

// global variables in main
graphics::Brush br;

// TEST
std::vector<GameObject*> govec;

Keyset wasdqex(key::SCANCODE_W, key::SCANCODE_S, key::SCANCODE_A, key::SCANCODE_D, key::SCANCODE_Q, key::SCANCODE_E, key::SCANCODE_X);
Keyset tfghryb(key::SCANCODE_T, key::SCANCODE_G, key::SCANCODE_F, key::SCANCODE_H, key::SCANCODE_R, key::SCANCODE_Y, key::SCANCODE_B);

VisualEffect ve(600.0f, 400.0f, 0.0f, 0.0f, 50.0f,
	new std::string[7] {
		"assets\\expl1.png", "assets\\expl2.png",
		"assets\\expl3.png", "assets\\expl4.png",
		"assets\\expl5.png", "assets\\expl6.png",
		"assets\\expl7.png",
	}, 7, 20.0f, 10.f);

Entity eaccel = GObjFactory::createEntity(GObjFactory::ENEMY_1, 200.0f, 250.0f, -PI / 2.0f);
Entity erotate = GObjFactory::createEntity(GObjFactory::ENEMY_2, 500.0f, 250.0f, 0);
Entity enormal = GObjFactory::createEntity(GObjFactory::ENEMY_3, 800.0f, 250.0f, PI / 2.0f);
Entity eplayer = GObjFactory::createEntity(GObjFactory::PLAYER, 1000.0f, 250.0f, PI / 2.0f, PI/4.0f, wasdqex);
// END TEST

// sgg functions
void update(float ms)
{
	game_data* gd = (game_data*)graphics::getUserData();

	gd->fps = (int)(1000.0f / ms);
	switch (gd->game_state)
	{
	case game_states::TEST: {
		
		gd->game_state = ((game_states::MENU * graphics::getKeyState(graphics::scancode_t::SCANCODE_B)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_B)));

		ve.update(ms);

		for (int i = 0; i < govec.size(); ++i)
			govec[i]->update(ms);

		if (eplayer.hasFired())
			govec.push_back(&eplayer.getProjectile());

		break;
	}
	case game_states::LOAD: {
		gd->el += ms;

		if (gd->el > (1/(gd->sps)) * 1000.0f)
		{
			gd->el = 0.0f;

			if (++(gd->curr_img) == gd->images.size())
				gd->game_state = game_states::MENU;
		}

		break;
	}
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
		gd->game_state = ((game_states::TEST * graphics::getKeyState(graphics::scancode_t::SCANCODE_T)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_T)));

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
	case game_states::TEST: {
		ve.draw();
		for (int i = 0; i < govec.size(); ++i)
			govec[i]->draw();
		break;
	}
	case game_states::LOAD: {
		setColor(br, 'W');

		std::string curr_image = image_path + gd->images[gd->curr_img];

		br.texture = curr_image;
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_HEIGHT / 4, CANVAS_HEIGHT / 4, br);

		br.texture = "";
		graphics::drawText(CANVAS_WIDTH / 50, 4 * CANVAS_HEIGHT / 5, CANVAS_HEIGHT / 8, "Loading:   " + curr_image, br);
		
		break;
	}
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

	initialize();

	graphics::startMessageLoop();

	return 0;
}

// function definitions
void initialize()
{
	game_data* gd = new game_data();
	gd->game_state = game_states::LOAD;

	graphics::setUserData(gd);

	// load stuff
	if (!graphics::setFont(font))
		std::cerr << "Unable to load font from: " << font << std::endl;

	if (!load_images_from_file(image_path))
		std::cerr << "Unable to load assets from: " << image_path << std::endl;

	// these should not be here, they should be initialized from file
	govec.push_back(&eaccel);
	govec.push_back(&erotate);
	govec.push_back(&enormal);
	govec.push_back(&eplayer);

	// ...
}

// true == success
bool load_images_from_file(const std::string& image_path)
{
	game_data* gd = (game_data*)graphics::getUserData();

	std::string temp_file_name = "_mytemp.txt";

	// create temp file and stream to that temp file
	system(("dir " + image_path + " > " + temp_file_name).c_str());
	std::ifstream in(temp_file_name);

	if (!in)
	{
		std::cerr << "Error opening file " << temp_file_name << std::endl;
		return false;
	}

	// get all assets from file
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	std::smatch match;
	std::regex r("(\\w+\\.png)");

	while (std::regex_search(contents, match, r))
	{
		gd->images.push_back(match[1].str());
		contents = match.suffix();
	}

	// close stream, delete file
	in.close();
	remove(temp_file_name.c_str());

	return true;
}

// nothing to see below here
float canvas_width() { return CANVAS_WIDTH; }
float canvas_height() { return CANVAS_HEIGHT; }
