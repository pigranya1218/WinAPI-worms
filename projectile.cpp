#include "stdafx.h"
#include "projectile.h"
#include "stageManager.h"
#include "wormManager.h"

bool projectile::checkPixelAvail(int x, int y)
{
	bool isBomb = false;

	// pixel이 있는지 검사
	COLORREF pixel = _stageManager->getPixel(x, y);
	int pixelR = GetRValue(pixel);
	int pixelG = GetGValue(pixel);
	int pixelB = GetBValue(pixel);
	if (!(pixelR == 255 && pixelG == 0 && pixelB == 255)) // 픽셀이 존재한다면
	{
		return true;
	}

	// worm이 있는지 검사
	RECT rc = RectMakeCenter(_x, _y, 4, 4);
	return _wormManager->checkCollisionPixel(rc);
}

int projectile::getFrameIndex()
{
	float frameAngle = _angleDisplay;

	frameAngle = PI2 - frameAngle; // reverse
	frameAngle += (PI / 2) ;

	if (frameAngle > PI2) frameAngle -= PI2;
	if (frameAngle < 0) frameAngle += PI2;

	int portion = frameAngle / _frameAngleOffset;
	int frameIndex = (portion + 1) % 32;
	
	return frameIndex;
}

bool projectile::gravityMove(float wind)
{
	_gravity += 0.08;
	_wind += wind;
	float deltaX = cosf(_angle) * _power + _wind;
	float deltaY = (-sinf(_angle) * _power) + _gravity;
	_angleDisplay = atan2f(-deltaY, deltaX);
	if (_angleDisplay < 0) _angleDisplay += PI2;
	if (_angleDisplay > PI2) _angleDisplay -= PI2;

	_x += deltaX;
	_y += deltaY;

	return checkPixelAvail(_x, _y);
}

void projectile::release()
{
}

void projectile::update()
{
}

void projectile::render()
{
	if (_ani == nullptr)
	{
		int frameIndex = getFrameIndex();
		CAMERA_MANAGER->frameRender(getMemDC(), _img, _x - (_img->getFrameWidth() / 2), _y - (_img->getFrameHeight() / 2), 0, frameIndex, false);
		
		char _buffer[100];
		sprintf_s(_buffer, "angle : %.2f, displayAngle : %.2f, frameIndex : %d", _angle, _angleDisplay, frameIndex);
		TextOut(getMemDC(), 10, 10, _buffer, strlen(_buffer));
	}
}
