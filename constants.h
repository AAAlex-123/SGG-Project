#pragma once
#include "graphics.h"

// constants
const short WINDOW_WIDTH = 1200;
const short WINDOW_HEIGHT = 600;
const float CANVAS_WIDTH = 1200.0f;
const float CANVAS_HEIGHT = 600.0f;

// enum
enum game_states
{
	TEST = -2,
	LOAD = -1,
	CREDITS = 0,
	MENU = 1,
	GAME = 2,
	END = 3,
	OPTIONS = 4,
	OP_LEVEL = 5,
	HELP = 8,
	EXIT = 9,
};

// assets and other paths
const std::string asset_path = ".\\assets\\";
const std::string font = asset_path + "orange juice 2.0.ttf";
const std::string image_path = ".\\assets\\";
const std::string level_path = ".\\levels.txt";

// function declarations
void initialize();
// true == success
bool load_images_from_file(const std::string& image_path);
bool load_levels_from_file(const std::string& level_path);

void setColor(graphics::Brush&, const float*);
void setColor(graphics::Brush&, char c);

// key short names
const graphics::scancode_t K_UP = graphics::scancode_t::SCANCODE_UP;
const graphics::scancode_t K_DOWN = graphics::scancode_t::SCANCODE_DOWN;
const graphics::scancode_t K_LEFT = graphics::scancode_t::SCANCODE_LEFT;
const graphics::scancode_t K_RIGHT = graphics::scancode_t::SCANCODE_RIGHT;
const graphics::scancode_t K_W = graphics::scancode_t::SCANCODE_W;
const graphics::scancode_t K_A = graphics::scancode_t::SCANCODE_A;
const graphics::scancode_t K_S = graphics::scancode_t::SCANCODE_S;
const graphics::scancode_t K_D = graphics::scancode_t::SCANCODE_D;

//game data
struct GameData {
	list<Entity *> enemyLs; 
	list<Entity*> playerLs; //uniform access to either 1 or 2 players
	list<Projectile *> enemyProjLs;
	list<Projectile*> playerProjLs;
	list<VisualEffect *> effectsLs;
	list<Entity *> enemyQueue;

	int fps;
	int game_state;


	void spawnNextEnemy() { //its ok for this one to be in-line
		if (!this->enemyQueue.empty()) {
			enemyLs.push_back(enemyQueue.front());
			enemyQueue.pop_front();
		}
		else
			cerr << "Queue is empty!" << endl; //debug
	}
};
