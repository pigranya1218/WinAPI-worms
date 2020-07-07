#pragma once
#include "state.h"

class longJumpState : public state
{
private:
	image* _img;
	animation* _ani;
	bool _ready;
	bool _isGround;
	float _angle = PI * 0.5; // 90 degree,  점프 각도
	float _power = 3.5; // 점프 파워
	float _xMove = 0.12;

public:
	longJumpState() {}
	~longJumpState(){}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);

	void setCurrentAni(animation* ani) { _ani = ani; };
};

