#include "stdafx.h"
#include "allState.h"
#include "worm.h"

void longJumpState::enter(worm& player)
{
	_img = IMAGE_MANAGER->findImage(getImageKey("JUMP", player.getSlope()));

	_isGround = false;
	_ready = true;
}

void longJumpState::exit(worm& player)
{
	_ani->release();
	SAFE_DELETE(_ani);
}

state* longJumpState::update(worm& player)
{
	float xMove = (player.getDirection() == DIRECTION::LEFT)? _xMove : -_xMove;

	if (player.isTurn())
	{
	}

	if (!_ani->isPlay() && _ready) // 점프 상태로 전환
	{
		_img = IMAGE_MANAGER->findImage("LONGJUMP");
		_ready = false;

		_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
		_ani->setDefPlayFrame(false, false);
		_ani->setFPS(20);
		_ani->start();

		player.setAngle(_angle); // 플레이어의 점프 각도 설정
		player.setGravity(0.0); // 플레이어의 중력 설정
		player.setPower(_power); // 플레이어의 날아가는 파워 설정, 1초에 설정된 파워의 거리만큼 이동
	}

	if (!_ready && !_isGround) // 점프한 상태인 경우
	{
		bool isLanded = player.gravityMove(xMove, 0.6); // 점프 계산 이동
		if (isLanded) // 착륙한 경우
		{
			_isGround = true;
			if (player.getGravity() >= 8) // 땅에 쎄게 부딪힌 경우
			{
				// 대미지 및 턴 종료
				player.setDamage(player.getGravity());
				player.setWaiting();

				player.updateSlope(); // 땅에 박힐 당시의 기울기 계산
				_img = IMAGE_MANAGER->findImage(getImageKey("FALLEN_TWANG", player.getSlope()));
				_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
				_ani->setDefPlayFrame(false, false);
				_ani->setFPS(30);
				_ani->start();
			}
			else
			{
				return new moveState;
			}
		}
	}

	if (_isGround && !_ani->isPlay()) // 땅에 박히는 애니메이션 끝난 경우
	{
		return new moveState;
	}

	_ani->frameUpdate(TIME_MANAGER->getElapsedTime()); // 상태가 바뀌지 않은 경우 프레임 업데이트

	return nullptr;
}

void longJumpState::render(worm& player)
{
	RECT rc = player.getRect();

	float x = (rc.left + rc.right) / 2, y = (rc.top + rc.bottom) / 2;
	POINT pos = _ani->getFramePos();

	CAMERA_MANAGER->aniRender(getMemDC(), _img, x - 30, y - 30, _ani, ((player.getDirection() == DIRECTION::RIGHT) ? true : false));
}
