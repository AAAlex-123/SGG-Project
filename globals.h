#pragma once
#include "scancodes.h"
typedef graphics::scancode_t key;

const float PI = 3.14159265359f;

struct Keyset
{
	key up, down, left, right, rleft, rright, fire;
	Keyset(key up, key down, key left, key right, key rleft, key rright, key fire)
		: up(up), down(down), left(left), right(right), rleft(rleft), rright(rright), fire(fire) {}
};

float get_canvas_height();
float get_canvas_width();
