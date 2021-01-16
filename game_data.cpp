#include "game_data.h"
#include "Factory.h"
#include <iostream>
#include <regex>
#include <fstream>

using namespace std;


GameData::GameData()	// the most epic initialization list you'll ever see
	: fps(0), game_state(GAME_STATE::LOAD),
	elapsed(0.0f), sprites_per_second(20.0f), curr_img_index(0), image_names(),
	levels(std::unordered_map<int, Level*>()), _waves(std::unordered_map<std::string, Wave*>()),
	current_level(nullptr), _active_level_id(-1), _selected_level_id(-1),
	bg_offset(0.0f), height_perc_per_second(0.02f), seen_info(false), isMultiplayer(false),
	_playing_level_id(-1), level_transition_timer(set_level_transition_timer()),
	enemyLs(new list<Entity*>), playerLs(new list<Player*>), enemyProjLs(new list<Projectile*>),
	playerProjLs(new list<Projectile*>), effectsLs(new list<VisualEffect*>), powerupLs(new list<Powerup*>), buttonLs(new list<Button*>),
	stats(new Stats())
{
	// sets all the user-selectable levels to nullptr to check later
	// if a user-selectable level has been defined (changing the nullptr to Level*) by calling `levels[id]`
	for (int i = 0; i < 10; ++i)
		levels[i] = nullptr;

	create_buttons();

	achievements = {
	new GameData::Achievement("Kill 100 total enemies", icon_path + "achievement_1.png",Stats::ALL,100),
	new GameData::Achievement("Destroy 20 balloons", icon_path + "achievement_2.png",Stats::BALLOON,20),
	new GameData::Achievement("Destroy an American airship", icon_path + "achievement_3.png",Stats::AIRSHIP,1),
	new GameData::Achievement("Destroy 50 british fighters", icon_path + "achievement_4.png",Stats::BLACK_PLANE,50)};
}

void GameData::reset()
{
	delete current_level;
	current_level = nullptr;
	_active_level_id = -1;
	_selected_level_id = -1;
	_playing_level_id = -1;
	score = 0;

	clearList(playerLs);
	clearList(enemyLs);
	clearList(enemyProjLs);
	clearList(playerProjLs);
	clearList(effectsLs);
	clearList(powerupLs);
	clearList(buttonLs);

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
		std::cerr << "Warning: Level loading from files failed, loading from backup files" << std::endl;
		if (!_load_level_data_from_file(hardcoded_level_file, hardcoded_wave_file))
		{
			std::cerr << "Warning: Level loading from backup files failed, loading hardcoded levels" << std::endl;
			_load_hardcoded_levels();
			return;
		}
	}
	std::cout << "Levels loaded successfully" << std::endl;
}

Level* const GameData::get_next_level()
{
	Level* const return_val = levels[_playing_level_id + 1];
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
			/*  couldn't make it work. leave it here for possible future work
				// create a new wave only if one doesn't already exist
				// allows to add spawnpoints to existing waves just by re-declaring the wave
				if (_waves.find(curr_wave) == _waves.end())
			*/
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

			/*
				arbitrarily match ints to the factory's enum
			
				SIMPLE_ENEMY = 1
				SIMPLE_ENEMY_F = 2
				ACCELERATING_ENEMY = 3
				TANK_ENEMY = 4
				HOMING_ENEMY = 5
				ROTATING_ENEMY_C = 6
				ROTATING_ENEMY_CA = 7
				ROTATING_ENEMY_D = 8
			*/

			Factory::ENEMY type = Factory::ENEMY::SIMPLE_ENEMY;
			switch (stoi(match[1]))
			{
			case 1:
				type = Factory::ENEMY::SIMPLE_ENEMY;
				break;
			case 2:
				type = Factory::ENEMY::SIMPLE_ENEMY_F;
				break;
			case 3:
				type = Factory::ENEMY::ACCELERATING_ENEMY;
				break;
			case 4:
				type = Factory::ENEMY::TANK_ENEMY;
				break;
			case 5:
				type = Factory::ENEMY::HOMING_ENEMY;
				break;
			case 6:
				type = Factory::ENEMY::ROTATING_ENEMY_C;
				break;
			case 7:
				type = Factory::ENEMY::ROTATING_ENEMY_CA;
				break;
			case 8:
				type = Factory::ENEMY::ROTATING_ENEMY_D;
				break;
			default:
				std::cerr << "GameData::_load_waves_from_file: invalid enemy type: " << stoi(match[1])
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
			if (curr_level_id == -1)
			{
				std::cerr << "Error: ID -1 is not valid" << std::endl;
				return false;
			}
			curr_level_id = stoi(match[1]);
			/*  couldn't make it work. leave it here for possible future work
				// create a new level only if one doesn't already exist
				// allows to add waves to existing levels just by re-declaring the level
				if (levels.find(curr_level_id) == levels.end())
			*/
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
			levels[curr_level_id]->add_wave(stof(match[2]), _waves[match[1]]->clone());
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

void GameData::create_buttons()
{
	// info
	buttonLs->push_back(new GameStateChangingButton(this, 335.0f, 435.0f, 50.0f, new string(icon_path + "to_menu.png"), GAME_STATE::INFO, GAME_STATE::MENU));
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
	clearList(playerLs);
	clearList(enemyLs);
	clearList(enemyProjLs);
	clearList(playerProjLs);
	clearList(effectsLs);
	clearList(buttonLs);
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

// the granddaddy of all methods
void GameData::_load_hardcoded_levels()
{
	Wave* curr_wave;
	_waves["donuts"] = new Wave("donuts");
	curr_wave = _waves["donuts"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ROTATING_ENEMY_D, 0.1f, 0.1f, -180.0f / 180 * PI / 180 * PI, 2, 10.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ROTATING_ENEMY_D, 0.4f, 0.1f, -180.0f / 180 * PI, 2, 10.0f, 2.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ROTATING_ENEMY_D, 0.7f, 0.1f, -180.0f / 180 * PI, 2, 10.0f, 4.0f));
	_waves["half-circles-u"] = new Wave("half-circles-u");
	curr_wave = _waves["half-circles-u"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ROTATING_ENEMY_CA, 0.5f, -0.1f, -180.0f / 180 * PI, 5, 3.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ROTATING_ENEMY_C, 0.5f, -0.1f, 180.0f / 180 * PI, 5, 3.0f, 1.5f));
	_waves["tank-u"] = new Wave("tank-u");
	curr_wave = _waves["tank-u"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::TANK_ENEMY, 0.1f, -0.1f, -180.0f / 180 * PI, 1, 0.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::TANK_ENEMY, 0.3f, -0.1f, -180.0f / 180 * PI, 1, 0.0f, 2.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::TANK_ENEMY, 0.7f, -0.1f, -180.0f / 180 * PI, 1, 0.0f, 2.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::TANK_ENEMY, 0.9f, -0.1f, -180.0f / 180 * PI, 1, 0.0f, 0.0f));
	_waves["simple-u-h"] = new Wave("simple-u-h");
	curr_wave = _waves["simple-u-h"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 0.1f, -0.1f, -170.0f / 180 * PI, 6, 2.0f, 0.3f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 0.2f, -0.1f, -165.0f / 180 * PI, 4, 2.0f, 1.4f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 0.3f, -0.1f, 175.0f / 180 * PI, 5, 2.2f, 0.7f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 0.45f, -0.1f, -165.0f / 180 * PI, 6, 1.9f, 1.1f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 0.58f, -0.1f, 170.0f / 180 * PI, 8, 1.7f, 1.2f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 0.70f, -0.1f, 175.0f / 180 * PI, 7, 2.2f, 0.5f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 0.81f, -0.1f, 150.0f / 180 * PI, 4, 2.7f, 0.8f));
	_waves["simple-u-e"] = new Wave("simple-u-e");
	curr_wave = _waves["simple-u-e"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 0.2f, -0.1f, -175.0f / 180 * PI, 4, 2.8f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 0.3f, -0.1f, -175.0f / 180 * PI, 3, 3.1f, 1.3f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 0.6f, -0.1f, 175.0f / 180 * PI, 4, 1.5f, 2.5f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 0.9f, -0.1f, 170.0f / 180 * PI, 4, 2.3f, 1.9f));
	_waves["simple-u-m"] = new Wave("simple-u-m");
	curr_wave = _waves["simple-u-m"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 0.2f, -0.1f, -175.0f / 180 * PI, 6, 2.7f, 0.2f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 0.35f, -0.1f, -170.0f / 180 * PI, 5, 3.4f, 1.8f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 0.4f, -0.1f, -175.0f / 180 * PI, 6, 2.4f, 0.9f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 0.5f, -0.1f, 170.0f / 180 * PI, 5, 3.0f, 1.7f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 0.6f, -0.1f, 175.0f / 180 * PI, 6, 2.7f, 1.1f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 0.75f, -0.1f, 170.0f / 180 * PI, 6, 3.3f, 0.5f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 0.85f, -0.1f, 165.0f / 180 * PI, 4, 3.7f, 1.4f));
	_waves["half-circles-s"] = new Wave("half-circles-s");
	curr_wave = _waves["half-circles-s"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ROTATING_ENEMY_C, -0.1f, 0.25f, -45.0f / 180 * PI, 2, 7.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ROTATING_ENEMY_C, -0.1f, 0.35f, -45.0f / 180 * PI, 2, 7.0f, 2.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ROTATING_ENEMY_C, -0.1f, 0.45f, -45.0f / 180 * PI, 2, 7.0f, 4.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ROTATING_ENEMY_CA, 1.1f, 0.30f, 45.0f / 180 * PI, 2, 7.0f, 1.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ROTATING_ENEMY_CA, 1.1f, 0.40f, 45.0f / 180 * PI, 2, 7.0f, 3.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ROTATING_ENEMY_CA, 1.1f, 0.50f, 45.0f / 180 * PI, 2, 7.0f, 5.0f));
	_waves["tank-l"] = new Wave("tank-l");
	curr_wave = _waves["tank-l"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::TANK_ENEMY, -0.1f, 0.15f, -90.0f / 180 * PI, 2, 7.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::TANK_ENEMY, -0.1f, 0.25f, -90.0f / 180 * PI, 2, 7.0f, 1.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::TANK_ENEMY, -0.1f, 0.35f, -90.0f / 180 * PI, 2, 7.0f, 2.0f));
	_waves["tank-r"] = new Wave("tank-r");
	curr_wave = _waves["tank-r"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::TANK_ENEMY, 1.1f, 0.15f, 90.0f / 180 * PI, 2, 7.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::TANK_ENEMY, 1.1f, 0.25f, 90.0f / 180 * PI, 2, 7.0f, 1.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::TANK_ENEMY, 1.1f, 0.35f, 90.0f / 180 * PI, 2, 7.0f, 2.0f));
	_waves["simple-l-h"] = new Wave("simple-l-h");
	curr_wave = _waves["simple-l-h"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, -0.1f, 0.10f, -135.0f / 180 * PI, 7, 2.8f, 0.1f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, -0.1f, 0.15f, -135.0f / 180 * PI, 7, 3.0f, 0.5f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, -0.1f, 0.20f, -125.0f / 180 * PI, 4, 4.0f, 2.4f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, -0.1f, 0.15f, -110.0f / 180 * PI, 5, 4.1f, 2.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, -0.1f, 0.20f, -120.0f / 180 * PI, 6, 3.7f, 1.7f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, -0.1f, 0.25f, -110.0f / 180 * PI, 4, 4.4f, 2.9f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, -0.1f, 0.25f, -120.0f / 180 * PI, 6, 3.7f, 1.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, -0.1f, 0.30f, -110.0f / 180 * PI, 7, 3.3f, 0.7f));
	_waves["simple-l-m"] = new Wave("simple-l-m");
	curr_wave = _waves["simple-l-m"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, -0.1f, 0.15f, -135.0f / 180 * PI, 6, 3.0f, 0.1f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, -0.1f, 0.20f, -125.0f / 180 * PI, 6, 3.5f, 0.9f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, -0.1f, 0.15f, -110.0f / 180 * PI, 4, 4.0f, 1.4f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, -0.1f, 0.20f, -120.0f / 180 * PI, 5, 3.7f, 2.2f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, -0.1f, 0.25f, -110.0f / 180 * PI, 6, 3.3f, 0.6f));
	_waves["simple-l-e"] = new Wave("simple-l-e");
	curr_wave = _waves["simple-l-e"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, -0.1f, 0.15f, -135.0f / 180 * PI, 4, 3.4f, 0.1f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, -0.1f, 0.20f, -125.0f / 180 * PI, 4, 3.9f, 0.9f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, -0.1f, 0.15f, -110.0f / 180 * PI, 3, 4.3f, 1.4f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, -0.1f, 0.20f, -120.0f / 180 * PI, 3, 4.1f, 1.7f));
	_waves["simple-r-h"] = new Wave("simple-r-h");
	curr_wave = _waves["simple-r-h"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 1.1f, 0.10f, 135.0f / 180 * PI, 6, 2.8f, 0.1f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 1.1f, 0.15f, 135.0f / 180 * PI, 7, 3.0f, 0.5f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 1.1f, 0.20f, 125.0f / 180 * PI, 7, 3.2f, 2.4f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 1.1f, 0.15f, 110.0f / 180 * PI, 7, 3.5f, 2.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 1.1f, 0.20f, 120.0f / 180 * PI, 4, 3.2f, 1.7f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 1.1f, 0.25f, 110.0f / 180 * PI, 5, 4.4f, 2.9f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 1.1f, 0.25f, 120.0f / 180 * PI, 6, 3.7f, 1.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 1.1f, 0.30f, 110.0f / 180 * PI, 4, 4.0f, 0.7f));
	_waves["simple-r-m"] = new Wave("simple-r-m");
	curr_wave = _waves["simple-r-m"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 1.1f, 0.15f, 135.0f / 180 * PI, 4, 4.0f, 0.1f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 1.1f, 0.20f, 125.0f / 180 * PI, 5, 3.5f, 0.9f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 1.1f, 0.15f, 110.0f / 180 * PI, 6, 3.0f, 1.4f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 1.1f, 0.20f, 120.0f / 180 * PI, 6, 3.2f, 2.2f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 1.1f, 0.25f, 110.0f / 180 * PI, 6, 3.3f, 0.6f));
	_waves["simple-r-e"] = new Wave("simple-r-e");
	curr_wave = _waves["simple-r-e"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 1.1f, 0.15f, 135.0f / 180 * PI, 3, 4.4f, 0.1f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 1.1f, 0.20f, 125.0f / 180 * PI, 4, 3.9f, 0.9f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY, 1.1f, 0.15f, 110.0f / 180 * PI, 3, 4.3f, 1.4f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::SIMPLE_ENEMY_F, 1.1f, 0.20f, 120.0f / 180 * PI, 4, 3.5f, 1.7f));
	_waves["balloons-e"] = new Wave("balloons-e");
	curr_wave = _waves["balloons-e"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::HOMING_ENEMY, -0.1f, -0.1f, -160.0f / 180 * PI, 2, 5.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::HOMING_ENEMY, 0.5f, -0.1f, -180.0f / 180 * PI, 2, 5.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::HOMING_ENEMY, 1.1f, -0.1f, 160.0f / 180 * PI, 2, 5.0f, 0.0f));
	_waves["balloons-m"] = new Wave("balloons-m");
	curr_wave = _waves["balloons-m"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::HOMING_ENEMY, -0.1f, -0.1f, -130.0f / 180 * PI, 2, 7.0f, 3.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::HOMING_ENEMY, -0.1f, 0.1f, -140.0f / 180 * PI, 3, 7.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::HOMING_ENEMY, 1.1f, 0.1f, 140.0f / 180 * PI, 3, 7.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::HOMING_ENEMY, 1.1f, -0.1f, 130.0f / 180 * PI, 2, 7.0f, 3.0f));
	_waves["balloons-h"] = new Wave("balloons-h");
	curr_wave = _waves["balloons-h"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::HOMING_ENEMY, -0.1f, 0.2f, -150.0f / 180 * PI, 4, 7.0f, 2.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::HOMING_ENEMY, 0.2f, -0.1f, -160.0f / 180 * PI, 3, 8.0f, 1.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::HOMING_ENEMY, 0.4f, -0.1f, -170.0f / 180 * PI, 3, 8.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::HOMING_ENEMY, 0.6f, -0.1f, 170.0f / 180 * PI, 3, 8.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::HOMING_ENEMY, 0.8f, -0.1f, 160.0f / 180 * PI, 3, 8.0f, 1.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::HOMING_ENEMY, 1.1f, 0.2f, 150.0f / 180 * PI, 4, 7.0f, 2.0f));
	_waves["accel-v-e"] = new Wave("accel-v-e");
	curr_wave = _waves["accel-v-e"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, -0.1f, -0.1f, -140.0f / 180 * PI, 3, 4.5f, 3.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, 0.25f, -0.1f, -175.0f / 180 * PI, 2, 7.0f, 1.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, 0.75f, -0.1f, 175.0f / 180 * PI, 2, 7.0f, 1.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, 1.1f, -0.1f, 140.0f / 180 * PI, 3, 4.5f, 3.0f));
	_waves["accel-v-h"] = new Wave("accel-v-h");
	curr_wave = _waves["accel-v-h"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, -0.1f, -0.1f, -160.0f / 180 * PI, 6, 3.5f, 3.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, 0.1f, -0.1f, -175.0f / 180 * PI, 5, 5.5f, 2.5f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, 0.4f, -0.1f, 180.0f / 180 * PI, 4, 6.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, 0.6f, -0.1f, 180.0f / 180 * PI, 4, 6.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, 0.9f, -0.1f, 175.0f / 180 * PI, 5, 5.5f, 2.5f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, 1.1f, -0.1f, 160.0f / 180 * PI, 6, 3.5f, 3.0f));
	_waves["accel-h-e"] = new Wave("accel-h-e");
	curr_wave = _waves["accel-h-e"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, -0.1f, 0.3f, -110.0f / 180 * PI, 3, 2.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, -0.1f, 0.4f, -110.0f / 180 * PI, 3, 2.0f, 1.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, 1.1f, 0.35f, 110.0f / 180 * PI, 3, 2.0f, 0.5f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, 1.1f, 0.45f, 110.0f / 180 * PI, 3, 2.0f, 1.5f));
	_waves["accel-h-h"] = new Wave("accel-h-h");
	curr_wave = _waves["accel-h-h"];
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, -0.1f, 0.0f, -120.0f / 180 * PI, 6, 2.0f, 0.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, -0.1f, 0.1f, -120.0f / 180 * PI, 6, 2.0f, 0.5f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, -0.1f, 0.2f, -120.0f / 180 * PI, 6, 2.0f, 1.0f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, -0.1f, 0.3f, -120.0f / 180 * PI, 6, 2.0f, 1.5f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, 1.1f, 0.05f, 120.0f / 180 * PI, 6, 2.0f, 0.25f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, 1.1f, 0.15f, 120.0f / 180 * PI, 6, 2.0f, 0.75f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, 1.1f, 0.25f, 120.0f / 180 * PI, 6, 2.0f, 1.25f));
	curr_wave->add_spawnpoint(new Spawnpoint(Factory::ENEMY::ACCELERATING_ENEMY, 1.1f, 0.35f, 120.0f / 180 * PI, 6, 2.0f, 1.75f));

	Level* curr_level;
	levels[0] = new Level(0, "A new journey begins");
	curr_level = levels[0];
	curr_level->add_wave(0, _waves["simple-u-e"]->clone());
	curr_level->add_wave(17, _waves["simple-u-e"]->clone());
	curr_level->add_wave(33, _waves["simple-l-e"]->clone());
	curr_level->add_powerup(50, new PointsPowerup(get_canvas_width() * 0.5f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	
	levels[1] = new Level(1, "Watch out!");
	curr_level = levels[1];
	curr_level->add_wave(0, _waves["accel-v-e"]->clone());
	curr_level->add_wave(17, _waves["simple-r-e"]->clone());
	curr_level->add_wave(36, _waves["accel-v-e"]->clone());
	curr_level->add_wave(54, _waves["simple-l-m"]->clone());
	curr_level->add_wave(76, _waves["simple-u-m"]->clone());
	curr_level->add_powerup(96, new HealthPowerup(get_canvas_width() * 0.3f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(100, new PointsPowerup(get_canvas_width() * 0.5f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(98, new HealthPowerup(get_canvas_width() * 0.7f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));

	levels[2] = new Level(2, "Getting harder");
	curr_level = levels[2];
	curr_level->add_wave(0, _waves["tank-u"]->clone());
	curr_level->add_wave(16, _waves["simple-r-m"]->clone());
	curr_level->add_wave(38, _waves["accel-v-e"]->clone());
	curr_level->add_wave(53, _waves["simple-u-m"]->clone());
	curr_level->add_wave(66, _waves["accel-v-e"]->clone());
	curr_level->add_wave(81, _waves["tank-u"]->clone());
	curr_level->add_wave(85, _waves["simple-u-e"]->clone());
	curr_level->add_powerup(102, new HealthPowerup(get_canvas_width() * 0.3f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(104, new PointsPowerup(get_canvas_width() * 0.7f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(106, new ProjectilePowerup(get_canvas_width() * 0.5f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));

	levels[3] = new Level(3, "They are coming for you");
	curr_level = levels[3];
	curr_level->add_wave(0, _waves["balloons-e"]->clone());
	curr_level->add_wave(7, _waves["simple-l-m"]->clone());
	curr_level->add_wave(15, _waves["tank-l"]->clone());
	curr_level->add_wave(25, _waves["accel-h-e"]->clone());
	curr_level->add_wave(33, _waves["simple-u-m"]->clone());
	curr_level->add_wave(40, _waves["balloons-e"]->clone());
	curr_level->add_wave(52, _waves["tank-u"]->clone());
	curr_level->add_wave(60, _waves["simple-u-m"]->clone());
	curr_level->add_powerup(80, new HealthPowerup(get_canvas_width() * 0.5f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));

	levels[4] = new Level(4, "getting rolling");
	curr_level = levels[4];
	curr_level->add_wave(3, _waves["donuts"]->clone());
	curr_level->add_wave(15, _waves["simple-u-m"]->clone());
	curr_level->add_wave(23, _waves["tank-r"]->clone());
	curr_level->add_wave(38, _waves["accel-v-h"]->clone());
	curr_level->add_wave(53, _waves["balloons-m"]->clone());
	curr_level->add_wave(63, _waves["simple-r-m"]->clone());
	curr_level->add_wave(78, _waves["donuts"]->clone());
	curr_level->add_wave(88, _waves["accel-h-e"]->clone());
	curr_level->add_wave(94, _waves["simple-u-h"]->clone());
	curr_level->add_wave(104, _waves["tank-l"]->clone());
	curr_level->add_powerup(2, new HealthPowerup(get_canvas_width() * 0.6f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(0, new PointsPowerup(get_canvas_width() * 0.2f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(0, new PointsPowerup(get_canvas_width() * 0.5f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(0, new PointsPowerup(get_canvas_width() * 0.8f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(2, new HealthPowerup(get_canvas_width() * 0.4f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(113, new HealthPowerup(get_canvas_width() * 0.3f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(113, new HealthPowerup(get_canvas_width() * 0.7f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));

	levels[5] = new Level(5, "even more enemies");
	curr_level = levels[5];
	curr_level->add_wave(0, _waves["half-circles-u"]->clone());
	curr_level->add_wave(13, _waves["simple-r-e"]->clone());
	curr_level->add_wave(15, _waves["simple-l-e"]->clone());
	curr_level->add_wave(30, _waves["donuts"]->clone());
	curr_level->add_wave(35, _waves["tank-l"]->clone());
	curr_level->add_wave(60, _waves["balloons-m"]->clone());
	curr_level->add_wave(65, _waves["simple-u-e"]->clone());
	curr_level->add_wave(75, _waves["simple-u-e"]->clone());
	curr_level->add_wave(90, _waves["accel-v-e"]->clone());
	curr_level->add_wave(90, _waves["simple-u-e"]->clone());
	curr_level->add_wave(100, _waves["donuts"]->clone());
	curr_level->add_wave(105, _waves["simple-u-m"]->clone());
	curr_level->add_powerup(130, new HealthPowerup(get_canvas_width() * 0.3f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(130, new HealthPowerup(get_canvas_width() * 0.7f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(128, new PointsPowerup(get_canvas_width() * 0.5f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(132, new ProjectilePowerup(get_canvas_width() * 0.5f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));

	levels[6] = new Level(6, "almost there");
	curr_level = levels[6];
	curr_level->add_wave(0, _waves["half-circles-s"]->clone());
	curr_level->add_wave(15, _waves["half-circles-s"]->clone());
	curr_level->add_wave(7, _waves["simple-u-m"]->clone());
	curr_level->add_wave(20, _waves["tank-u"]->clone());
	curr_level->add_wave(27, _waves["balloons-h"]->clone());
	curr_level->add_wave(38, _waves["donuts"]->clone());
	curr_level->add_wave(35, _waves["simple-r-m"]->clone());
	curr_level->add_wave(35, _waves["simple-l-m"]->clone());
	curr_level->add_wave(52, _waves["half-circles-u"]->clone());
	curr_level->add_wave(56, _waves["tank-l"]->clone());
	curr_level->add_wave(56, _waves["tank-r"]->clone());
	curr_level->add_wave(62, _waves["simple-u-h"]->clone());
	curr_level->add_wave(73, _waves["accel-h-h"]->clone());
	curr_level->add_wave(83, _waves["simple-u-h"]->clone());
	curr_level->add_wave(88, _waves["half-circles-s"]->clone());
	curr_level->add_wave(98, _waves["tank-l"]->clone());
	curr_level->add_wave(108, _waves["balloons-h"]->clone());
	curr_level->add_wave(116, _waves["simple-l-m"]->clone());
	curr_level->add_wave(117, _waves["simple-r-m"]->clone());
	curr_level->add_powerup(138, new PointsPowerup(get_canvas_width() * 0.5f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(140, new HealthPowerup(get_canvas_width() * 0.3f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(140, new HealthPowerup(get_canvas_width() * 0.5f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(140, new HealthPowerup(get_canvas_width() * 0.7f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));

	levels[7] = new Level(7, "one last hold");
	curr_level = levels[7];
	curr_level->add_wave(0, _waves["simple-u-h"]->clone());
	curr_level->add_wave(5, _waves["donuts"]->clone());
	curr_level->add_wave(10, _waves["tank-l"]->clone());
	curr_level->add_wave(13, _waves["tank-r"]->clone());
	curr_level->add_wave(20, _waves["simple-u-h"]->clone());
	curr_level->add_wave(25, _waves["half-circles-s"]->clone());
	curr_level->add_wave(31, _waves["accel-h-h"]->clone());
	curr_level->add_wave(40, _waves["balloons-h"]->clone());
	curr_level->add_wave(47, _waves["simple-l-h"]->clone());
	curr_level->add_wave(47, _waves["simple-l-h"]->clone());
	curr_level->add_wave(60, _waves["tank-u"]->clone());
	curr_level->add_wave(63, _waves["simple-u-h"]->clone());
	curr_level->add_wave(65, _waves["donuts"]->clone());
	curr_level->add_wave(70, _waves["accel-v-h"]->clone());
	curr_level->add_wave(75, _waves["simple-u-h"]->clone());
	curr_level->add_wave(80, _waves["balloons-h"]->clone());
	curr_level->add_wave(85, _waves["half-circles-s"]->clone());
	curr_level->add_wave(95, _waves["tank-l"]->clone());
	curr_level->add_wave(95, _waves["simple-r-h"]->clone());
	curr_level->add_wave(105, _waves["accel-h-h"]->clone());
	curr_level->add_wave(108, _waves["balloons-h"]->clone());
	curr_level->add_wave(115, _waves["donuts"]->clone());
	curr_level->add_wave(117, _waves["tank-u"]->clone());

	levels[-10] = new Level(-10, "demo1");
	curr_level = levels[-10];
	curr_level->add_wave(0, _waves["simple-u-e"]->clone());
	curr_level->add_wave(16, _waves["simple-l-e"]->clone());
	curr_level->add_wave(30, _waves["balloons-e"]->clone());
	curr_level->add_wave(45, _waves["simple-u-e"]->clone());
	curr_level->add_wave(57, _waves["tank-u"]->clone());
	curr_level->add_powerup(72, new ProjectilePowerup(get_canvas_width() * 0.5f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(70, new HealthPowerup(get_canvas_width() * 0.3f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(70, new HealthPowerup(get_canvas_width() * 0.5f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(70, new HealthPowerup(get_canvas_width() * 0.7f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));

	levels[-9] = new Level(-9, "demo2");
	curr_level = levels[-9];
	curr_level->add_wave(0, _waves["donuts"]->clone());
	curr_level->add_wave(10, _waves["simple-u-e"]->clone());
	curr_level->add_wave(22, _waves["accel-h-e"]->clone());
	curr_level->add_wave(30, _waves["tank-l"]->clone());
	curr_level->add_wave(45, _waves["simple-r-m"]->clone());
	curr_level->add_wave(65, _waves["half-circles-s"]->clone());
	curr_level->add_powerup(87.5f, new ProjectilePowerup(get_canvas_width() * 0.8f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(87, new HealthPowerup(get_canvas_width() * 0.65f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(86.5f, new HealthPowerup(get_canvas_width() * 0.55f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(86, new HealthPowerup(get_canvas_width() * 0.45f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(85.5f, new HealthPowerup(get_canvas_width() * 0.35f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	curr_level->add_powerup(85, new PointsPowerup(get_canvas_width() * 0.2f, get_canvas_height() * 0.1f, -180.0f / 180 * PI));
	
	levels[-8] = new Level(-8, "demo3");
	curr_level = levels[-8];
	curr_level->add_wave(0, _waves["balloons-m"]->clone());
	curr_level->add_wave(14, _waves["simple-u-h"]->clone());
	curr_level->add_wave(22, _waves["half-circles-u"]->clone());
	curr_level->add_wave(32, _waves["accel-h-e"]->clone());
	curr_level->add_wave(42, _waves["donuts"]->clone());
	curr_level->add_wave(50, _waves["accel-v-e"]->clone());
	curr_level->add_wave(55, _waves["simple-u-e"]->clone());
	curr_level->add_wave(65, _waves["tank-u"]->clone());
}
