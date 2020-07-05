#include "stdafx.h"
#include "allState.h"
#include "worm.h"

void moveState::enter(worm & player)
{
	_img = IMAGE_MANAGER->findImage(getImageKey("MOVE", player.getSlope()));
	_ani = new animation;
	_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
	_ani->setDefPlayFrame(false, true);
	_ani->setFPS(20);
	_ani->start();
}

void moveState::exit(worm & player)
{
	SAFE_DELETE(_ani);
}

state * moveState::update(worm & player)
{
	if (!player.isTurn())
	{
		return new idleState;
	}

	if (KEY_MANAGER->isStayKeyDown(VK_LEFT)) // �������� �����ؼ� �̵�
	{
		if (player.getDirection() == DIRECTION::RIGHT)
		{
			player.setDirection(DIRECTION::LEFT);
			player.reverseSlope();
		}
	}
	else if (KEY_MANAGER->isStayKeyDown(VK_RIGHT)) // ���������� �����ؼ� �̵�
	{
		if (player.getDirection() == DIRECTION::LEFT)
		{
			player.setDirection(DIRECTION::RIGHT);
			player.reverseSlope();
		}
	}
	else if (KEY_MANAGER->isOnceKeyDown(VK_SPACE)) // ���� ��ư �Է�
	{
		return new jumpState;
	}
	else // �������� ����
	{
		player.updateSlope();
		return new idleState;
	}

	bool isNotFallen = player.move();
	if (!isNotFallen) // �����̴� �������� ���
	{
		return new fallenState;
	}
	_ani->frameUpdate(TIME_MANAGER->getElapsedTime()); // ���°� �ٲ��� ���� ��� ������ ������Ʈ
	return nullptr;
}

void moveState::render(worm & player)
{
	// ������
	RECT rc = player.getRect();
	float x = (rc.left + rc.right) / 2, y = (rc.top + rc.bottom) / 2;
	POINT pos = _ani->getFramePos();
	
	_img = IMAGE_MANAGER->findImage(getImageKey("MOVE", player.getSlope()));
	CAMERA_MANAGER->aniRender(getMemDC(), _img, x - 30, y - 30, _ani, ((player.getDirection() == DIRECTION::RIGHT) ? true : false));
	
	
}
