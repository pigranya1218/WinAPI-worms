#pragma once
#include "state.h"

class moveState : public state
{
private:

public:
	moveState() {}
	~moveState() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);
};

