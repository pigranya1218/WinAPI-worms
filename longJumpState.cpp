#include "stdafx.h"
#include "allState.h"
#include "worm.h"

void longJumpState::enter(worm & player)
{
	_img = IMAGE_MANAGER->findImage(getImageKey("IDLE_BIGHEAD", player.getSlope()));
	_ani = new animation;
	_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
	_ani->setDefPlayFrame(true, true);
	_ani->setFPS(13);
	_ani->start();
}

void longJumpState::exit(worm & player)
{
	SAFE_DELETE(_ani);
}

state * longJumpState::update(worm & player)
{
	if (player.isTurn())
	{
		if (KEY_MANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			player.setPlayerPos(CAMERA_MANAGER->getAbsoluteL(_ptMouse.x), CAMERA_MANAGER->getAbsoluteT(_ptMouse.y));
			return nullptr;
		}
		if (KEY_MANAGER->isOnceKeyDown(VK_LEFT)) // �������� �����̱�
		{
			if (player.getDirection() == DIRECTION::RIGHT)
			{
				player.setDirection(DIRECTION::LEFT);
				player.reverseSlope();
			}
			return new moveState;
		}
		else if (KEY_MANAGER->isOnceKeyDown(VK_RIGHT)) // ���������� �����̱�
		{
			if (player.getDirection() == DIRECTION::LEFT)
			{
				player.setDirection(DIRECTION::RIGHT);
				player.reverseSlope();
			}
			return new moveState;
		}
		else if (KEY_MANAGER->isOnceKeyDown(VK_SPACE)) // �����ϱ�
		{
			return new jumpState;
		}
	}
	else // �������� IDLE �ִϸ��̼� ���
	{

	}

	_ani->frameUpdate(TIME_MANAGER->getElapsedTime()); // ���°� �ٲ��� ���� ��� ������ ������Ʈ
	return nullptr;
}

void longJumpState::render(worm & player)
{
	RECT rc = player.getRect();

	float x = (rc.left + rc.right) / 2, y = (rc.top + rc.bottom) / 2;
	POINT pos = _ani->getFramePos();

	_img = IMAGE_MANAGER->findImage(getImageKey("IDLE_BIGHEAD", player.getSlope()));
	CAMERA_MANAGER->aniRender(getMemDC(), _img, x - 30, y - 30, _ani, ((player.getDirection() == DIRECTION::RIGHT) ? true : false));
}
