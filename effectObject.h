#pragma once
#include "object.h"

class effectObject : public object
{
protected:
	image* _img;
	animation* _ani;

	bool _effectedGravity;
	bool _effectedPixel;

	void move();
	bool movePixel();


public:
	void init(image* img, int fps, float x, float y, float width, float height, float angle, float power, bool effectedGravity, bool effectedPixel)
	{
		_img = img;
		_ani = new animation;
		_ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
		_ani->setDefPlayFrame(false, false);
		_ani->setFPS(fps);
		_ani->start();

		_x = x;
		_y = y;
		_width = width;
		_height = height;
		_angle = angle;
		_power = power;
		_effectedGravity = effectedGravity;
		_effectedPixel = effectedPixel;
	}

	virtual void release();

	virtual void update();

	virtual void render();

	void moveEffect();
};

