#pragma once
#include "state.h"
class jumpState : public state
{
private:
	image* _img;
	animation* _ani;
	bool _ready;
	bool _isGround;
	float _angle = PI * 0.25; // 45 degree,  점프 각도
	float _power = 2.3; // 점프 파워
	bool _transToLong;

public:
	jumpState() {}
	~jumpState() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);
};

