#include "stdafx.h"
#include "allWeapon.h"
#include "worm.h"
#include "state.h"
#include "stageManager.h"
#include "wormManager.h"

void homingWeapon::shot(worm& shooter)
{
	homing* _homing = new homing; // 유도 미사일

	float initGravity = 0;
	float initAngle = getRealAngle(shooter, _angle);
	float initPower = _power * ((float)_gauge / 100);

	float initX = shooter.getX() + (cosf(initAngle) * 15);
	float initY = shooter.getY() + (-sinf(initAngle) * 15);
	float widthX = 4;
	float widthY = 4;
	float initBombWidth = _bombWidth;
	float initDamage = _damage;

	_homing->init(shooter.getStageManager(), shooter.getWormManager(), shooter.getObjectManager(), initX, initY, widthX, widthY, initAngle, initPower, initDamage, initBombWidth, true, true, true, true, false);
	_homing->setPoint(_pointX, _pointY);
	_homing->setImage(IMAGE_MANAGER->findImage("PROJECTILE_HOMING_BLUE"));
	_homing->setAnimation();

	shooter.getWormManager()->shoot(_homing);
	shooter.adjustWeapon(WEAPON_CODE::HOMING, -1);
}


void homingWeapon::enter(worm& player)
{
	_weaponImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_" + _weaponName + "_LINK", player.getSlope()));
	_ani = new animation;
	_ani->init(_weaponImg->getWidth(), _weaponImg->getHeight(), _weaponImg->getFrameWidth(), _weaponImg->getFrameHeight());
	_ani->setDefPlayFrame(false, false);
	_ani->setFPS(30);
	_ani->start();

	_markerImg = IMAGE_MANAGER->findImage("WEAPON_MARKER_OKAY");
	_markerAni = new animation;
	_markerAni->init(_markerImg->getWidth(), _markerImg->getHeight(), _markerImg->getFrameWidth(), _markerImg->getFrameHeight());
	_markerAni->setDefPlayFrame(true, true);
	_markerAni->setFPS(40);
	_markerAni->start();

	_aimImg = IMAGE_MANAGER->findImage("WEAPON_AIM");
	_blobImg = IMAGE_MANAGER->findImage("WEAPON_BLOB");

	_angle = PI / 2; // 무기의 각도 
	_angleOffset = PI / 31; // 무기 각도 오프셋
	_frameAngleOffset = _angleOffset / 2;
	_gauge = 0; // 무기의 충전률
	_power = 12; // 무기의 파워
	_damage = 30; // 무기의 대미지
	_bombWidth = 130; // 무기의 폭파 반경
	_aimRadius = 90;

	_pointX = player.getX();
	_pointY = player.getY();

	_state = WEAPON_STATE::BEGIN;
}

void homingWeapon::exit(worm& player)
{
	SAFE_DELETE(_ani);
	SAFE_DELETE(_markerAni);
}

WEAPON_FINISH_TYPE homingWeapon::update(worm& player)
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
			if (KEY_MANAGER->isStayKeyDown(VK_LBUTTON))
			{
				_pointX = CAMERA_MANAGER->getAbsoluteL(_ptMouse.x);
				_pointY = CAMERA_MANAGER->getAbsoluteT(_ptMouse.y);
				SOUND_MANAGER->stop("EFFECT_CURSOR");
				SOUND_MANAGER->play("EFFECT_CURSOR", 1);
			}
			if (KEY_MANAGER->isOnceKeyDown(VK_SPACE))
			{
				_state = WEAPON_STATE::GAUGING;
				SOUND_MANAGER->stop("EFFECT_GAUGEUP");
				SOUND_MANAGER->play("EFFECT_GAUGEUP");
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
				SOUND_MANAGER->stop("EFFECT_GAUGEUP");
				SOUND_MANAGER->stop("EFFECT_SHOOT");
				SOUND_MANAGER->play("EFFECT_SHOOT");

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
	_markerAni->frameUpdate(TIME_MANAGER->getElapsedTime());
	return WEAPON_FINISH_TYPE::ATTACK;
}

void homingWeapon::render(worm& player)
{
	bazookaWeapon::render(player);

	switch (_state)
	{
	case WEAPON_STATE::IDLE:
	case WEAPON_STATE::SHOOTING:
	case WEAPON_STATE::WAITING:
	case WEAPON_STATE::FINISH:
	{
		CAMERA_MANAGER->aniRender(getMemDC(), _markerImg, _pointX - (_markerImg->getFrameWidth() / 2), _pointY - (_markerImg->getFrameHeight() / 2), _markerAni, false);
	}
	break;
	}
}
