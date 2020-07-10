#include "stdafx.h"
#include "projectile.h"
#include "stageManager.h"
#include "wormManager.h"

bool projectile::checkPixelAvail(int x, int y)
{
	bool isBomb = false;

	RECT rc = RectMakeCenter(_x, _y, _width, _height);

	// pixel�� �ִ��� �˻�
	for (int x = rc.left; x < rc.right; x++)
	{
		for (int y = rc.top; y < rc.bottom; y++)
		{
			COLORREF pixel = _stageManager->getPixel(x, y);
			int pixelR = GetRValue(pixel);
			int pixelG = GetGValue(pixel);
			int pixelB = GetBValue(pixel);
			if (!(pixelR == 255 && pixelG == 0 && pixelB == 255)) // �ȼ��� �����Ѵٸ�
			{
				return true;
			}
		}
	}

	// worm�� �ִ��� �˻�
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
	if (!_isFinish)
	{
		float wind = _stageManager->getWind() * 0.2;
		bool isBomb = gravityMove(wind);

		if (isBomb) // ���Ľ�Ű��
		{
			EFFECT_MANAGER->play("EFFECT_CIRCLE", _x, _y, _bombWidth, _bombWidth);
			EFFECT_MANAGER->play("EFFECT_ELIPSE", _x, _y, _bombWidth + 30, _bombWidth + 30);
			EFFECT_MANAGER->play("EFFECT_EX_POW", _x, _y - 50, 50, 50);

			_stageManager->pixelBomb(_x, _y, _damage, _bombWidth); // �ȼ� ���Ľ�Ű��
			_wormManager->wormBomb(_x, _y, _damage, _bombWidth); // ���� ���� ���󰡰� �ϱ�

			_isFinish = true;
		}
	}
}

void projectile::render()
{
	if (_isAngleFrame)
	{
		int frameIndex = getFrameIndex();
		CAMERA_MANAGER->frameRender(getMemDC(), _img, _x - (_img->getFrameWidth() / 2), _y - (_img->getFrameHeight() / 2), 0, frameIndex, false);
	}
	else
	{
		CAMERA_MANAGER->render(getMemDC(), _img, _x - (_img->getWidth() / 2), _y - (_img->getHeight() / 2));
	}
	
}
