#include "stdafx.h"
#include "allWeapon.h"
#include "wormManager.h"

void bananaWeapon::shot(worm& shooter)
{
	banana* _projectile = new banana;

	float initGravity = 0;
	float initAngle = getRealAngle(shooter, _angle);
	float initPower = _power * ((float)_gauge / 100);

	float initX = shooter.getX() + (cosf(initAngle) * 15);
	float initY = shooter.getY() + (-sinf(initAngle) * 15);
	float widthX = 4;
	float widthY = 4;
	float initBombWidth = _bombWidth;
	float initDamage = _damage;

	_projectile->init(shooter.getStageManager(), shooter.getWormManager(), shooter.getObjectManager(), initX, initY, widthX, widthY, initAngle, initPower, initDamage, initBombWidth, true, true, true, true, true);
	_projectile->setImage(IMAGE_MANAGER->findImage("PROJECTILE_BANANA"));
	_projectile->setAnimation();
	_projectile->setLevel(2);

	shooter.getWormManager()->shoot(_projectile);
	shooter.adjustWeapon(WEAPON_CODE::BANANA, -1);
}

void bananaWeapon::enter(worm& player)
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
	_damage = 50; // 무기의 대미지
	_bombWidth = 160; // 무기의 폭파 반경
	_aimRadius = 90;

	_state = WEAPON_STATE::BEGIN;
}

WEAPON_FINISH_TYPE bananaWeapon::update(worm& player)
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
				_state = WEAPON_STATE::FINISH;
				player.setWaiting();
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
