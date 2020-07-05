#include "stdafx.h"
#include "allState.h"
#include "worm.h"

void fallenState::enter(worm& player)
{
	_img = IMAGE_MANAGER->findImage("FALLEN");
	_ani = new animation;
	_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
	_ani->setDefPlayFrame(true, true);
	_ani->setFPS(20);
	_ani->start();
	_isGround = false;
}

void fallenState::exit(worm& player)
{
	player.setGravity(0); // gravity 초기화
	SAFE_DELETE(_ani);
}

state* fallenState::update(worm& player)
{
	// 땅에 닿을 때까지 떨어지기
	if (!_isGround)
	{
		_isGround = player.gravityMove();
		if (_isGround)
		{
			_gravity = player.getGravity(); // 착지할 당시의 중력값 저장
			if (_gravity >= 6) // 20 이상의 중력값에선 땅에 박혀야 함
			{
				player.updateSlope(); // 땅에 박힐 당시의 기울기 계산
				_img = IMAGE_MANAGER->findImage(getImageKey("FALLEN_TWANG", player.getSlope()));
				_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
				_ani->setDefPlayFrame(false, false);
				_ani->setFPS(20);
				_ani->start();
			}
			else // 딱히 위협적이지 않은 중력값에선 그냥 moveState로 바로 이동
			{
				return new moveState; 
			}
		}
	}
	else
	{
		if (!_ani->isPlay()) // 땅에 박힌 애니메이션 끝나면 moveState 이동
		{
			return new moveState;
		}
	}
	
	_ani->frameUpdate(TIME_MANAGER->getElapsedTime()); // 상태가 바뀌지 않은 경우 프레임 업데이트
	return nullptr;
}

void fallenState::render(worm& player)
{
	RECT rc = player.getRect();

	float x = (rc.left + rc.right) / 2, y = (rc.top + rc.bottom) / 2;
	POINT pos = _ani->getFramePos();

	CAMERA_MANAGER->aniRender(getMemDC(), _img, x - 30, y - 30, _ani, ((player.getDirection() == DIRECTION::RIGHT) ? true : false));
}
