#include "game_data.h"
#include "GObjFactory.h"
#include <iostream>
#include <regex>
#include <fstream>

using namespace std;

GameData::Stats GameData::game_stats = Stats();

const array<GameData::Achievement const *, 4> GameData::achievements = {
	new GameData::Achievement("Kill 100 total enemies", image_path + "achievement_1.png", "Kill a total of 100 enemies of any type.",Stats::ALL,100),
	new GameData::Achievement("Destroy 20 balloons", image_path + "achievement_2.png", "Balloons are well-protected floating enemies.",Stats::BALLOON,20),
	new GameData::Achievement("Destroy an American airship", image_path + "achievement_3.png","American airships are well armoured and have excellent targeting.",Stats::AIRSHIP,1),
	new GameData::Achievement("Destroy 50 british fighters", image_path + "achievement_4.png","You can recognise British fighters by their black color.",Stats::BLACK_PLANE,50)
};


GameData::GameData()
	: fps(0), game_state(0),
	el(0.0f), sps(20.0f), curr_img(0), images(),
	levels(std::unordered_map<int, Level*>()), _waves(std::unordered_map<std::string, Wave*>()),
	curr_active_level(-1), curr_selected_level(-1),
	bg_offset(0.0f), height_perc_per_second(0.02f),
	curr_playing_level(-1), level_transition_timer(set_level_transition_timer()),
	enemyLs(new list<Entity*>), playerLs(new list<Entity*>), enemyProjLs(new list<Projectile*>),
	playerProjLs(new list<Projectile*>), effectsLs(new list<VisualEffect*>),  powerupLs(new list<Powerup*>), buttons(new list<Button*>)
{
	// sets all the user-selectable levels to nullptr
	// to check later if a user-selectable level has been
	// defined (setting the nullptr to Level*) by calling `levels[id]`
	for (int i = 0; i < 10; ++i)
		levels[i] = nullptr;

	if (!load_level_data_from_file(level_path, wave_path))
	{
		std::cerr << "Warning: Level loading from files failed, loading hardcoded levels" << std::endl;
		_load_hardcoded_levels();
	}
	std::cout << "Levels loaded successfully" << std::endl;

	create_buttons();
}

void GameData::updateBackground(float ms)
{
	bg_offset = (bg_offset < get_canvas_width() / get_canvas_height())
		? (bg_offset + (height_perc_per_second * (ms / 1000.0f)))
		: (0.0f);
}

void GameData::drawBackground(graphics::Brush& br)
{
	br.texture = image_path + "background.png";
	setColor(br, new float[3]{ 1.0f, 1.0f, 1.0f });
	br.outline_opacity = 0.0f;
	float cw = get_canvas_width(), ch = get_canvas_height();
	graphics::drawRect(cw / 2, ch * (bg_offset - (cw / ch)), cw, cw, br);
	graphics::drawRect(cw / 2, ch * bg_offset, cw, cw, br);
	graphics::drawRect(cw / 2, ch * (bg_offset + (cw / ch)), cw, cw, br);
}

void GameData::create_buttons()
{
	// menu
	buttons->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "exit.png"), game_states::MENU, game_states::EXIT));
	buttons->push_back(new GameStateChangingButton(this, 370.0f, 30.0f, 30.0f, new string(icon_path + "help.png"), game_states::MENU, game_states::HELP));
	buttons->push_back(new GameStateChangingButton(this, 370.0f, 75.0f, 30.0f, new string(icon_path + "options.png"), game_states::MENU, game_states::OPTIONS));
	buttons->push_back(new GameStateChangingButton(this, 370.0f, 120.0f, 30.0f, new string(icon_path + "credits.png"), game_states::MENU, game_states::CREDITS));
	// game
	buttons->push_back(new GameStateChangingButton(this, 370.0f, 30.0f, 30.0f, new string(icon_path + "pause.png"), game_states::GAME, game_states::PAUSE));
	// === temp ===
	buttons->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), game_states::GAME, game_states::MENU));
	// === temp ===
		// game -- pause
	buttons->push_back(new GameStateChangingButton(this, 370.0f, 30.0f, 30.0f, new string(icon_path + "continue.png"), game_states::PAUSE, game_states::GAME));
	// game -- lose
	buttons->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), game_states::GAME_LOSE, game_states::RESET));
	// game -- win
	buttons->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), game_states::GAME_WIN, game_states::RESET));
	// help
	buttons->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), game_states::HELP, game_states::MENU));
	// credits
	buttons->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), game_states::CREDITS, game_states::MENU));
	// options
	buttons->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), game_states::OPTIONS, game_states::MENU));
	buttons->push_back(new GameStateChangingButton(this, 200.0f, 100.0f, 100.0f, new string(icon_path + "level_select.png"), game_states::OPTIONS, game_states::OP_LEVEL));
	buttons->push_back(new GameStateChangingButton(this, 200.0f, 350.0f, 100.0f, new string(icon_path + "player_select.png"), game_states::OPTIONS, game_states::OP_PLAYER));
	// options -- player
	buttons->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), game_states::OP_PLAYER, game_states::OPTIONS));
	buttons->push_back(new VariableChangingButton<bool>(this, 112.5f, 200.0f, 125.0f, new string(icon_path + "singleplayer.png"), game_states::OP_PLAYER, &this->isMult, false));
	buttons->push_back(new VariableChangingButton<bool>(this, 287.5f, 200.0f, 125.0f, new string(icon_path + "multiplayer.png"), game_states::OP_PLAYER, &this->isMult, true));
	// options -- level
	buttons->push_back(new GameStateChangingButton(this, 30.0f, 30.0f, 30.0f, new string(icon_path + "back.png"), game_states::OP_LEVEL, game_states::OPTIONS));
}

void GameData::click_buttons()
{
	for (Button* b : *buttons)
	{
		b->execute();
	}
}

bool GameData::load_level_data_from_file(const std::string& level_path, const std::string& wave_path) {

	// first load waves
	if (!_load_waves_from_file(wave_path))
	{
		std::cerr << "Warning: Unable to load waves from file: " + level_path << std::endl;
		return false;
	}
	// then load levels; the levels' waves must have the same name as the waves loaded above
	if (!_load_levels_from_file(level_path))
	{
		std::cerr << "Warning: Unable to load levels from file: " + level_path << std::endl;
		return false;
	}
	return true;
}

bool GameData::_load_waves_from_file(const std::string& wave_path)
{
	std::cout << "Loading waves..." << std::endl;
	// create stream to levels_path
	std::ifstream in(wave_path);

	if (!in) {
		std::cerr << "Error opening file '" << wave_path << "'" << std::endl;
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
			_waves[curr_wave]->add_spawnpoint(new Spawnpoint(
				stoi(match[1]), stof(match[2]), stof(match[3]), stof(match[4]) / 180 * PI,
				stoi(match[5]), stof(match[6]), stof(match[7])
			));
		}
		// eof
		else if (std::regex_search(contents, match, eof))
		{ break; }
		// comment
		else if (std::regex_search(contents, match, comment))
		{ ; }
		// no match and not eof -> invalid line syntax
		else
		{
			std::cerr << "Error: Line " << line << " has invalid syntax or was not found" << std::endl;
			return false;
		}

		++line;
		contents = match.suffix();
	}

	// close stream
	in.close();
	return true;
}

bool GameData::_load_levels_from_file(const std::string& level_path)
{
	std::cout << "Loading levels..." << std::endl;
	// create stream to levels_path
	std::ifstream in(level_path);

	if (!in) {
		std::cerr << "Error opening file '" << level_path << "'" << std::endl;
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

	int curr_level_id = 0, line = 0;

	while (true)
	{
		// construct regexes ...
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
		{ break; }
		// comment
		else if (std::regex_search(contents, match, comment))
		{ ; }
		// no match and not eof -> invalid line syntax
		else
		{
			std::cerr << "Error: Line " << line << " has invalid syntax or was not found" << std::endl;
			return false;
		}

		++line;
		contents = match.suffix();
	}

	// close stream
	in.close();
	return true;
}

void GameData::_load_hardcoded_levels()
{
	Spawnpoint* sp11 = new Spawnpoint(3, 0.0f, 0.5, -PI / 2, 10, 1.0f, 0.0f);

	Wave* w1 = new Wave("line");
	w1->add_spawnpoint(sp11);

	Spawnpoint* sp21 = new Spawnpoint(2, 0.2f, 0.1f, 0.0f, 2, 1.5f, 1.0f);
	Spawnpoint* sp22 = new Spawnpoint(2, 0.5f, 0.1f, 0.0f, 2, 3.0f, 1.0f);
	Spawnpoint* sp23 = new Spawnpoint(2, 0.8f, 0.1f, 0.0f, 2, 5.0f, 1.0f);

	Wave* w2 = new Wave("donuts");
	w2->add_spawnpoint(sp21);
	w2->add_spawnpoint(sp22);
	w2->add_spawnpoint(sp23);

	Level* l1 = new Level(-2, "owo");
	l1->add_wave(5.0f, w1);
	l1->add_wave(10.0f, w2);
	
/*	ABOUT LEVEL ID
	=>	no level can have an id of -1 (because -1 is the default)

	=>	levels selectable by users must have id 0-9 inclusive
		(see update::OP_LEVEL case for selectable levels)

	=>	levels not be selectable by users must have any other id
		(see above and also l1 and update::TEST case)

	=>	level id should always match its key in the levels map (see below)
*/
	levels[l1->id()] = l1;

	// dummy levels to see the level select screen
	Level *l4 = new Level(4, "xd");
	Level* l6 = new Level(6, "uwu");

	levels[l4->id()] = l4;
	levels[l6->id()] = l6;
}

Level* GameData::get_next_level() {
	Level* return_val = levels[curr_playing_level - 1];
	if (!return_val)
		levels.erase(curr_playing_level - 1);
	return return_val;
}

void GameData::addScore(int scored) {
	score += scored;
}

int GameData::getScore() const {
	return score;
}

void GameData::spawn() {
	if (levels[curr_playing_level]->can_spawn())
		enemyLs->push_back(levels[curr_playing_level]->spawn());
	if (levels[curr_playing_level]->can_spawn_p())
		powerupLs->push_back(levels[curr_playing_level]->spawn_p());
}

GameData::~GameData() {
	deleteList(playerLs);
	deleteList(enemyLs);
	deleteList(enemyProjLs);
	deleteList(playerProjLs);
	deleteList(effectsLs);
	deleteList(buttons);
}

//=======INNER CLASSES========

int GameData::Stats::find_type(const Entity const* en) const {
	const std::string name = *en->getSprite();
	if (name == image_path + "plane1.png")
		return BASIC_PLANE;
	else if (name == image_path + "plane2.png")
		return BLACK_PLANE;
	else if (name == image_path + "plane3.png")
		return AIRSHIP;
	else if (name == image_path + "balloon.png")
		return BALLOON;
	else {
		std::cerr << "Entity can't be identified in Stats (sprite name = " << name << ")" << std::endl;
		return BASIC_PLANE;
	}
}

void GameData::Stats::plane_shot(const Entity const* en) {
	shot_down_arr[find_type(en)] ++;
}

int GameData::Stats::get_shot_number(int type) const {
	try {
		return shot_down_arr[type];
	}
	catch (std::out_of_range) {
		std::cerr << " Couldn't find enemy type information in get_shot_number, returning 0" << std::endl;
		return 0;
	}
}

int GameData::Stats::get_total_shot() const {
	int total_shot = 0;
	for (int shot_no : shot_down_arr)
		total_shot += shot_no;
	return total_shot;
}

GameData::Achievement::Achievement(std::string name, std::string icon, std::string description,int type, int kills) :
	name(name),icon(icon),description(description), TYPE(type), KILLS(kills)
{}

bool GameData::Achievement::is_achieved() const {
	if (TYPE == Stats::ALL) 
		return KILLS < game_stats.get_total_shot();
	else 
		return KILLS < game_stats.get_shot_number(TYPE);
}
