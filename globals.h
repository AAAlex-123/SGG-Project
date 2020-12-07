#pragma once
#include "scancodes.h"
typedef graphics::scancode_t key;

const float PI = 3.14159265359f;

struct Keyset
{
	key up, down, left, right, rleft, rright;
	Keyset(key up, key down, key left, key right, key rleft, key rright)
		: up(up), down(down), left(left), right(right), rleft(rleft), rright(rright) {}
};
