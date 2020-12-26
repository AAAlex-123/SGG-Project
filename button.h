#pragma once
#include "globals.h"
#include "drawing.h"

class GameData;

// not really, drawing has more stuff that button doesn't need
// but gd->draw() would like this inheritance to happen
class Button : public Drawing
{
protected:
	GameData* gd;
	int targetGS;
	bool clicked;
	graphics::MouseState ms;

public:
	Button(GameData*, float, float, float, std::string*, int targetGS);

	virtual void update(float) override final;
	virtual void draw() override final;

	// this one is called
	virtual void execute() const final;
	// this one is specified by subclasses
	virtual void execute_() const = 0;

	// so inheritance works lmao
	virtual operator bool() const override { return true; }
};

class GameStateChangingButton : public Button
{
private:
	int newGS;

public:
	GameStateChangingButton(GameData*, float, float, float, std::string*, int, int newGS);

	virtual void execute_() const override;
};

template<typename T>
class VariableChangingButton : public Button
{
private:
	T* attr;
	T val;

public:
	VariableChangingButton(GameData* gd, float x, float y, float size, std::string* icon, int targetGS, T* attribute, T value)
		: Button(gd, x, y, size, icon, targetGS), attr(attribute), val(value)
	{}

	virtual void execute_() const override
	{
		*attr = val;
	}
};
