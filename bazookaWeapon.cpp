#include "stdafx.h"
#include "bazookaWeapon.h"
#include "worm.h"
#include "state.h"
#include "stageManager.h"
#include "wormManager.h"

void bazookaWeapon::shot(worm& shooter)
{
	projectile* _projectile = new projectile;
	
	float initGravity = 0;
	float initAngle = getRealAngle(shooter, _angle);
	float initPower = _power * ((float) _gauge / 100);

	float initX = shooter.getX() + (cosf(initAngle) * 15);
	float initY = shooter.getY() + (-sinf(initAngle) * 15);
	float widthX = 4;
	float widthY = 4;
	float initBombWidth = _bombWidth;
	float initDamage = _damage;

	_projectile->init(shooter.getStageManager(),  shooter.getWormManager(), shooter.getObjectManager(), initX, initY, widthX, widthY, initAngle, initPower, initDamage, initBombWidth, true, true, true, true, true);
	_projectile->setImage(IMAGE_MANAGER->findImage("PROJECTILE_MISSILE"));
	
	shooter.getWormManager()->shoot(_projectile);
}

void bazookaWeapon::setWaiting(worm& player)
{
	_weaponImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_" + _weaponName + "_BACK", player.getSlope()));
	_ani->init(_weaponImg->getWidth(), _weaponImg->getHeight(), _weaponImg->getFrameWidth(), _weaponImg->getFrameHeight());
	_ani->setDefPlayFrame(false, false);
	_ani->setFPS(30);
	_ani->start();
	_state = WEAPON_STATE::WAITING;
	player.setWaiting();
}

void bazookaWeapon::enter(worm& player)
{
	_weaponImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_" + _weaponName + "_LINK", player.getSlope()));
	_ani = new animation;
	_ani->init(_weaponImg->getWidth(), _weaponImg->getHeight(), _weaponImg->getFrameWidth(), _weaponImg->getFrameHeight());
	_ani->setDefPlayFrame(false, false);
	_ani->setFPS(30);
	_ani->start();

	_aimImg = IMAGE_MANAGER->findImage("WEAPON_AIM");
	_blobImg = IMAGE_MANAGER->findImage("WEAPON_BLOB");

	_angle = PI / 2; // 무기의 각도 
	_angleOffset = PI / 31; // 무기 각도 오프셋
	_frameAngleOffset = _angleOffset / 2;
	_gauge = 0; // 무기의 충전률
	_power = 10; // 무기의 파워
	_damage = 30; // 무기의 대미지
	_bombWidth = 130; // 무기의 폭파 반경
	_aimRadius = 90;

	_state = WEAPON_STATE::BEGIN;
}

void bazookaWeapon::exit(worm& player)
{
	SAFE_DELETE(_ani);
}

WEAPON_FINISH_TYPE bazookaWeapon::update(worm& player)
{
	switch (_state)
	{
	case WEAPON_STATE::BEGIN:
	{
		if (player.isTurn())
		{
			if (!_ani->isPlay())
			{
				_state = WEAPON_STATE::IDLE;
			}
		}
		else
		{
			setWaiting(player);
		}
	}
	break;
	case WEAPON_STATE::IDLE:
	{
		if (player.isTurn())
		{
			if (KEY_MANAGER->isOnceKeyDown(VK_SPACE))
			{
				_state = WEAPON_STATE::GAUGING;
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
			else if (KEY_MANAGER->isOnceKeyDown(VK_LCONTROL))
			{
				return WEAPON_FINISH_TYPE::JUMPING;
			}
		}
		else
		{
			setWaiting(player);
		}
	}
	break;
	
	case WEAPON_STATE::GAUGING:
	{
		if (player.isTurn())
		{
			if (KEY_MANAGER->isStayKeyDown(VK_SPACE))
			{
				_gauge = min(100, _gauge + 2); // 게이지 충전
			}
			else
			{
				shot(player);
				setWaiting(player);
			}
		}
		else
		{
			setWaiting(player);
		}
	}
	break;
	case WEAPON_STATE::WAITING:
	{
		if (!_ani->isPlay())
		{
			_state = WEAPON_STATE::FINISH;
		}
	}
	break;
	case WEAPON_STATE::FINISH:
	{
		return WEAPON_FINISH_TYPE::FINISH;
	}
	break;
	}

	_ani->frameUpdate(TIME_MANAGER->getElapsedTime());
	return WEAPON_FINISH_TYPE::ATTACK;
}

void bazookaWeapon::render(worm& player)
{
	RECT rc = player.getRect();

	float x = (rc.left + rc.right) / 2, y = (rc.top + rc.bottom) / 2;

	switch (_state)
	{
	case WEAPON_STATE::BEGIN:
	case WEAPON_STATE::WAITING:
	case WEAPON_STATE::FINISH:
		CAMERA_MANAGER->aniRender(getMemDC(), _weaponImg, x - 30, y - 30, _ani, (player.getDirection() == DIRECTION::RIGHT));
		break;
	case WEAPON_STATE::IDLE:
	case WEAPON_STATE::GAUGING:
	case WEAPON_STATE::SHOOTING:
		int frameIndex = floor(((_angle / _frameAngleOffset) + 1) / 2);
		_weaponImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_" + _weaponName, player.getSlope()));
		CAMERA_MANAGER->frameRender(getMemDC(), _weaponImg, x - 30, y - 30, 0, frameIndex, ((player.getDirection() == DIRECTION::RIGHT) ? true : false));

		float realAngle = getRealAngle(player, _angle);
		float aimX = x + cosf(realAngle) * _aimRadius;
		float aimY = y - sinf(realAngle) * _aimRadius;
		CAMERA_MANAGER->frameRender(getMemDC(), _aimImg, aimX - (_aimImg->getFrameWidth() / 2), aimY - (_aimImg->getFrameHeight() / 2), 0, frameIndex, (player.getDirection() == DIRECTION::RIGHT));

		int blobCount = floor(_gauge / _blobOffset);
		float blobX = x + cosf(realAngle) * 16;
		float blobY = y - sinf(realAngle) * 16;
		for (int i = 0; i < blobCount; i++)
		{
			CAMERA_MANAGER->frameRender(getMemDC(), _blobImg, blobX - (_blobImg->getFrameWidth() / 2), blobY - (_blobImg->getFrameHeight() / 2), 0, i, (player.getDirection() == DIRECTION::RIGHT));

			blobX += cosf(realAngle) * 3.5;
			blobY += -sinf(realAngle) * 3.5;
		}
		break;
	}
}
