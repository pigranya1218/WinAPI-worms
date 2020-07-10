#pragma once
#include "state.h"

class deadState : public state
{
private:
	image* _img;
	animation* _ani;
	
	bool _isBomb;
	bool _isGround;
	float _damage; // 무기의 대미지
	float _bombWidth; // 무기의 폭파 반경

public:
	deadState() {}
	~deadState() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);

	virtual bool isStop();
};

