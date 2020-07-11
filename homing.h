#pragma once
#include "projectile.h"

class homing : public projectile
{
private:
	int _homingCount = 0;
	animation* _ani;

	bool _isHoming = false;

	float _pointX;
	float _pointY;
	float _pointAngle;

public:
	homing() {}
	~homing() { SAFE_DELETE(_ani); }



	void setPoint(float x, float y) { _pointX = x, _pointY = y; }
	void setAnimation() {
		_ani = new animation;
		_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
		_ani->setDefPlayFrame(false, true);
		_ani->setFPS(120);
		_ani->start();
	}
	virtual void update();
	virtual void render();
};

