#pragma once
#include "globals.h"
#include <list>
#include <vector>
#include <unordered_map>
#include <array>

class Player;
class Entity;
class Projectile;
class VisualEffect;
class Powerup;
class Button;
class Level;
class Wave;

/**
* A class that hold the necessary information the main program needs to run
*/
class GameData
{
public:
	class Stats;
	struct Achievement;

	GameData();
	~GameData();
	void reset();

	// ======= ACHIEVEMENTS =======

	Stats* stats;
	std::array<Achievement*, 4> achievements; //c++ doesn't allow creating a const array as a class member
	//Returns a list with all achieved achievements
	const std::list<const Achievement*> getAchieved(GameData*) const;

	// ======= GENERAL =======

	// general
	int fps;
	GAME_STATE game_state;

	// loading
	const float sprites_per_second;
	float elapsed;
	int curr_img_index;
	std::vector<std::string> image_names;
	bool seen_info;

	// game
	bool isMultiplayer;

	// background (looping and moving downwards)
	void updateBackground(float);
	void drawBackground(graphics::Brush&);
	float bg_offset, height_perc_per_second;
		// image data
	std::string bg_sprite = std::string(image_path + "background.png");
	float bg_img_width = 400.0f, bg_img_height = 1213.0f;

	// UI
	void addScore(int);
	int getScore() const;

	// buttons
	void click_buttons();

	// levels
	Level* current_level;		// stores the current level so that the map's levels remain constant
	std::unordered_map<int, Level*> levels;
	std::unordered_map<std::string, Wave*> _waves;
		// used mainly for level selection
	int _active_level_id, _selected_level_id, _playing_level_id;

	void load_levels();			// loads the levels and waves from the .txt files
	Level* const get_next_level();	// returns the next level; nullptr if the player has won (there are no more levels)
	void set_next_level();		// sets the next level
	void update_level(float);	// updates the current level's state

	void spawn();	// spawns new enemies

	float level_transition_timer;
	float set_level_transition_timer(float timer = 5.0f) { return timer; }

	// ======= COLLECTIONS =======

	//list pointers may change during a reset
	std::list<Player*>* playerLs;
	std::list<Entity*>* enemyLs;
	std::list<Projectile*>* enemyProjLs, * playerProjLs;
	std::list<VisualEffect*>* effectsLs;
	std::list<Powerup*>* powerupLs;
	std::list<Button*>* buttonLs;

	// Updates all objects within the list. Template class must be derived from Drawing.
	template <class T>
	static void update(float ms, std::list<T*>* const);

	// Draws all objects within the list. Template class must be derived from Drawing.
	template <class T>
	static void draw(std::list<T*>* const ls);

	// Checks collisions between 2 lists. Template classes must both be derived from GameObject.
	template <class T1, class T2>
	static void checkCollisions(std::list<T1*>* const, std::list<T2*>* const);

	// Spawns a projectile for every eligible object in the list. Template class must be derived from Entity.
	template <class T>
	void fire(std::list<T*>* const) const;

	// Checks if any object within the list must be destroyed, and deletes it. Template class must be derived from Drawing.
	template <class T>
	void checkAndDelete(std::list<T*>* const);

	// Deletes all data from the provided list
	template<class T>
	void static clearList(std::list<T*>* const);

private:
	// Methods used to create levels upon initialization
	bool _load_level_data_from_file(const std::string& level_path, const std::string& wave_path);
	bool _load_waves_from_file(const std::string& wave_path);
	bool _load_levels_from_file(const std::string& level_path);
	// Failsafe in case reading from file fails
	void _load_hardcoded_levels();

	void create_buttons();

	int score;
};

// ======= INNER CLASSES =======

/**
* Holds data for the player's performance during the game
*/
class GameData::Stats 
{
public:
	// Updates internal data based on plane shot
	void plane_shot(const Entity * const en);
	int get_shot_number(int type) const;
	int get_total_shot() const;

	const static int BASIC_PLANE = 0;
	const static int BLACK_PLANE = 1;
	const static int AIRSHIP = 2;
	const static int BALLOON = 3;
	const static int ALL = 100;

private:
	std::array<int, 4> shot_down_arr;
	int find_type(const Entity * const en) const;
};

/**
* A small collection of publically available data for every achievement
*/
struct GameData::Achievement
{
public:
	Achievement(std::string name, std::string icon, int type, int kills);

	const std::string name, icon;
	bool is_achieved(GameData*) const;

private:
	const int TYPE, KILLS;
};

// template method implementations
// must be placed after stats/achievement definitions
#include "game_data.hpp"
