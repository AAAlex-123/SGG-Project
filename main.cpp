#include "globals.h"
#include "constants.h"
#include "GObjFactory.h"
#include "game_data.h"
#include "UI.h"
#include <iostream>
#include <thread>

//#define no_threads

// global variables in main
graphics::Brush br;
graphics::Brush bg_br;
UI* ui = nullptr;

int curr_music;
const int MENU_MUSIC = 1;
const int BATTLE_MUSIC = 2;
const int LOSE_MUSIC = 3;
const int WIN_MUSIC = 4;

#ifndef no_threads
std::thread updateThread;
std::thread collisionThread;

bool th_1_start = false;
bool th_2_start = false;
bool th_1_done = false;
bool th_2_done = false;
bool game_over = false;
bool terminate_all = false;

float * const global_ms = new float;

//Note: The thread decisions below could have been implemented with <condition_variable> ( e.g cv.wait(), cv.notify_all())
//but it's probably overkill for something as simple as starting and stopping 2 threads

//thread functions
void updateAndSpawn(GameData* starting_gd, float* const ms) {
	bool gd_changed = false;
	GameData* gd = starting_gd;

	while (true) {
		while (!th_1_start) {
			if (game_over)
				gd_changed = true;
			if (terminate_all) //putting this condition on the while loop itself doesn't seem to work for some reason
				return;
		}



		if (!game_over) {

			if (gd_changed) {
				gd = reinterpret_cast<GameData*>(graphics::getUserData());
				gd_changed = false;
			}

			gd->spawn();
			gd->update(*ms, gd->enemyLs);
			gd->update(*ms, gd->enemyProjLs);
			gd->update(*ms, gd->playerLs);
			gd->update(*ms, gd->playerProjLs);
			gd->update(*ms, gd->effectsLs);
			gd->update(*ms, gd->powerupLs);

			gd->update_level(*ms);
			gd->updateBackground(*ms);

			th_1_done = true;
			th_1_start = false;
		}
	}
}

void checkAndFire(GameData* starting_gd) {
	bool gd_changed = false;
	GameData* gd = starting_gd;

	while (true) {
		while (!th_2_start) {
			if (game_over)
				gd_changed = true;
			if (terminate_all)
				return;
		}

		if (!game_over) {

			if (gd_changed) {
				gd = reinterpret_cast<GameData*>(graphics::getUserData());
				gd_changed = false;
			}

			gd->checkCollisions(gd->enemyProjLs, gd->playerLs);
			gd->checkCollisions(gd->playerProjLs, gd->enemyLs);
			gd->checkCollisions(gd->enemyLs, gd->playerLs);
			gd->checkCollisions(gd->playerLs, gd->powerupLs);
			gd->fire(gd->playerLs);
			gd->fire(gd->enemyLs);

			th_2_done = true;
			th_2_start = false;
		}
	}
}
#endif

//Standard exiting operation to be called whenever the window closes or the game ends
void close() {
	if (ui != nullptr) { //if game had started
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


//========== SGG FUNCTIONS ===============
void update(float ms)
{
	GameData* gd = reinterpret_cast<GameData*> (graphics::getUserData());
	//std::cout << "curr gs: " << gd->game_state << std::endl;
#ifndef no_threads
	* global_ms = ms;
#endif // !nothreads

	//choose music and background
	if (gd->game_state == game_states::GAME && curr_music == MENU_MUSIC) {
		graphics::playMusic(music_path + "battle_music.mp3", 0.5f);
		bg_br.texture = "";
		curr_music = BATTLE_MUSIC;
	}
	else if (gd->game_state == game_states::GAME_LOSE && curr_music == BATTLE_MUSIC) {
		graphics::playMusic(music_path + "defeat_music.mp3", 0.5f, false, 2000);
		bg_br.texture = image_path + "defeat.png";
		curr_music = LOSE_MUSIC;
	}
	else if (gd->game_state == game_states::GAME_WIN && curr_music == BATTLE_MUSIC) {
		graphics::playMusic(music_path + "victory_music.mp3", 0.5f, false, 2000);
		bg_br.texture = image_path + "win.png";
		curr_music = WIN_MUSIC;
	}
	else if (gd->game_state == game_states::MENU && (curr_music == WIN_MUSIC || curr_music == LOSE_MUSIC)) { // if (game_states::MENU && (curr_music == WIN_MUSIC || curr_music == LOSE_MUSIC))
		graphics::playMusic(music_path + "menu_music.mp3", 0.5f);
		bg_br.texture = image_path + "menu.png";
		curr_music = MENU_MUSIC;
	}

	gd->fps = (int)(1000.0f / ms);
	gd->update(ms, gd->buttonLs);
	gd->click_buttons();

	switch (gd->game_state)
	{
	case game_states::LOAD: {
		gd->elapsed += ms;

		if (gd->elapsed > (1 / (gd->sprites_per_second)) * 1000.0f)
		{
			gd->elapsed = 0.0f;
			++(gd->curr_img_index);
		}

		gd->game_state = ((game_states::LOAD_L * (gd->curr_img_index == gd->image_names.size())) + (gd->game_state * !(gd->curr_img_index == gd->image_names.size())));

		break;
	}
	case game_states::LOAD_L: {
		gd->load_levels();
		gd->game_state = game_states::MENU;
		break;
	}
	case game_states::MENU: {
		if (graphics::getKeyState(graphics::scancode_t::SCANCODE_S) || graphics::getKeyState(graphics::scancode_t::SCANCODE_D))
		{
			gd->game_state = game_states::GAME;
			gd->_playing_level_id = gd->_selected_level_id == -1 ? 0 : gd->_selected_level_id;
			if (graphics::getKeyState(graphics::scancode_t::SCANCODE_D))
				gd->_playing_level_id = -10;

			gd->current_level = gd->levels[gd->_playing_level_id]->clone();

			GObjFactory::setPlayerData(gd->playerLs);
			//Due to Factory constraints we are forced to upcast then downcast the player pointer.
			gd->playerLs->push_back(dynamic_cast<Player*>(GObjFactory::createEntity(GObjFactory::PLAYER, get_canvas_width() / 3.0f, get_canvas_height() * 0.7f, 0)));
			if (gd->isMultiplayer)
				gd->playerLs->push_back(dynamic_cast<Player*>(GObjFactory::createEntity(GObjFactory::PLAYER, 2 * get_canvas_width() / 3.0f, get_canvas_height() * 0.7f, 0)));

#ifndef no_threads
			// start threads only the first time the game starts
			if (!updateThread.joinable())
				updateThread = std::thread(updateAndSpawn, gd, global_ms);
			if (!collisionThread.joinable())
				collisionThread = std::thread(checkAndFire, gd);

			// continue the infinite loop of the thread
			game_over = false;
#endif
			ui = new UI(gd);
		}

		break;
	}
	case game_states::ACHIEVEMENTS: {
		break;
	}
	case game_states::GAME: {

		// yes there is a button to pause but some prefer to use the keyboard for easier access
		gd->game_state = ((game_states::PAUSE * graphics::getKeyState(graphics::scancode_t::SCANCODE_P)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_P)));

		// level change logic
		if ((!(*gd->current_level)) && (gd->enemyLs->empty()) && (gd->enemyProjLs->empty()))
		{
			gd->level_transition_timer = gd->set_level_transition_timer();
			gd->game_state = game_states::LEVEL_TRANSITION;
		}

#ifndef no_threads
		//start threads
		th_1_start = true;
		th_2_start = true;

		//wait for threads to stop
		while (!(th_1_done && th_2_done))
			;

		//reset threads for next cycle
		th_1_done = false;
		th_2_done = false;
#endif
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

	//delete
		//these are kept seperate from the concurrent threads as they may change *all* their data during their execution
		//so a mutex wouldn't make sense
		gd->checkAndDelete(gd->enemyLs);
		gd->checkAndDelete(gd->enemyProjLs);
		gd->checkAndDelete(gd->playerLs);
		gd->checkAndDelete(gd->playerProjLs);
		gd->checkAndDelete(gd->effectsLs);
		gd->checkAndDelete(gd->powerupLs);

		break;
	}
	case game_states::LEVEL_TRANSITION: {

		// do some updating while waiting for next level
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
			gd->game_state = game_states::GAME_WIN;
		}
		else if (gd->level_transition_timer <= 0.0f)
		{
			gd->set_next_level();
			gd->game_state = game_states::GAME;
		}
		break;
	}
	case game_states::GAME_LOSE:
	case game_states::GAME_WIN: {
#ifndef no_threads
		//stop threads until a new game has started
		th_1_start = false;
		th_2_start = false;
		game_over = true;
#endif 
		break;
	}
	case game_states::RESET: {
		GObjFactory::reset();

		gd->reset();

		gd->game_state = game_states::MENU;
		break;
	}
	case game_states::OPTIONS: {
		break;
	}
	case game_states::OP_LEVEL: {

		bool codes[10] = { graphics::getKeyState(graphics::scancode_t::SCANCODE_0) ,graphics::getKeyState(graphics::scancode_t::SCANCODE_1) ,graphics::getKeyState(graphics::scancode_t::SCANCODE_2) ,
		graphics::getKeyState(graphics::scancode_t::SCANCODE_3) ,graphics::getKeyState(graphics::scancode_t::SCANCODE_4) ,graphics::getKeyState(graphics::scancode_t::SCANCODE_5),
		graphics::getKeyState(graphics::scancode_t::SCANCODE_6) ,graphics::getKeyState(graphics::scancode_t::SCANCODE_7) ,graphics::getKeyState(graphics::scancode_t::SCANCODE_8) ,
		graphics::getKeyState(graphics::scancode_t::SCANCODE_9) };

		for (int i = 0; i < 10; i++)
			gd->_active_level_id = (i * (codes[i] && gd->levels[i])) + (gd->_active_level_id * !(codes[i] && gd->levels[i]));


		gd->_selected_level_id = (gd->_active_level_id * graphics::getKeyState(graphics::scancode_t::SCANCODE_S)) + (gd->_selected_level_id * !graphics::getKeyState(graphics::scancode_t::SCANCODE_S));


		gd->_active_level_id = (-1 * graphics::getKeyState(graphics::scancode_t::SCANCODE_D)) + (gd->_active_level_id * !graphics::getKeyState(graphics::scancode_t::SCANCODE_D));
		gd->_selected_level_id = (-1 * graphics::getKeyState(graphics::scancode_t::SCANCODE_D)) + (gd->_selected_level_id * !graphics::getKeyState(graphics::scancode_t::SCANCODE_D));

		break;
	}
	case game_states::OP_PLAYER: {
		break;
	}
	case game_states::EXIT: {
		std::terminate();
	}
	case game_states::PAUSE: {
		// yes there is a button to un-pause but some prefer to use the keyboard for easier access
		gd->game_state = ((game_states::GAME * graphics::getKeyState(graphics::scancode_t::SCANCODE_U)) + (gd->game_state * !graphics::getKeyState(graphics::scancode_t::SCANCODE_U)));

		break;
	}
	case game_states::HELP:
	case game_states::CREDITS:
	default: {
		;
	}
	}
}

void draw()
{
	GameData* gd = reinterpret_cast<GameData*> (graphics::getUserData());
	graphics::resetPose();
	br.texture = "";

	if (bg_br.texture != "") 	//if there is something to draw
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, bg_br);

	switch (gd->game_state)
	{
	case game_states::LOAD: {
		setColor(br, 'W');

		if (gd->image_names.empty())
			return;
		std::string curr_image = image_path + gd->image_names[gd->curr_img_index];

		br.texture = curr_image;
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_HEIGHT / 4, CANVAS_HEIGHT / 4, br);
		setColor(br, 'L');
		br.texture = "";
		graphics::drawText(CANVAS_WIDTH / 100, 15 * CANVAS_HEIGHT / 100, CANVAS_HEIGHT / 25, "Loading:   " + curr_image, br);


		break;
	}
	case game_states::LOAD_L: {
		setColor(br, 'L');
		graphics::drawText(CANVAS_WIDTH / 100, 15 * CANVAS_HEIGHT / 100, CANVAS_HEIGHT / 25, "Loading levels...", br);
		break;
	}
	case game_states::MENU: {
		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		graphics::drawText(CANVAS_WIDTH / 4, CANVAS_HEIGHT / 4, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "Welcome!", br);
		graphics::drawText(CANVAS_WIDTH / 8, 3 * CANVAS_HEIGHT / 5, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "Press S to start!", br);
		graphics::drawText(CANVAS_WIDTH / 8, 4 * CANVAS_HEIGHT / 5, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "Press D to demo!", br);

		break;
	}
	case game_states::ACHIEVEMENTS: {
		graphics::drawText(CANVAS_WIDTH / 2, 20, 20, "Unlocked Achievements: " + std::to_string(gd->getAchieved(gd).size()) + "/4", br);
		int i = 0;
		for (auto a : gd->getAchieved(gd)) {
			br.texture = a->icon;
			setColor(br,'W');
			graphics::drawRect(60, 75 + i * 100, 75, 75, br);
			setColor(br, 'L');
			graphics::drawText(0, 75 + i * 100 + 50, 10, a->name, br);
			i++;
		}
		graphics::drawText(0, CANVAS_HEIGHT - 15, 10, "Return to this page after playing a game to see if you have won any achievements!", br);
		graphics::drawText(0, CANVAS_HEIGHT, 10, "Achievement progress is kept between successive games (but not if the program closes).", br);
		break;
	}
	case game_states::GAME: {

		gd->drawBackground(br);
		// entity drawing
		gd->draw(gd->enemyLs);
		gd->draw(gd->enemyProjLs);
		gd->draw(gd->playerLs);
		gd->draw(gd->playerProjLs);
		gd->draw(gd->effectsLs);
		gd->draw(gd->powerupLs);
		ui->draw();
		break;
	}
	case game_states::LEVEL_TRANSITION: {
		// do some drawing while waiting for next level
		gd->drawBackground(br);
		gd->draw(gd->playerLs);
		gd->draw(gd->playerProjLs);
		gd->draw(gd->effectsLs);
		gd->draw(gd->powerupLs);
		ui->draw();

		// display timer
		graphics::resetPose();
		setColor(br, 'L');
		graphics::drawText(0.2f * get_canvas_width(), 0.3f * get_canvas_height(), 20,
                      "Next level in: " + std::to_string(gd->level_transition_timer).substr(0, 5), br);
		ui->draw();
		break;
	}
	case game_states::GAME_LOSE: {
		setColor(br, new float[3]{ 1.0f, 1.0f, 1.0f });
		graphics::drawText(CANVAS_WIDTH / 6, 2 * CANVAS_HEIGHT / 5, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "You lost!", br);
		graphics::drawText(CANVAS_WIDTH / 6, CANVAS_HEIGHT / 2, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "Final score: " + std::to_string(gd->getScore()), br);
		break;
	}
	case game_states::GAME_WIN: {
		setColor(br, new float[3]{ 1.0f, 1.0f, 1.0f });
		graphics::drawText(CANVAS_WIDTH / 6, 2 * CANVAS_HEIGHT / 5, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "You won!", br);
		graphics::drawText(CANVAS_WIDTH / 6, CANVAS_HEIGHT / 2, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 10, "Final score: " + std::to_string(gd->getScore()), br);
		break;
	}
	case game_states::RESET: {
		break;
	}
	case game_states::OPTIONS: {

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		graphics::drawText(CANVAS_WIDTH / 3, 4.0f * CANVAS_HEIGHT / 10, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Select Level", br);
		graphics::drawText(CANVAS_WIDTH / 3, 9.0f * CANVAS_HEIGHT / 10, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Select Players", br);
		break;
	}
	case game_states::OP_LEVEL: {

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		gd->_active_level_id == -1
			? graphics::drawText(CANVAS_WIDTH / 11, 1.7f * CANVAS_HEIGHT / 10, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 20, "use  the  numbers  to  select  a  track", br)
			: graphics::drawText(CANVAS_WIDTH / 8, 1.7f * CANVAS_HEIGHT / 10, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 30, "Select  this  track  or  Deselect  all  tracks", br);

		int display_height_index = -1;	// -1 because prefix ++ is cooler
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
				graphics::drawText(CANVAS_WIDTH / 20, (2.5f + 0.5f * ++display_height_index) * CANVAS_HEIGHT / 10, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 28, gd->levels[i]->info(), br);
			}
		}

		break;
	}
	case game_states::OP_PLAYER: {

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		graphics::drawText(5 * CANVAS_WIDTH / 20, CANVAS_HEIGHT / 8, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Choose gamemode:", br);
		graphics::drawText(CANVAS_WIDTH / 8, 2 * CANVAS_HEIGHT / 3, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "SinglePlayer", br);
		graphics::drawText(4.5f * CANVAS_WIDTH / 8, 2 * CANVAS_HEIGHT / 3, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Multiplayer", br);

		graphics::drawText(4 * CANVAS_WIDTH / 32, 5 * CANVAS_HEIGHT / 6, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15,
			gd->isMultiplayer ? "Multiplayer mode selected!" : "Singleplayer mode selected!", br);
		break;
	}
	case game_states::HELP: {

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });
		graphics::drawText(CANVAS_WIDTH / 10, 2.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "Player 1:", br);
		graphics::drawText(CANVAS_WIDTH / 10, 3.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 28, "WASD to move, X to fire, Q/E to spin", br);
		graphics::drawText(CANVAS_WIDTH / 10, 5.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "Player 2:", br);
		graphics::drawText(CANVAS_WIDTH / 10, 6.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 28, "Arrow-keys to move, SPACE to fire, '.'/',' to spin", br);
		graphics::drawText(CANVAS_WIDTH / 10, 8.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "Pick up powerups by running into them.", br);
		graphics::drawText(CANVAS_WIDTH / 10, 9.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "Gain points by killing enemies.", br);

		setColor(br,'W');
		br.outline_opacity = 0.f;
		br.texture = std::string(image_path + "player1.png");
		graphics::drawRect(CANVAS_WIDTH / 10, CANVAS_HEIGHT - 80, 40, 80, br);
		br.texture = std::string(image_path + "player2.png");

		graphics::drawRect(CANVAS_WIDTH - (CANVAS_WIDTH / 10), CANVAS_HEIGHT - 80, 40, 80, br);

		br.texture = "";
		break;
	}
	case game_states::EXIT: {
		break;
	}
	case game_states::PAUSE: {

		graphics::drawText(CANVAS_WIDTH / 6, 2.5f * CANVAS_HEIGHT / 13, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 17, "Game paused...", br);

		graphics::drawText(4 * get_canvas_width() / 5, 2 * get_canvas_height() / 20 + 20, ((get_canvas_width() + get_canvas_width()) / 2) / 35, "Unpause (U)", br);

		break;
	}
	case game_states::CREDITS: {

		setColor(br, new float[3]{ 0.0f, 0.0f, 0.0f });

		graphics::drawText(1 * CANVAS_WIDTH / 3, 1 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Made by:", br);
		graphics::drawText(22 * CANVAS_WIDTH / 200, 2 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 12, "Dimitris Tsirmpas and", br);
		graphics::drawText(25 * CANVAS_WIDTH / 100, 3 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 12, "Alex Mandelias", br);
		graphics::drawText(30 * CANVAS_WIDTH / 100, 4 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 18, "with the help of the", br);
		graphics::drawText(33 * CANVAS_WIDTH / 100, 5 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 12, "SGG library.", br);
		graphics::drawText(33 * CANVAS_WIDTH / 300, 6 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 12, "Asset credits can be found", br);
		graphics::drawText(33 * CANVAS_WIDTH / 150, CANVAS_HEIGHT, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 12, "in the assets folder.", br);
		break;
	}
	default: {
		graphics::drawText(40 * CANVAS_WIDTH / 100, 1 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Invalid game state", br);
		graphics::drawText(20 * CANVAS_WIDTH / 100, 1 * CANVAS_HEIGHT / 7, ((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / 15, "Please exit and inform the developers", br);
	}
	}
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

int main(int argc, char** argv)
{
	graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "1942ripoff");
	graphics::setFullScreen(true);
	std::set_terminate(close);

	graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

	graphics::setDrawFunction(draw);
	graphics::setUpdateFunction(update);
	graphics::setResizeFunction(resize);

	br.fill_color[0] = 0.5f;
	br.fill_color[1] = 0.7f;
	br.fill_color[2] = 0.9f;
	graphics::setWindowBackground(br);

	initialize();
	graphics::playMusic(music_path + "menu_music.mp3", 0.5f, true);
	curr_music = MENU_MUSIC;
	bg_br.texture = image_path + "menu.png";
	graphics::startMessageLoop();
	
	//destroyWindow is called in terminate() which is called in every case the game closes
	return 0;
}

// function definitions
void initialize()
{
	GameData* gd = new GameData();
	gd->game_state = game_states::LOAD;

	graphics::setUserData((void*)gd);

	// load stuff
	if (!graphics::setFont(font))
		std::cerr << "Unable to load font from: " << font << std::endl;

	if (!load_images_from_file(image_path))
		std::cerr << "Unable to load images from: " << image_path << std::endl;
}

inline float get_canvas_width() { return CANVAS_WIDTH; }
inline float get_canvas_height() { return CANVAS_HEIGHT; }

float mouse_x(float mx) { return (mx - ((WINDOW_WIDTH  - (CANVAS_WIDTH  * c2w)) / 2)) * w2c; }

float mouse_y(float my) { return (my - ((WINDOW_HEIGHT - (CANVAS_HEIGHT * c2w)) / 2)) * w2c; }
