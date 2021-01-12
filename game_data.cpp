#include "game_data.h"
#include "GObjFactory.h"
#include <thread>
#include <iostream>
#include <regex>
#include <fstream>

using namespace std;

GameData::GameData()	// the most epic initialization list you'll ever see
	: fps(0), game_state(GAME_STATE::LOAD),
	elapsed(0.0f), sprites_per_second(20.0f), curr_img_index(0), image_names(),
	levels(std::unordered_map<int, Level*>()), _waves(std::unordered_map<std::string, Wave*>()),
	current_level(nullptr), _active_level_id(-1), _selected_level_id(-1),
	bg_offset(0.0f), height_perc_per_second(0.02f), isMultiplayer(false),
	_playing_level_id(-1), level_transition_timer(set_level_transition_timer()),
	enemyLs(new list<Entity*>), playerLs(new list<Player*>), enemyProjLs(new list<Projectile*>),
	playerProjLs(new list<Projectile*>), effectsLs(new list<VisualEffect*>), powerupLs(new list<Powerup*>), buttonLs(new list<Button*>),
	stats(new Stats()), achievements(array<GameData::Achievement*, 4>())
{
	// sets all the user-selectable levels to nullptr to check later
	// if a user-selectable level has been defined (changing the nullptr to Level*) by calling `levels[id]`
	for (int i = 0; i < 10; ++i)
		levels[i] = nullptr;

	create_buttons();

	achievements[0] = new GameData::Achievement("Kill 100 total enemies", icon_path + "achievement_1.png", Stats::ALL, 100);
	achievements[1] = new GameData::Achievement("Destroy 20 balloons", icon_path + "achievement_2.png", Stats::BALLOON, 20);
	achievements[2] = new GameData::Achievement("Destroy an American airship", icon_path + "achievement_3.png", Stats::AIRSHIP, 1);
	achievements[3] = new GameData::Achievement("Destroy 50 british fighters", icon_path + "achievement_4.png", Stats::BLACK_PLANE, 50);
}

void GameData::reset()
{
	delete current_level;
	current_level = nullptr;
	_active_level_id = -1;
	_selected_level_id = -1;
	_playing_level_id = -1;
	score = 0;

	enemyLs = new list<Entity*>;
	playerLs = new list<Player*>;
	enemyProjLs = new list<Projectile*>;
	playerProjLs = new list<Projectile*>;
	effectsLs = new list<VisualEffect*>;
	powerupLs = new list<Powerup*>;
	buttonLs = new list<Button*>;

	create_buttons();
}

const list<const GameData::Achievement*> GameData::getAchieved(GameData* gd) const
{
	list<const GameData::Achievement*> ls;

	for (GameData::Achievement* a : achievements)
		if (a->is_achieved(gd))
			ls.push_back(a);

	return ls;
}

void GameData::updateBackground(float ms)
{
	bg_offset = (bg_offset < bg_img_height / get_canvas_height())
		? (bg_offset + (height_perc_per_second * (ms / 1000.0f)))
		: (0.0f);
}

void GameData::drawBackground(graphics::Brush& br)
{
	br.texture = bg_sprite;
	br.outline_opacity = 0.0f;
	setColor(br, 'W');

	float cw = get_canvas_width(), ch = get_canvas_height();
	graphics::drawRect(cw / 2, (ch * bg_offset) - bg_img_height, bg_img_width, bg_img_height, br);
	graphics::drawRect(cw / 2, (ch * bg_offset),				 bg_img_width, bg_img_height, br);
}

void GameData::addScore(int scored)
{
	score += scored;
}

int GameData::getScore() const
{
	return score;
}

void GameData::click_buttons()
{
	for (Button* b : *buttonLs)
		b->execute();
}

void GameData::load_levels()
{
	if (!_load_level_data_from_file(level_file, wave_file))
	{
		std::cerr << "Warning: Level loading from files failed, loading hardcoded levels" << std::endl;
		_load_hardcoded_levels();
	}
	std::cout << "Levels loaded successfully" << std::endl;
}

Level* GameData::get_next_level()
{
	Level* return_val = levels[_playing_level_id + 1];
	if (!return_val)
		levels.erase(_playing_level_id + 1);
	return return_val;
}

void GameData::set_next_level()
{
	current_level = levels[++_playing_level_id]->clone();
}

void GameData::update_level(float ms)
{
	current_level->update(ms);
}

void GameData::spawn()
{
	if (current_level->can_spawn())
		enemyLs->push_back(current_level->spawn());
	if (current_level->can_spawn_p())
		powerupLs->push_back(current_level->spawn_p());
}

bool GameData::_load_level_data_from_file(const std::string& level_file_path, const std::string& wave_file_path)
{
	// first load waves
	if (!_load_waves_from_file(wave_file_path))
	{
		std::cerr << "Warning: Unable to load waves from file: " + wave_file_path << std::endl;
		return false;
	}

	// then load levels; the levels' waves must have the same name as the waves loaded above
	if (!_load_levels_from_file(level_file_path))
	{
		std::cerr << "Warning: Unable to load levels from file: " + level_file_path << std::endl;
		return false;
	}
	return true;
}

bool GameData::_load_waves_from_file(const std::string& wave_file_path)
{
	// create stream to levels_path
	std::ifstream in(wave_file_path);

	if (!in)
	{
		std::cerr << "Error opening file '" << wave_file_path << "'" << std::endl;
		return false;
	}

	// get all lines from file
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	std::smatch match;

	// regex patterns
	std::string rs1("Wave\\(\"(.*)\"\\)");
	std::string rs2("Spawnpoint\\((\\d+), (.*?), (.*?), (.*?), (\\d+), (.*?), (.*?)\\)");

	// regex objects
	std::regex r1, r2, eof, comment;

	int line = 0;
	std::string curr_wave = "";

	while (true)
	{
		// construct regexes 
		r1 = std::regex("^" + std::to_string(line) + "   " + rs1 + "$");
		r2 = std::regex("^" + std::to_string(line) + "   " + rs2 + "$");
		eof = std::regex("^" + std::to_string(line) + "$");
		comment = std::regex("^" + std::to_string(line) + "//");

		// wave declaration
		if (std::regex_search(contents, match, r1))
		{
			curr_wave = match[1];
			_waves[curr_wave] = new Wave(curr_wave);
		}

		// spawnnpoint declaration
		else if (std::regex_search(contents, match, r2))
		{
			if (curr_wave == "")
			{
				std::cerr << "Error: Attempting to add spawnpoint before declaring a wave at line " << line << std::endl;
				return false;
			}

			// SIMPLE_ENEMY = 1,		SIMPLE_ENEMY_F = 7,		ROTATING_ENEMY_D = 2,	ROTATING_ENEMY_CA = 6
			// ROTATING_ENEMY_C = 8,	ACCELERATING_ENEMY = 3, TANK_ENEMY = 4,			HOMING_ENEMY = 5;

			// arbitrarily match ints to the factory's enum

			GObjFactory::ENEMY type = GObjFactory::ENEMY::SIMPLE_ENEMY;
			switch (stoi(match[1]))
			{
			case 1:
				type = GObjFactory::ENEMY::SIMPLE_ENEMY;
				break;
			case 2:
				type = GObjFactory::ENEMY::ROTATING_ENEMY_D;
				break;
			case 3:
				type = GObjFactory::ENEMY::ACCELERATING_ENEMY;
				break;
			case 4:
				type = GObjFactory::ENEMY::TANK_ENEMY;
				break;
			case 5:
				type = GObjFactory::ENEMY::HOMING_ENEMY;
				break;
			case 6:
				type = GObjFactory::ENEMY::ROTATING_ENEMY_CA;
				break;
			case 7:
				type = GObjFactory::ENEMY::SIMPLE_ENEMY_F;
				break;
			case 8:
				type = GObjFactory::ENEMY::ROTATING_ENEMY_C;
				break;
			default:
				std::cerr << "GameData::_load_waves_from_file: invalid enemy type from file: " << stoi(match[1])
					<< ". Creating simple enemy" << std::endl;;
			}
			_waves[curr_wave]->add_spawnpoint(new Spawnpoint(
				type, stof(match[2]), stof(match[3]), stof(match[4]) / 180 * PI,
				stoi(match[5]), stof(match[6]), stof(match[7])
			));
		}

		// eof
		else if (std::regex_search(contents, match, eof))
		{
			break;
		}

		// comment
		else if (std::regex_search(contents, match, comment))
		{
			;
		}

		// no match and not eof -> invalid line syntax
		else
		{
			std::cerr << "Error: Line " << line << " has invalid syntax or was not found" << std::endl;
			return false;
		}

		++line;
		// advance the regex somehow idk ask stackoverflow
		contents = match.suffix();
	}

	// close stream
	in.close();
	return true;
}

bool GameData::_load_levels_from_file(const std::string& level_file_path)
{
	// create stream to levels_path
	std::ifstream in(level_file_path);

	if (!in)
	{
		std::cerr << "Error opening file '" << level_file_path << "'" << std::endl;
		return false;
	}

	// get all lines from file
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	std::smatch match;

	// regex patterns
	std::string rs1("Level\\((-?\\d+), \"(.*)\"\\)");
	std::string rs2("wave (.*?) (.*)");
	std::string rs3("(.*?)Powerup\\((.*?), (.*?), (.*?)\\) (.*?)");

	// regex objects
	std::regex r1, r2, r3, eof, comment;

	int line = 0;
	int curr_level_id = 0;

	while (true)
	{
		// construct regexes
		r1 = std::regex("^" + std::to_string(line) + "   " + rs1 + "$");
		r2 = std::regex("^" + std::to_string(line) + "   " + rs2 + "$");
		r3 = std::regex("^" + std::to_string(line) + "   " + rs3 + "$");
		eof = std::regex("^" + std::to_string(line) + "$");
		comment = std::regex("^" + std::to_string(line) + "//");

		// level declaration
		if (std::regex_search(contents, match, r1))
		{
			curr_level_id = stoi(match[1]);
			levels[curr_level_id] = new Level(curr_level_id, match[2]);
		}

		// wave declaration
		else if (std::regex_search(contents, match, r2))
		{
			if (_waves.find(match[1]) == _waves.end())
			{
				std::cerr << "Error: Wave with name '" << match[1] << "' not found" << std::endl;
				return false;
			}
			levels[curr_level_id]->add_wave(stof(match[2]), new Wave(*_waves[match[1]]));
		}

		// powerup declaration
		else if (std::regex_search(contents, match, r3))
		{
			if (match[1] == "Health")
				levels[curr_level_id]->add_powerup(stof(match[5]), new HealthPowerup(
					get_canvas_width() * stof(match[2]), get_canvas_height() * stof(match[3]), stof(match[4]) / 180 * PI));
			else if (match[1] == "Projectile")
				levels[curr_level_id]->add_powerup(stof(match[5]), new ProjectilePowerup(
					get_canvas_width() * stof(match[2]), get_canvas_height() * stof(match[3]), stof(match[4]) / 180 * PI));
			else if (match[1] == "Points")
				levels[curr_level_id]->add_powerup(stof(match[5]), new PointsPowerup(
					get_canvas_width() * stof(match[2]), get_canvas_height() * stof(match[3]), stof(match[4]) / 180 * PI));
			else
			{
				std::cerr << "Error: Powerup with name '" << match[1] << "' not found" << std::endl;
				return false;
			}
		}

		// eof
		else if (std::regex_search(contents, match, eof))
		{
			break;
		}

		// comment
		else if (std::regex_search(contents, match, comment))
		{
			;
		}

		// no match and not eof -> invalid line syntax
		else
		{
			std::cerr << "Error: Line " << line << " has invalid syntax or was not found" << std::endl;
			return false;
		}

		++line;
		// advance the regex somehow idk ask stackoverflow
		contents = match.suffix();
	}

	// close stream
	in.close();
	return true;
}

// wip lmao
void GameData::_load_hardcoded_levels()
{
	Spawnpoint* sp11 = new Spawnpoint(GObjFactory::ENEMY::SIMPLE_ENEMY, 0.0f, 0.5, -PI / 2, 10, 1.0f, 0.0f);

	Wave* w1 = new Wave("line");
	w1->add_spawnpoint(sp11);

	Level* l1 = new Level(-2, "owo");
	l1->add_wave(0.0f, w1);

	/*	ABOUT LEVEL ID
		=>	no level can have an id of -1 (because -1 is the default)
		=>	levels selectable by users must have id 0-9 inclusive
		=>	levels not be selectable by users must have any other id
		=>	level id should always match its key in the levels map like below
	*/
	levels[l1->id()] = l1;
}

void GameData::create_buttons()
{
	// menu
	buttonLs->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "exit.png"), GAME_STATE::MENU, GAME_STATE::EXIT));
	buttonLs->push_back(new GameStateChangingButton(this, 370.0f, 30.0f, 30.0f, new string(icon_path + "help.png"), GAME_STATE::MENU, GAME_STATE::HELP));
	buttonLs->push_back(new GameStateChangingButton(this, 370.0f, 75.0f, 30.0f, new string(icon_path + "options.png"), GAME_STATE::MENU, GAME_STATE::OPTIONS));
	buttonLs->push_back(new GameStateChangingButton(this, 370.0f, 120.0f, 30.0f, new string(icon_path + "credits.png"), GAME_STATE::MENU, GAME_STATE::CREDITS));
	buttonLs->push_back(new GameStateChangingButton(this, 370.0f, 165.0f, 30.0f, new string(icon_path + "achievements.png"), GAME_STATE::MENU, GAME_STATE::ACHIEVEMENTS));
	buttonLs->push_back(new GameStateChangingButton(this, 370.0f, 210.0f, 30.0f, new string(icon_path + "reload.png"), GAME_STATE::MENU, GAME_STATE::LOAD_L));
	// game
	buttonLs->push_back(new GameStateChangingButton(this, 370.0f, 30.0f, 30.0f, new string(icon_path + "pause.png"), GAME_STATE::GAME, GAME_STATE::PAUSE));
	// game -- pause
	buttonLs->push_back(new GameStateChangingButton(this, 370.0f, 30.0f, 30.0f, new string(icon_path + "continue.png"), GAME_STATE::PAUSE, GAME_STATE::GAME));
	// game -- lose
	buttonLs->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back2.png"), GAME_STATE::GAME_LOSE, GAME_STATE::RESET));
	// game -- win
	buttonLs->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back2.png"), GAME_STATE::GAME_WIN, GAME_STATE::RESET));
	// help
	buttonLs->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), GAME_STATE::HELP, GAME_STATE::MENU));
	// credits
	buttonLs->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), GAME_STATE::CREDITS, GAME_STATE::MENU));
	// options
	buttonLs->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), GAME_STATE::OPTIONS, GAME_STATE::MENU));
	buttonLs->push_back(new GameStateChangingButton(this, 200.0f, 100.0f, 100.0f, new string(icon_path + "level_select.png"), GAME_STATE::OPTIONS, GAME_STATE::OP_LEVEL));
	buttonLs->push_back(new GameStateChangingButton(this, 200.0f, 350.0f, 100.0f, new string(icon_path + "player_select.png"), GAME_STATE::OPTIONS, GAME_STATE::OP_PLAYER));
	// options -- player
	buttonLs->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), GAME_STATE::OP_PLAYER, GAME_STATE::OPTIONS));
	buttonLs->push_back(new VariableChangingButton<bool>(this, 112.5f, 200.0f, 125.0f, new string(icon_path + "singleplayer.png"), GAME_STATE::OP_PLAYER, &this->isMultiplayer, false));
	buttonLs->push_back(new VariableChangingButton<bool>(this, 287.5f, 200.0f, 125.0f, new string(icon_path + "multiplayer.png"), GAME_STATE::OP_PLAYER, &this->isMultiplayer, true));
	// options -- level
	buttonLs->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), GAME_STATE::OP_LEVEL, GAME_STATE::OPTIONS));
	// achievement
	buttonLs->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), GAME_STATE::ACHIEVEMENTS, GAME_STATE::MENU));
	// invalid_state
	buttonLs->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), GAME_STATE::INVALID_STATE, GAME_STATE::MENU));
}

GameData::~GameData()
{
	deleteList(playerLs);
	deleteList(enemyLs);
	deleteList(enemyProjLs);
	deleteList(playerProjLs);
	deleteList(effectsLs);
	deleteList(buttonLs);
}

//=======INNER CLASSES========

void GameData::Stats::plane_shot(const Entity * const en)
{
	++shot_down_arr[find_type(en)];
}

int GameData::Stats::get_shot_number(int type) const
{
	if (type > shot_down_arr.size() || type < 0)
	{
		std::cerr << " Couldn't find enemy type information in get_shot_number, returning 0" << std::endl;
		return 0;
	}
	return shot_down_arr[type];
}

int GameData::Stats::get_total_shot() const
{
	int total_shot = 0;
	for (int shot_no : shot_down_arr)
		total_shot += shot_no;
	return total_shot;
}

int GameData::Stats::find_type(const Entity* const en) const
{
	const std::string name = *en->getSprite();
	if (name == image_path + "plane1.png")
		return BASIC_PLANE;
	else if (name == image_path + "plane2.png" || name == image_path + "plane4.png")
		return BLACK_PLANE;
	else if (name == image_path + "plane3.png")
		return AIRSHIP;
	else if (name == image_path + "balloon.png")
		return BALLOON;
	else
	{
		std::cerr << "Entity can't be identified in Stats (sprite name = " << name << ")" << std::endl;
		return BASIC_PLANE;
	}
}

GameData::Achievement::Achievement(std::string name, std::string icon, int type, int kills)
	: name(name), icon(icon), TYPE(type), KILLS(kills)
{}

bool GameData::Achievement::is_achieved(GameData* gd) const
{
	if (TYPE == Stats::ALL)
		return KILLS < gd->stats->get_total_shot();
	return KILLS < gd->stats->get_shot_number(TYPE);
}
