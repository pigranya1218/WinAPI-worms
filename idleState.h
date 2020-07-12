#pragma once
#include "state.h"
class idleState : public state
{
private:
	image* _img;
	animation* _ani;
	bool _isDeadly = false;

public:
	idleState() {}
	~idleState() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);

	virtual bool isStop() { return true; }
};

