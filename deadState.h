#pragma once
#include "state.h"

class deadState : public state
{
private:

public:
	deadState() {}
	~deadState() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);
};

