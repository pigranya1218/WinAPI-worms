#include "stdafx.h"
#include "bazukaWeapon.h"
#include "worm.h"
#include "state.h"

void bazukaWeapon::shoot(worm& shooter)
{



}

void bazukaWeapon::enter(worm& player)
{
	_weaponImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_BAZUKA", player.getSlope()));
	_aimImg = IMAGE_MANAGER->findImage("WEAPON_AIM");
	_blobImg = IMAGE_MANAGER->findImage("WEAPON_BLOB");
	_gage = 0;
}

void bazukaWeapon::exit(worm& player)
{
	SAFE_DELETE(_ani);
}

WEAPON_FINISH_TYPE bazukaWeapon::update(worm& player)
{
	if (player.isTurn())
	{
		if (_isGaging)
		{
			if (KEY_MANAGER->isOnceKeyUp(VK_SPACE))
			{
				// 발사!!
				shoot(player);
				return WEAPON_FINISH_TYPE::FINISH;
			}
			else
			{
				_gage = min(100, _gage + 2); // 게이지 충전
			}
		}
		else
		{
			if (KEY_MANAGER->isOnceKeyDown(VK_SPACE))
			{
				_isGaging = true;
			}
			else if (KEY_MANAGER->isStayKeyDown(VK_UP))
			{
				_angle = min(PI, _angle + _angleOffset);
			}
			else if (KEY_MANAGER->isStayKeyDown(VK_DOWN))
			{
				_angle = max(0, _angle - _angleOffset);
			}
			else if (KEY_MANAGER->isOnceKeyDown(VK_LEFT) || KEY_MANAGER->isOnceKeyDown(VK_RIGHT)) // 이동을 시도하는 경우
			{
				return WEAPON_FINISH_TYPE::MOVING;
			} 
		}
	}
	else
	{
		return WEAPON_FINISH_TYPE::FINISH;
	}

	return WEAPON_FINISH_TYPE::ATTACK;
}

void bazukaWeapon::render(worm& player)
{
	if (_isGaging)
	{

	}
	
	RECT rc = player.getRect();

	float x = (rc.left + rc.right) / 2, y = (rc.top + rc.bottom) / 2;
	int frameIndex = floor(((_angle / _frameAngleOffset) + 1) / 2);

	_weaponImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_BAZUKA", player.getSlope()));
	CAMERA_MANAGER->frameRender(getMemDC(), _weaponImg, x - 30, y - 30, 0, frameIndex, ((player.getDirection() == DIRECTION::RIGHT) ? true : false));

	float realAngle = getRealAngle(player, _angle);
	float aimX = x + cosf(realAngle) * _aimRadius;
	float aimY = y - sinf(realAngle) * _aimRadius;
	CAMERA_MANAGER->frameRender(getMemDC(), _aimImg, aimX - (_aimImg->getFrameWidth() / 2), aimY - (_aimImg->getFrameHeight() / 2), 0, frameIndex, ((player.getDirection() == DIRECTION::RIGHT) ? true : false));

	int blobCount = floor(_gage / _blobOffset);
	float blobX = x + cosf(realAngle) * 16;
	float blobY = y - sinf(realAngle) * 16;
	for (int i = 0; i < blobCount; i++)
	{
		CAMERA_MANAGER->frameRender(getMemDC(), _blobImg, blobX - (_blobImg->getFrameWidth() / 2), blobY - (_blobImg->getFrameHeight() / 2), 0, i, ((player.getDirection() == DIRECTION::RIGHT) ? true : false));

		blobX += cosf(realAngle) * 3.5;
		blobY += -sinf(realAngle) * 3.5;
	}

	char _buffer[50];
	sprintf_s(_buffer, "angle : %.2f, frameIndex : %d", _angle, frameIndex);
	TextOut(getMemDC(), 10, 10, _buffer, strlen(_buffer));
}
