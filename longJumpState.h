#pragma once
#include "state.h"

class longJumpState : public state
{
private:
	image* _img;
	animation* _ani;

public:
	longJumpState() {}
	~longJumpState(){}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);
};

