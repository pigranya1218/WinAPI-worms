#include "stdafx.h"
#include "state.h"
#include "worm.h"

string state::getImageKey(string key, SLOPE slope)
{
	string buffer;
	switch (slope)
	{
	case SLOPE::DOWN:
		buffer = "DOWN";
		break;
	case SLOPE::NORMAL:
		buffer = "NORMAL";
		break;
	case SLOPE::UP:
		buffer = "UP";
		break;
	}

	return key + "_" + buffer;
}

void state::enter(worm & player)
{
}

void state::exit(worm & player)
{
}

state * state::update(worm & player)
{
	return nullptr;
}

void state::render(worm & player)
{
}
