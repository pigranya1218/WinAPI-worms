#include "stdafx.h"
#include "effectObject.h"

void effectObject::move()
{
	if (_effectedGravity) _gravity += 0.08;
	float deltaX = cosf(_angle) * _power;
	float deltaY = (-sinf(_angle) * _power) + _gravity;

	_x += deltaX;
	_y += deltaY;
}

bool effectObject::movePixel()
{
	return false;
}

void effectObject::release()
{
	SAFE_DELETE(_ani);
}

void effectObject::update()
{
	_isUpdate = true;
	_isFinish = false;
	if (!_ani->isPlay())
	{
		_isFinish = true;
		return;
	}
	move();
	_ani->frameUpdate(TIME_MANAGER->getElapsedTime());
}

void effectObject::render()
{
	CAMERA_MANAGER->aniRender(getMemDC(), _img, _x - (_width / 2), _y - (_height / 2), _width, _height, _ani, false);
}

void effectObject::moveEffect()
{
	if (_effectedPixel)
	{
		bool isGround = movePixel();
		if (isGround) // 튕길 수 있도록
		{
			_gravity = 0;
		}
	}
	else
	{
		move();
	}
}
