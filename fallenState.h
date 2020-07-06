#pragma once
#include "state.h"

class fallenState : public state
{
private:
	image* _img;
	animation* _ani;
	bool _isGround;
	float _gravity;
	float _startX, _startY;

public:
	fallenState() {}
	~fallenState() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);
};

