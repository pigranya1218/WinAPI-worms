#include "stdafx.h"
#include "allWeapon.h"
#include "worm.h"
#include "stageManager.h"
#include "objectManager.h"

void mineWeapon::shot(worm& shooter)
{
	mineObject* mine = new mineObject;

	float initAngle = (shooter.getDirection() == DIRECTION::LEFT)?(PI * 0.9):(PI * 0.1);
	float initPower = 2;

	float initX = shooter.getX() + (cosf(initAngle) * 10);
	float initY = shooter.getY() + (-sinf(initAngle) * 10);

	mine->init(shooter.getStageManager(), shooter.getWormManager(), shooter.getObjectManager(), initX, initY, initAngle, initPower);
	shooter.getObjectManager()->pushObject(mine);
	shooter.adjustWeapon(WEAPON_CODE::MINE, -1);
}

void mineWeapon::setWaiting(worm& player)
{
	_wormImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_MINE_BACK", player.getSlope()));
	_wormAni->setDefPlayFrame(false, false);
	_wormAni->start();

	_state = WEAPON_STATE::WAITING;
}

void mineWeapon::enter(worm& player)
{
	_wormImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_MINE_LINK", player.getSlope()));
	_wormAni = new animation;
	_wormAni->init(_wormImg->getWidth(), _wormImg->getHeight(), _wormImg->getFrameWidth(), _wormImg->getFrameHeight());
	_wormAni->setDefPlayFrame(false, false);
	_wormAni->setFPS(30);
	_wormAni->start();
}

void mineWeapon::exit(worm& player)
{
	SAFE_DELETE(_wormAni);
}

WEAPON_FINISH_TYPE mineWeapon::update(worm& player)
{
	switch (_state)
	{
	case WEAPON_STATE::BEGIN:
	{
		if (player.isTurn())
		{
			if (!_wormAni->isPlay())
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
				shot(player);
				_state = WEAPON_STATE::FINISH;
			}
			if (KEY_MANAGER->isOnceKeyDown(VK_LEFT) || KEY_MANAGER->isOnceKeyDown(VK_RIGHT)) // 이동을 시도하는 경우
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
			player.setWaiting();
		}
	}
	break;
	case WEAPON_STATE::WAITING:
	{
		if (!_wormAni->isPlay())
		{
			_state = WEAPON_STATE::FINISH;
		}
	}
	break;
	case WEAPON_STATE::FINISH:
	{
		return WEAPON_FINISH_TYPE::FINISH_BUT_MOVE;
	}
	break;
	}

	_wormAni->frameUpdate(TIME_MANAGER->getElapsedTime());
	return WEAPON_FINISH_TYPE::ATTACK;
}

void mineWeapon::render(worm& player)
{
	CAMERA_MANAGER->aniRender(getMemDC(), _wormImg, player.getX() - (_wormImg->getFrameWidth() / 2), player.getY() - (_wormImg->getFrameHeight() / 2), _wormAni, (player.getDirection() == DIRECTION::RIGHT));
}
