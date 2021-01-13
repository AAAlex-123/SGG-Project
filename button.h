#pragma once
#include "globals.h"
#include "drawing.h"

class GameData;

/**
* Αn interface for buttons that, when clicked, perform an action
*/
class Button : public Drawing
{
public:
	Button(GameData* const, float x, float y, float size, std::string*, GAME_STATE targetGS);

	virtual void update(float) override final;
	virtual void draw() override final;

	// template method: this called by other classes
	virtual void execute() const final;

	// defined
	virtual operator bool() const override;

protected:
	GameData* const gd;
	GAME_STATE targetGS;	// the gamestate where the button will work
	bool clicked;
	graphics::MouseState ms;
	
	// template method: this one is specified by subclasses
	virtual void execute_() const = 0;
};

class GameStateChangingButton : public Button
{
public:
	GameStateChangingButton(GameData* const, float, float, float, std::string*, GAME_STATE, GAME_STATE newGS);

protected:
	virtual void execute_() const override;

private:
	GAME_STATE newGS;
};

template<typename T>
class VariableChangingButton : public Button
{
public:
	VariableChangingButton(GameData* const gd, float x, float y, float size, std::string* icon, GAME_STATE targetGS, T* attribute, T value)
		: Button(gd, x, y, size, icon, targetGS), attr(attribute), val(value)
	{}

protected:
	virtual void execute_() const override
	{
		*attr = val;
	}

private:
	T* const attr;
	T val;
};
