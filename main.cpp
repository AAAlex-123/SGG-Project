#include "globals.h"
#include "constants.h"
#include "GObjFactory.h"
#include "game_data.h"
#include "UI.h"
#include "Player.h"
#include <iostream>
#include <thread>

// uncomment this to disable threads
// #define no_threads

// global variables in main
graphics::Brush br;
graphics::Brush bg_br;
UI* ui = nullptr;
MUSIC curr_music;

#ifndef no_threads
std::thread updateThread;
std::thread collisionThread;

bool game_over = false;
bool paused = false;
bool terminate_all = false;
#endif

// ========== SGG FUNCTIONS ==========
void update(float ms_)
{
	// use a static variable to pass its value to the update thread once and have it update every cycle
	static float ms = 0;	// first initialize then assign
	ms = ms_;

	GameData* gd = reinterpret_cast<GameData*> (graphics::getUserData());

	// choose music and background with a finite state machine
	if (gd->game_state == GAME_STATE::GAME && curr_music == MUSIC::MENU_MUSIC)
	{
		graphics::playMusic(music_path + "battle_music.mp3", 0.5f);
		bg_br.texture = "";
		curr_music = MUSIC::BATTLE_MUSIC;
	}
	else if (gd->game_state == GAME_STATE::GAME_LOSE && curr_music == MUSIC::BATTLE_MUSIC)
	{
		graphics::playMusic(music_path + "defeat_music.mp3", 0.5f, false, 2000);
		bg_br.texture = image_path + "defeat.png";
		curr_music = MUSIC::LOSE_MUSIC;
	}
	else if (gd->game_state == GAME_STATE::GAME_WIN && curr_music == MUSIC::BATTLE_MUSIC)
	{
		graphics::playMusic(music_path + "victory_music.mp3", 0.5f, false, 2000);
		bg_br.texture = image_path + "win.png";
		curr_music = MUSIC::WIN_MUSIC;
	}
	else if (gd->game_state == GAME_STATE::MENU && (curr_music == MUSIC::WIN_MUSIC || curr_music == MUSIC::LOSE_MUSIC))
	{
		graphics::playMusic(music_path + "menu_music.mp3", 0.5f);
		bg_br.texture = image_path + "menu.png";
		curr_music = MUSIC::MENU_MUSIC;
	}

	gd->fps = (int)(1000.0f / ms);
	gd->update(ms, gd->buttonLs);
	gd->click_buttons();

	switch (gd->game_state)
	{
	case GAME_STATE::LOAD: {

		gd->elapsed += ms;

		if (gd->elapsed > (1 / (gd->sprites_per_second)) * 1000.0f)
		{
			gd->elapsed = 0.0f;
			++(gd->curr_img_index);
		}

		gd->game_state = (gd->curr_img_index == gd->image_names.size()) ? (GAME_STATE::LOAD_L) : (gd->game_state);

		break;
	}
	case GAME_STATE::LOAD_L: {

		gd->load_levels();
		gd->game_state = GAME_STATE::MENU;

		break;
	}
	case GAME_STATE::MENU: {

		if (graphics::getKeyState(key::SCANCODE_S) || graphics::getKeyState(key::SCANCODE_D))
		{
			gd->game_state = GAME_STATE::GAME;
			ui = new UI(gd);

			// === initialize level ===

			if (graphics::getKeyState(key::SCANCODE_S))
				gd->_playing_level_id = (gd->_selected_level_id == -1) ? (0) : (gd->_selected_level_id);

			else if (graphics::getKeyState(key::SCANCODE_D))
				gd->_playing_level_id = -10;

			gd->current_level = gd->levels[gd->_playing_level_id]->clone();

			// === generate players ===

			GObjFactory::setPlayerData(gd->playerLs);
			// Due to Factory constraints we are forced to upcast the player pointer.
			gd->playerLs->push_back(dynamic_cast<Player*>(GObjFactory::createEntity(GObjFactory::ENEMY::PLAYER, get_canvas_width() / 3.0f, get_canvas_height() * 0.7f, 0)));
			if (gd->isMultiplayer)
				gd->playerLs->push_back(dynamic_cast<Player*>(GObjFactory::createEntity(GObjFactory::ENEMY::PLAYER, 2 * get_canvas_width() / 3.0f, get_canvas_height() * 0.7f, 0)));

#ifndef no_threads
			// === start threads ===	(only the first time the game starts)
			if (!updateThread.joinable())
				updateThread = std::thread(updateAndSpawn, gd, &ms);
			if (!collisionThread.joinable())
				collisionThread = std::thread(checkAndFire, gd, &ms);

			// continue the infinite loop of the thread
			game_over = false;
#endif
		}

		break;
	}
	case GAME_STATE::ACHIEVEMENTS: {
		break;
	}
	case GAME_STATE::GAME: {
		// unpause here in case the game is unpause with a button, in which case no code can easily be executed
		paused = false;

		gd->game_state = graphics::getKeyState(key::SCANCODE_P) ? (GAME_STATE::PAUSE) : (gd->game_state);

		// level change logic
		if ((!(*gd->current_level)) && (gd->enemyLs->empty()) && (gd->enemyProjLs->empty()))
		{
			gd->level_transition_timer = gd->set_level_transition_timer();
			gd->game_state = GAME_STATE::LEVEL_TRANSITION;
		}

#ifdef no_threads
		//update
		gd->update(ms, gd->enemyLs);
		gd->update(ms, gd->enemyProjLs);
		gd->update(ms, gd->playerLs);
		gd->update(ms, gd->playerProjLs);
		gd->update(ms, gd->effectsLs);
		gd->update(ms, gd->powerupLs);

		gd->update_level(ms);
		gd->updateBackground(ms);

		//check collisions
		gd->checkCollisions(gd->enemyProjLs, gd->playerLs);
		gd->checkCollisions(gd->playerProjLs, gd->enemyLs);
		gd->checkCollisions(gd->enemyLs, gd->playerLs);
		gd->checkCollisions(gd->playerLs, gd->powerupLs);

		//fire
		gd->fire(gd->playerLs);
		gd->fire(gd->enemyLs);

		//spawn new enemies
		gd->spawn();
#endif

		// delete
			// these are kept seperate from the concurrent threads as they may change *all* their data during their execution
			// so a mutex wouldn't make sense
		gd->checkAndDelete(gd->enemyLs);
		gd->checkAndDelete(gd->enemyProjLs);
		gd->checkAndDelete(gd->playerLs);
		gd->checkAndDelete(gd->playerProjLs);
		gd->checkAndDelete(gd->effectsLs);
		gd->checkAndDelete(gd->powerupLs);

		break;
	}
	case GAME_STATE::LEVEL_TRANSITION: {

		// update only what is necessary during transitions
		gd->update(ms, gd->playerLs);
		gd->update(ms, gd->playerProjLs);
		gd->update(ms, gd->effectsLs);
		gd->update(ms, gd->powerupLs);
		gd->updateBackground(ms);

		gd->fire(gd->playerLs);

		gd->checkCollisions(gd->playerLs, gd->powerupLs);

		gd->checkAndDelete(gd->playerProjLs);
		gd->checkAndDelete(gd->effectsLs);
		gd->checkAndDelete(gd->powerupLs);

		// update timer
		gd->level_transition_timer -= (ms / 1000.0f);

		// if there isn't a next level, the player has won
		if (!gd->get_next_level())
		{
			gd->game_state = GAME_STATE::GAME_WIN;
		}
		else if (gd->level_transition_timer <= 0.0f)
		{
			gd->set_next_level();
			gd->game_state = GAME_STATE::GAME;
		}

		break;
	}
	case GAME_STATE::GAME_LOSE:
	case GAME_STATE::GAME_WIN: {

#ifndef no_threads
		game_over = true;
#endif 
		break;
	}
	case GAME_STATE::RESET: {

		GObjFactory::reset();
		gd->reset();
		gd->clearList(gd->playerLs);
		gd->clearList(gd->enemyLs);
		gd->clearList(gd->enemyProjLs);
		gd->clearList(gd->playerProjLs);
		gd->clearList(gd->effectsLs);

		gd->game_state = GAME_STATE::MENU;

		break;
	}
	case GAME_STATE::OPTIONS: {
		break;
	}
	case GAME_STATE::OP_LEVEL: {

		bool codes[10] = { graphics::getKeyState(key::SCANCODE_0), graphics::getKeyState(key::SCANCODE_1),
			graphics::getKeyState(key::SCANCODE_2), graphics::getKeyState(key::SCANCODE_3), graphics::getKeyState(key::SCANCODE_4),
			graphics::getKeyState(key::SCANCODE_5), graphics::getKeyState(key::SCANCODE_6), graphics::getKeyState(key::SCANCODE_7),
			graphics::getKeyState(key::SCANCODE_8), graphics::getKeyState(key::SCANCODE_9) };

		for (int i = 0; i < 10; i++)
			gd->_active_level_id = (i * (codes[i] && gd->levels[i])) + (gd->_active_level_id * !(codes[i] && gd->levels[i]));

		gd->_selected_level_id = (graphics::getKeyState(key::SCANCODE_S)) ? (gd->_active_level_id) : (gd->_selected_level_id);

		gd->_active_level_id = graphics::getKeyState(key::SCANCODE_D) ? -1 : gd->_active_level_id;
		gd->_selected_level_id = graphics::getKeyState(key::SCANCODE_D) ? -1 : gd->_selected_level_id;

		break;
	}
	case GAME_STATE::OP_PLAYER: {
		break;
	}
	case GAME_STATE::HELP: {
		break;
	}
	case GAME_STATE::EXIT: {
		std::terminate();
	}
	case GAME_STATE::PAUSE: {

		gd->game_state = (graphics::getKeyState(key::SCANCODE_U)) ? (GAME_STATE::GAME) : (gd->game_state);

		// pause threads; this is unpaused at the beginning of each update cycle in the GAMESTATE::GAME
		paused = true;

		break;
	}
	case GAME_STATE::CREDITS:
	case GAME_STATE::INVALID_STATE: {
		break;
	}
	default: {
		// this should never be executed, just a failsafe in case the programmers messed up
		gd->game_state = GAME_STATE::INVALID_STATE;
	}
	}
}

void draw()
{
	GameData* gd = reinterpret_cast<GameData*> (graphics::getUserData());
	graphics::resetPose();

	if (bg_br.texture != "") 	// if there is a background to draw
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, bg_br);

	switch (gd->game_state)
	{
	case GAME_STATE::LOAD: {

		if (gd->image_names.empty())
			return;
		std::string curr_image = asset_path + gd->image_names[gd->curr_img_index];

		setColor(br, 'W');
		br.texture = curr_image;
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_HEIGHT / 4, CANVAS_HEIGHT / 4, br);

		setColor(br, 'L');
		graphics::drawText(6 * CANVAS_WIDTH / 100, 9 * CANVAS_HEIGHT / 100, CANVAS_HEIGHT / 25, "Loading:   " + curr_image, br);

		break;
	}
	case GAME_STATE::LOAD_L: {

		setColor(br, 'L');
		graphics::drawText(6 * CANVAS_WIDTH / 100, 9 * CANVAS_HEIGHT / 100, CANVAS_HEIGHT / 25, "Loading levels...", br);

		break;
	}
	case GAME_STATE::MENU: {

		setColor(br, 'L');
		graphics::drawText(0.30f * CANVAS_WIDTH, 0.20f * CANVAS_HEIGHT, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "Welcome!", br);
		graphics::drawText(0.22f * CANVAS_WIDTH, 0.44f * CANVAS_HEIGHT, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 12, "Press S to start!", br);
		graphics::drawText(0.20f * CANVAS_WIDTH, 0.59f * CANVAS_HEIGHT, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 12, "Press D for demo!", br);

		break;
	}
	case GAME_STATE::ACHIEVEMENTS: {

		setColor(br, 'L');
		graphics::drawText(0.47f * CANVAS_WIDTH, 24, 20, "Unlocked Achievements: " + std::to_string(gd->getAchieved(gd).size()) + "/4", br);
		int i = 0;
		for (auto a : gd->getAchieved(gd))
		{
			br.texture = a->icon;
			setColor(br, 'W');
			graphics::drawRect(60, 75 + i * 100, 75, 75, br);
			setColor(br, 'L');
			graphics::drawText(0, 75 + i * 100 + 50, 10, a->name, br);
			++i;
		}

		setColor(br, 'L');
		graphics::drawText(0.03f * CANVAS_WIDTH, 0.92f * CANVAS_HEIGHT, 15, "Return here after playing a game to see if you have won any achievements!", br);
		graphics::drawText(0.03f * CANVAS_WIDTH, 0.97f * CANVAS_HEIGHT, 15, "Achievements are kept between games (but not if the program closes)", br);

		break;
	}
	case GAME_STATE::GAME: {

		gd->drawBackground(br);

		gd->draw(gd->enemyLs);
		gd->draw(gd->enemyProjLs);
		gd->draw(gd->playerLs);
		gd->draw(gd->playerProjLs);
		gd->draw(gd->effectsLs);
		gd->draw(gd->powerupLs);

		ui->draw();

		break;
	}
	case GAME_STATE::LEVEL_TRANSITION: {

		gd->drawBackground(br);
		gd->draw(gd->playerLs);
		gd->draw(gd->playerProjLs);
		gd->draw(gd->effectsLs);
		gd->draw(gd->powerupLs);
		ui->draw();

		// display timer
		graphics::resetPose();
		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		graphics::drawText(CANVAS_WIDTH * 0.35f, CANVAS_HEIGHT * 0.3f, 20,
			"Next Level in: " + std::to_string(gd->level_transition_timer).substr(0, 4), br);
		ui->draw();

		break;
	}
	case GAME_STATE::GAME_LOSE: {

		setColor(br, new float[3]{ 1.0f, 1.0f, 1.0f });
		graphics::drawText(CANVAS_WIDTH / 7, 2 * CANVAS_HEIGHT / 5, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "You lost!", br);
		graphics::drawText(CANVAS_WIDTH / 12, CANVAS_HEIGHT / 2, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "Final score: " + std::to_string(gd->getScore()), br);

		break;
	}
	case GAME_STATE::GAME_WIN: {

		setColor(br, new float[3]{ 1.0f, 1.0f, 1.0f });
		graphics::drawText(CANVAS_WIDTH / 7, 2 * CANVAS_HEIGHT / 5, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "You won!", br);
		graphics::drawText(CANVAS_WIDTH / 11, CANVAS_HEIGHT / 2, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "Final score: " + std::to_string(gd->getScore()), br);

		break;
	}
	case GAME_STATE::RESET: {
		break;
	}
	case GAME_STATE::OPTIONS: {

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		graphics::drawText(CANVAS_WIDTH / 3, 4.0f * CANVAS_HEIGHT / 10, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Select Level", br);
		graphics::drawText(CANVAS_WIDTH / 3, 9.0f * CANVAS_HEIGHT / 10, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Select Players", br);

		break;
	}
	case GAME_STATE::OP_LEVEL: {

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		gd->_active_level_id == -1
			? graphics::drawText(0.18f * CANVAS_WIDTH, 0.15f * CANVAS_HEIGHT, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 20, "Use the numbers to select a track", br)
			: graphics::drawText(0.05f * CANVAS_WIDTH, 0.15f * CANVAS_HEIGHT, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 20, "Select (S) this level or Deselect (D) all levels", br);

		int display_height_index = -1;	// -1 because ++ prefix is cooler
		for (int i = 0; i < 10; ++i)
		{
			if (gd->levels[i])
			{
				if (gd->_selected_level_id == gd->levels[i]->id())
					setColor(br, 'O');
				else if (gd->_active_level_id == gd->levels[i]->id())
					setColor(br, new float[3]{ 1.0f, 1.0f, 1.0f });
				else
					setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
				graphics::drawText(CANVAS_WIDTH / 20, (2.5f + 0.8f * ++display_height_index) * CANVAS_HEIGHT / 10, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 24, gd->levels[i]->info(), br);
			}
		}

		break;
	}
	case GAME_STATE::OP_PLAYER: {

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		graphics::drawText(5 * CANVAS_WIDTH / 20, CANVAS_HEIGHT / 8, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Choose gamemode:", br);
		graphics::drawText(CANVAS_WIDTH / 8, 2 * CANVAS_HEIGHT / 3, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Singleplayer", br);
		graphics::drawText(4.5f * CANVAS_WIDTH / 8, 2 * CANVAS_HEIGHT / 3, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Multiplayer", br);

		graphics::drawText(4 * CANVAS_WIDTH / 32, 5 * CANVAS_HEIGHT / 6, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15,
			gd->isMultiplayer ? "Multiplayer mode selected!" : "Singleplayer mode selected!", br);

		break;
	}
	case GAME_STATE::HELP: {

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		graphics::drawText(CANVAS_WIDTH / 10, 2.0f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "Player 1:", br);
		graphics::drawText(CANVAS_WIDTH / 10, 2.7f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 28, "WASD to move, X to fire, Q/E to spin", br);
		graphics::drawText(CANVAS_WIDTH / 10, 5.0f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "Player 2:", br);
		graphics::drawText(CANVAS_WIDTH / 10, 5.7f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 28, "Arrow-keys to move, SPACE to fire, '.'/',' to spin", br);
		graphics::drawText(CANVAS_WIDTH / 10, 7.0f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 19, "Pick up powerups by running into them.", br);
		graphics::drawText(CANVAS_WIDTH / 10, 8.2f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "Gain points by killing enemies.", br);

		setColor(br, new float[3]{ 1.0f, 1.0f, 1.0f });
		br.outline_opacity = 0.f;
		br.texture = std::string(image_path + "player1.png");
		graphics::drawRect(CANVAS_WIDTH / 10, CANVAS_HEIGHT - 80, 40, 80, br);
		br.texture = std::string(image_path + "player2.png");

		graphics::drawRect(CANVAS_WIDTH - (CANVAS_WIDTH / 10), CANVAS_HEIGHT - 80, 40, 80, br);

		break;
	}
	case GAME_STATE::EXIT: {
		break;
	}
	case GAME_STATE::PAUSE: {

		setColor(br, new float[3]{ 0.5f, 0.7f, 0.9f });
		br.texture = "";
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		setColor(br, 'L');
		graphics::drawText(CANVAS_WIDTH / 3, 2.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "Game paused...", br);

		graphics::drawText(6 * CANVAS_WIDTH / 7, 2 * CANVAS_WIDTH / 20 + 20, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 35, "Unpause (U)", br);

		break;
	}
	case GAME_STATE::CREDITS: {

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		graphics::drawText(0.40f * CANVAS_WIDTH, 0.15f * CANVAS_HEIGHT, 29, "Made by:", br);
		graphics::drawText(0.13f * CANVAS_WIDTH, 2 * CANVAS_HEIGHT / 7, 37.5f, "Dimitris Tsirmpas and", br);
		graphics::drawText(0.26f * CANVAS_WIDTH, 3 * CANVAS_HEIGHT / 7, 37.5f, "Alex Mandelias", br);
		graphics::drawText(0.30f * CANVAS_WIDTH, 4 * CANVAS_HEIGHT / 7, 25, "with the help of the", br);
		graphics::drawText(0.32f * CANVAS_WIDTH, 5 * CANVAS_HEIGHT / 7, 37.5f, "SGG Library", br);
		graphics::drawText(0.26f * CANVAS_WIDTH, 0.90f * CANVAS_HEIGHT, 25.f, "Asset credits can be found", br);
		graphics::drawText(0.31f * CANVAS_WIDTH, 0.97f * CANVAS_HEIGHT, 25.f, "in the assets folder.", br);

		break;
	}
	case GAME_STATE::INVALID_STATE: {

		setColor(br, new float[3]{ 1.0f, 0.0f, 0.0f });
		graphics::drawText(20 * CANVAS_WIDTH / 100, 2 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "Invalid game state", br);
		graphics::drawText(10 * CANVAS_WIDTH / 100, 4 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Please press the back button", br);
		graphics::drawText(10 * CANVAS_WIDTH / 100, 4 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "and inform the developers", br);

		break;
	}
	default: {
		// this should never be executed, just a failsafe in case the programmers messed up
		gd->game_state = GAME_STATE::INVALID_STATE;
	}
	}
	// buttons should be drawn above everything else
	gd->draw(gd->buttonLs);
}

void resize(int new_w, int new_h)
{
	WINDOW_WIDTH = new_w;
	WINDOW_HEIGHT = new_h;
	w2c = (WINDOW_HEIGHT - CANVAS_HEIGHT) < (WINDOW_WIDTH - CANVAS_WIDTH)
		? CANVAS_HEIGHT / WINDOW_HEIGHT
		: CANVAS_WIDTH / WINDOW_WIDTH;
	c2w = 1.0f / w2c;
}

int main()
{
	graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "1942ripoff");
	//graphics::setFullScreen(true);
	std::set_terminate(close);

	graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

	graphics::setUpdateFunction(update);
	graphics::setDrawFunction(draw);
	graphics::setResizeFunction(resize);

	initialize();

	graphics::startMessageLoop();

	// destroyWindow is called in terminate(), whenever the game closes

	return 0;
}

// function definitions
void initialize()
{
	GameData* gd = new GameData();

	graphics::setUserData((void*)gd);

	if (!graphics::setFont(font_file))
		std::cerr << "Unable to load font from: " << font_file << std::endl;

	if (!load_images_from_file(image_path, icon_path))
		std::cerr << "Unable to load images or icons from: " << image_path << " and " << icon_path << std::endl;

	graphics::playMusic(music_path + "menu_music.mp3", 0.5f, true);
	curr_music = MUSIC::MENU_MUSIC;
	bg_br.texture = image_path + "menu.png";
}

// Standard exiting operation to be called whenever the window closes or the game ends
void close()
{
	if (ui != nullptr)	//if game had started
	{
		delete ui;
#ifndef no_threads
		terminate_all = true;
		updateThread.join();
		collisionThread.join();
#endif
	}
	graphics::destroyWindow();
	exit(0);
}

inline float get_canvas_width() { return CANVAS_WIDTH; }
inline float get_canvas_height() { return CANVAS_HEIGHT; }

float mouse_x(float mx) { return (mx - ((WINDOW_WIDTH - (CANVAS_WIDTH * c2w)) / 2)) * w2c; }
float mouse_y(float my) { return (my - ((WINDOW_HEIGHT - (CANVAS_HEIGHT * c2w)) / 2)) * w2c; }

#ifndef no_thread
/*	Note on thread implementation:
	The main thread, after executing the code of the update(float) method, executes
	code from the library which is responsible for almost all of the processing time.
	The other two threads execute code that doesn't interact with the library at all
	therefore their execution time is effectively 0, compared to that of the main thread.
	As a result these two threads need to wait for approximately the time between two
	update cycles before executing their code again.
	Although there is some variance regarding the delta time of two update cycles and
	the threads wait for the delta time of the previous update cycle, on the long run
	this doesn't make a difference
*/

void updateAndSpawn(GameData* const starting_gd, float* ms)
{
	bool gd_changed = false;
	GameData* gd = starting_gd;

	while (!terminate_all)
	{
		if (!game_over && !paused)
		{
			gd->spawn();
			gd->update(*ms, gd->enemyLs);
			gd->update(*ms, gd->enemyProjLs);
			gd->update(*ms, gd->playerLs);
			gd->update(*ms, gd->playerProjLs);
			gd->update(*ms, gd->effectsLs);
			gd->update(*ms, gd->powerupLs);

			gd->update_level(*ms);
			gd->updateBackground(*ms);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds((int)*ms));
	}
}

void checkAndFire(GameData* const starting_gd, float* ms)
{
	bool gd_changed = false;
	GameData* gd = starting_gd;

	while (!terminate_all)
	{
		if (!game_over && !paused)
		{
			gd->checkCollisions(gd->enemyProjLs, gd->playerLs);
			gd->checkCollisions(gd->playerProjLs, gd->enemyLs);
			gd->checkCollisions(gd->enemyLs, gd->playerLs);
			gd->checkCollisions(gd->playerLs, gd->powerupLs);
			gd->fire(gd->playerLs);
			gd->fire(gd->enemyLs);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds((int)*ms));
	}
}
#endif
