#pragma once
#include "state.h"

class attackState : public state
{
private:


public:
	attackState() {}
	~attackState() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);

	virtual bool isStop() { return false; }
};

