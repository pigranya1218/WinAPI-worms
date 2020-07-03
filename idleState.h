#pragma once
#include "state.h"
class idleState : public state
{
private:
	image* _img;
	animation* _ani;

public:
	idleState() {}
	~idleState() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);
};

