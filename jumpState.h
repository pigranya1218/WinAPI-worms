#pragma once
#include "state.h"
class jumpState : public state
{
private:
	image* _img;
	animation* _ani;
	bool _ready;
	bool _isGround;
	float _angle = PI * 0.2; // 36 degree,  점프 각도
	float _power = 2.5; // 점프 파워
	float _startX, _startY;

public:
	jumpState() {}
	~jumpState() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);
};

