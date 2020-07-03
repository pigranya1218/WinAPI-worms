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

	if (KEY_MANAGER->isStayKeyDown(VK_LEFT)) // 왼쪽으로 지속해서 이동
	{
		player.setDirection(DIRECTION::LEFT);
	}
	else if (KEY_MANAGER->isStayKeyDown(VK_RIGHT)) // 오른쪽으로 지속해서 이동
	{
		player.setDirection(DIRECTION::RIGHT);
	}
	else if (KEY_MANAGER->isOnceKeyDown(VK_SPACE)) // 점프 버튼 입력
	{
		return new jumpState;
	}
	else // 움직임을 멈춤
	{
		return new idleState;
	}

	player.move();
	_ani->frameUpdate(TIME_MANAGER->getElapsedTime()); // 상태가 바뀌지 않은 경우 프레임 업데이트
	return nullptr;
}

void moveState::render(worm & player)
{
	// 디버깅용
	RECT rc = player.getRect();
	float x = (rc.left + rc.right) / 2, y = (rc.top + rc.bottom) / 2;
	POINT pos = _ani->getFramePos();
	// CAMERA_MANAGER->rectangle(getMemDC(), rc);
	CAMERA_MANAGER->aniRender(getMemDC(), _img, x - 30, y - 28, _ani, ((player.getDirection() == DIRECTION::RIGHT) ? true : false));
}
