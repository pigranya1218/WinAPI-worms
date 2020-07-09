#pragma once
#include "state.h"

enum class FLY_STATE
{
	FLY, // ����ٴϰ� �ִ� ����
	SLIDING, // �̲������� �ִ� ����
	TO_IDLE // �����ϱ�
};

class flyState : public state
{
private:
	image* _img;
	animation* _ani;

	FLY_STATE _state;
	bool _isGround;

	float _angleOffset = PI2 / 31;
	float _frameAngleOffset = _angleOffset / 2;
	char _buffer[50];
	int _currFrame = 0;
	int _currFrameCount = 0;

	float getFrameAngle(worm& player);
	int getYFrameIndex(worm& player);

public:
	flyState() {}
	~flyState() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);
};

