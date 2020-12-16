#include "graphics.h"
#include "constants.h"
#include "GObjFactory.h"
#include "game_data.h"
#include <iostream>
#include <vector> //used in line 9+, debug

// global variables in main
graphics::Brush br;

// TEST
std::vector<GameObject*> govec;

Keyset wasdqex(key::SCANCODE_W, key::SCANCODE_S, key::SCANCODE_A, key::SCANCODE_D, key::SCANCODE_Q, key::SCANCODE_E, key::SCANCODE_X);
Keyset tfghryb(key::SCANCODE_T, key::SCANCODE_G, key::SCANCODE_F, key::SCANCODE_H, key::SCANCODE_R, key::SCANCODE_Y, key::SCANCODE_B);
Entity* eplayer = GObjFactory::createEntity(GObjFactory::PLAYER, 1000.0f, 250.0f, PI / 2.0f, PI / 4.0f, wasdqex);

VisualEffect ve(600.0f, 400.0f, 0.0f, 0.0f, 50.0f,
	new std::string[7] {
		"assets\\expl1.png", "assets\\expl2.png",
		"assets\\expl3.png", "assets\\expl4.png",
		"assets\\expl5.png", "assets\\expl6.png",
		"assets\\expl7.png",
	}, 7, 20.0f, 10.f);

Entity* eaccel = GObjFactory::createEntity(GObjFactory::ENEMY_1, 200.0f, 250.0f, -PI / 2.0f);
Entity* erotate = GObjFactory::createEntity(GObjFactory::ENEMY_2, 500.0f, 250.0f, 0);
Entity* enormal = GObjFactory::createEntity(GObjFactory::ENEMY_3, 800.0f, 250.0f, PI / 2.0f);
// END TEST

// sgg functions
void update(float ms)
{
	GameData* gd = reinterpret_cast<GameData*> (graphics::getUserData());

	gd->fps = (int)(1000.0f / ms);
	switch (gd->game_state)
	{
	case game_states::TEST: {
		
		// custom entity spawning

		gd->curr_selected_level = 0;

		gd->playerLs->push_back(eplayer);

		// ...


		gd->game_state = game_states::GAME;
		
		break;
	}
	case game_states::LOAD: {
		gd->el += ms;

		if (gd->el > (1 / (gd->sps)) * 1000.0f)
		{
			gd->el = 0.0f;
			++(gd->curr_img);
		}

		gd->game_state = ((game_states::MENU * (gd->curr_img == gd->images.size())) + (gd->game_state * !(gd->curr_img == gd->images.size())));

		break;
	}
	case game_states::MENU: {
		if (graphics::getKeyState(graphics::scancode_t::SCANCODE_S))
		{
			gd->game_state = game_states::GAME;

			// stuff to do when game starts ...

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
		
	//update
		gd->update(ms, gd->enemyLs);
		gd->update(ms, gd->enemyProjLs);
		gd->update(ms, gd->playerLs);
		gd->update(ms, gd->playerProjLs);
		gd->update(ms, gd->effectsLs);
		
		gd->updateLevel(ms);
		

	//check collisions
		gd->checkCollisions(gd->enemyProjLs, gd->playerLs);
		gd->checkCollisions(gd->playerProjLs, gd->enemyLs);
		gd->checkCollisions(gd->enemyLs, gd->playerLs);

	//fire
		gd->fire(gd->playerLs);
		gd->fire(gd->enemyLs);

	//spawn
		gd->spawn();
		

	//delete
		gd->checkAndDelete(gd->enemyLs);
		gd->checkAndDelete(gd->enemyProjLs);
		gd->checkAndDelete(gd->playerLs);
		gd->checkAndDelete(gd->playerProjLs);
		gd->checkAndDelete(gd->effectsLs);

		break;
	}
	case game_states::END: {
		gd->game_state = ((game_states::EXIT * graphics::getKeyState(graphics::scancode_t::SCANCODE_E)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_E)));

		// ...

		break;
	}
	case game_states::OPTIONS: {
		gd->game_state = ((game_states::MENU * graphics::getKeyState(graphics::scancode_t::SCANCODE_B)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_B)));

		gd->game_state = ((game_states::OP_LEVEL * graphics::getKeyState(graphics::scancode_t::SCANCODE_L)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_L)));

		// ...

		break;
	}
	case game_states::OP_LEVEL: {
		gd->game_state = ((game_states::OPTIONS * graphics::getKeyState(graphics::scancode_t::SCANCODE_B)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_B)));


		gd->curr_active_level = (0 * graphics::getKeyState(graphics::scancode_t::SCANCODE_0)) + (gd->curr_active_level * !graphics::getKeyState(graphics::scancode_t::SCANCODE_0));
		gd->curr_active_level = (1 * graphics::getKeyState(graphics::scancode_t::SCANCODE_1)) + (gd->curr_active_level * !graphics::getKeyState(graphics::scancode_t::SCANCODE_1));
		gd->curr_active_level = (2 * graphics::getKeyState(graphics::scancode_t::SCANCODE_2)) + (gd->curr_active_level * !graphics::getKeyState(graphics::scancode_t::SCANCODE_2));
		gd->curr_active_level = (3 * graphics::getKeyState(graphics::scancode_t::SCANCODE_3)) + (gd->curr_active_level * !graphics::getKeyState(graphics::scancode_t::SCANCODE_3));
		gd->curr_active_level = (4 * graphics::getKeyState(graphics::scancode_t::SCANCODE_4)) + (gd->curr_active_level * !graphics::getKeyState(graphics::scancode_t::SCANCODE_4));
		gd->curr_active_level = (5 * graphics::getKeyState(graphics::scancode_t::SCANCODE_5)) + (gd->curr_active_level * !graphics::getKeyState(graphics::scancode_t::SCANCODE_5));
		gd->curr_active_level = (6 * graphics::getKeyState(graphics::scancode_t::SCANCODE_6)) + (gd->curr_active_level * !graphics::getKeyState(graphics::scancode_t::SCANCODE_6));
		gd->curr_active_level = (7 * graphics::getKeyState(graphics::scancode_t::SCANCODE_7)) + (gd->curr_active_level * !graphics::getKeyState(graphics::scancode_t::SCANCODE_7));
		gd->curr_active_level = (8 * graphics::getKeyState(graphics::scancode_t::SCANCODE_8)) + (gd->curr_active_level * !graphics::getKeyState(graphics::scancode_t::SCANCODE_8));
		gd->curr_active_level = (9 * graphics::getKeyState(graphics::scancode_t::SCANCODE_9)) + (gd->curr_active_level * !graphics::getKeyState(graphics::scancode_t::SCANCODE_9));

		gd->curr_selected_level = (gd->curr_active_level * graphics::getKeyState(graphics::scancode_t::SCANCODE_S)) + (gd->curr_selected_level * !graphics::getKeyState(graphics::scancode_t::SCANCODE_S));


		gd->curr_active_level = (-1 * graphics::getKeyState(graphics::scancode_t::SCANCODE_D)) + (gd->curr_active_level * !graphics::getKeyState(graphics::scancode_t::SCANCODE_D));
		gd->curr_selected_level = (-1 * graphics::getKeyState(graphics::scancode_t::SCANCODE_D)) + (gd->curr_selected_level * !graphics::getKeyState(graphics::scancode_t::SCANCODE_D));

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
		;
	}
	}
}

void draw()
{
	GameData* gd = reinterpret_cast<GameData*> (graphics::getUserData());

	br.texture = "";
	graphics::resetPose();
	setColor(br, 'G');
	graphics::drawText(CANVAS_WIDTH / 150, 2 * CANVAS_HEIGHT / 20, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 35, "FPS: " + std::to_string(gd->fps), br);
	switch (gd->game_state)
	{
	case game_states::TEST: {
		
		// ...
		
		break;
	}
	case game_states::LOAD: {
		setColor(br, 'W');

		if (gd->images.empty())
			return;
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

		gd->draw(gd->enemyLs);
		gd->draw(gd->enemyProjLs);
		gd->draw(gd->playerLs);
		gd->draw(gd->playerProjLs);
		gd->draw(gd->effectsLs);

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
		graphics::drawText(CANVAS_WIDTH / 3, CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Level select", br);

		// ...
		break;
	}
	case game_states::OP_LEVEL: {
		setColor(br, new float[3]{ 0.0f, 0.0f, 1.0f });
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });

		graphics::drawText(CANVAS_WIDTH / 100, CANVAS_HEIGHT / 20, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 30, "<<< go Back", br);

		gd->curr_active_level == -1
			? graphics::drawText(CANVAS_WIDTH / 11, 1.7f * CANVAS_HEIGHT / 10, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 20, "use  the  numbers  to  select  a  track", br)
			: graphics::drawText(CANVAS_WIDTH / 8, 1.7f * CANVAS_HEIGHT / 10, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 30, "Select  this  track  or  Deselect  all  tracks", br);

		for (int i = 0; i < gd->levels.size(); ++i)
		{
			if (gd->curr_selected_level == gd->levels[i]->id())
				setColor(br, 'O');
			else if (gd->curr_active_level == gd->levels[i]->id())
				setColor(br, new float[3]{ 1.0f, 1.0f, 1.0f });
			else
				setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
			graphics::drawText(CANVAS_WIDTH / 20, (2.5f + 0.5f * i) * CANVAS_HEIGHT / 10, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 28, gd->levels[i]->info(), br);
		}

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
		graphics::drawText(40 * CANVAS_WIDTH / 100, 1 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Invalid game state", br);
		graphics::drawText(20 * CANVAS_WIDTH / 100, 1 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Please exit and inform the develompers", br);
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
	GameData* gd = new GameData();
	gd->game_state = game_states::MENU;

	graphics::setUserData((void*)gd);

	// load stuff
	if (!graphics::setFont(font))
		std::cerr << "Unable to load font from: " << font << std::endl;

	if (!load_images_from_file(image_path))
		std::cerr << "Unable to load images from: " << image_path << std::endl;

	// ...
}

// nothing to see below here
float get_canvas_width() { return CANVAS_WIDTH; }
float get_canvas_height() { return CANVAS_HEIGHT; }
