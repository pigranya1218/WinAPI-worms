#include "stdafx.h"
#include "worm.h"
#include "allState.h"

HRESULT worm::init(float x, float y)
{
	_state = new idleState();
	_x = x;
	_y = y;
	return S_OK;
}

void worm::release()
{

}

void worm::update()
{
	state* state = _state->update(*this);
	if (state != nullptr)
	{
		_state->exit(*this);
		state->enter(*this);
		delete _state;
		_state = state;
	}
}

void worm::render()
{
	_state->render(*this);
}
