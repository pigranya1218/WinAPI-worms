#pragma once
#include "projectile.h"

class banana : public projectile
{
private:
	int _maxBombCount = 3;
	int _bombCount = 0;
	int _level;
	bool _isGround;
	float _powerChange = 0.7;
	animation* _ani;

	bool checkMoveAvail(int x, int bot);
	bool checkPixelAvail(int x, int bot);

public:
	void setLevel(int level) { _level = level; }
	
	void makeBanana(float angle);

	void setAnimation() {
		_ani = new animation;
		_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
		_ani->setDefPlayFrame(false, true);
		_ani->setFPS(60);
		_ani->start();
	}

	virtual bool gravityMove(float xPower); // true : ÇÈ¼¿°ú Ãæµ¹
	virtual void update();
	virtual void render();
};

