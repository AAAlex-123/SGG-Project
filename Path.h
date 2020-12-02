#pragma once

//Interface for all Path classes
//Path is a Strategy changing its parent class' x and y coordinates
class Path {
public:
	virtual void move(float& x, float& y, const int vel) = 0;
};

class StraightPath: public Path {
public:
	virtual void move(float& x, float& y, const int vel) override {
		y += vel;
	}
};

class SwigglyPath : public Path {
	//...
};

// special case for player
class PlayerPath : public Path {
public:
	virtual void move(float& x, float& y, const int vel) override {
		;
	}
};
