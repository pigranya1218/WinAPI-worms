#pragma once
#include "state.h"

class deadState : public state
{
private:
	image* _img;
	animation* _ani;
	
	bool _isBomb;
	bool _isGround;
	float _damage; // ������ �����
	float _bombWidth; // ������ ���� �ݰ�

public:
	deadState() {}
	~deadState() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);

	virtual bool isStop();
};

