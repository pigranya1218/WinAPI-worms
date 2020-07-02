#pragma once
#include "state.h"
class jumpState : public state
{
private:

public:
	jumpState() {}
	~jumpState() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);
};

