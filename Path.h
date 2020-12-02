#pragma once
//name is capitalized due to VS reserving the lowercase name

//Interface for all Path classes
//Path is a Strategy changing its parent class x and y coordinates
class Path {
public:
	virtual void move(float & x,float & y, const int vel) = 0;
};

class StraightPath: public Path {
public:
	virtual void move(float& x, float& y, const int vel) override {
		x += vel; y += vel;
	}
};

class SwigglyPath : public Path {
//...
};