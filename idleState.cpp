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
		if (KEY_MANAGER->isOnceKeyDown(VK_LEFT)) // 왼쪽으로 움직이기
		{
			player.setDirection(DIRECTION::LEFT);
			return new moveState;
		}
		else if (KEY_MANAGER->isOnceKeyDown(VK_RIGHT)) // 오른쪽으로 움직이기
		{
			player.setDirection(DIRECTION::RIGHT);
			return new moveState;
		}
		else if (KEY_MANAGER->isOnceKeyDown(VK_SPACE)) // 점프하기
		{
			return new jumpState;
		}
	}
	else // 랜덤으로 IDLE 애니메이션 재생
	{

	}

	_ani->frameUpdate(TIME_MANAGER->getElapsedTime()); // 상태가 바뀌지 않은 경우 프레임 업데이트
	return nullptr;
}

void idleState::render(worm & player)
{
	RECT rc = player.getRect();

	float x = (rc.left + rc.right) / 2, y = (rc.top + rc.bottom) / 2;
	POINT pos = _ani->getFramePos();
	
	// 디버깅용
	// CAMERA_MANAGER->rectangle(getMemDC(), rc);

	CAMERA_MANAGER->aniRender(getMemDC(), _img, x - 30, y - 28, _ani, ( (player.getDirection() == DIRECTION::RIGHT)? true : false) );
}
