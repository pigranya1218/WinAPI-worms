#include "stdafx.h"
#include "allState.h"
#include "worm.h"

void idleState::enter(worm & player)
{
	_img = IMAGE_MANAGER->findImage(getImageKey("IDLE_BIGHEAD", player.getSlope()));
	_ani = new animation;
	_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
	_ani->setDefPlayFrame(true, true);
	_ani->setFPS(13);
	_ani->start();
}

void idleState::exit(worm & player)
{
	SAFE_DELETE(_ani);
}

state * idleState::update(worm & player)
{
	if (player.isTurn())
	{
		if (KEY_MANAGER->isOnceKeyDown(VK_LEFT)) // �������� �����̱�
		{
			player.setDirection(DIRECTION::LEFT);
			return new moveState;
		}
		else if (KEY_MANAGER->isOnceKeyDown(VK_RIGHT)) // ���������� �����̱�
		{
			player.setDirection(DIRECTION::RIGHT);
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

void idleState::render(worm & player)
{
	RECT rc = player.getRect();

	float x = (rc.left + rc.right) / 2, y = (rc.top + rc.bottom) / 2;
	POINT pos = _ani->getFramePos();
	
	// ������
	// CAMERA_MANAGER->rectangle(getMemDC(), rc);

	CAMERA_MANAGER->aniRender(getMemDC(), _img, x - 30, y - 28, _ani, ( (player.getDirection() == DIRECTION::RIGHT)? true : false) );
}
