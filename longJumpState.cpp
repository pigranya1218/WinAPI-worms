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

	if (!_ani->isPlay() && _ready) // ���� ���·� ��ȯ
	{
		SOUND_MANAGER->stop("WORM_LONGJUMP");
		SOUND_MANAGER->play("WORM_LONGJUMP", 1);

		_img = IMAGE_MANAGER->findImage("LONGJUMP");
		_ready = false;

		_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
		_ani->setDefPlayFrame(false, false);
		_ani->setFPS(20);
		_ani->start();

		player.setAngle(_angle); // �÷��̾��� ���� ���� ����
		player.setGravity(0.0); // �÷��̾��� �߷� ����
		player.setPower(_power); // �÷��̾��� ���ư��� �Ŀ� ����, 1�ʿ� ������ �Ŀ��� �Ÿ���ŭ �̵�
	}

	if (!_ready && !_isGround) // ������ ������ ���
	{
		bool isLanded = player.gravityMove(xMove, 0.6); // ���� ��� �̵�
		if (isLanded) // ������ ���
		{
			_isGround = true;
			if (player.getGravity() >= 8) // ���� ��� �ε��� ���
			{
				// ����� �� �� ����
				player.setDamage(player.getGravity());
				player.setWaiting();

				player.updateSlope(); // ���� ���� ����� ���� ���
				_img = IMAGE_MANAGER->findImage(getImageKey("FALLEN_TWANG", player.getSlope()));
				_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
				_ani->setDefPlayFrame(false, false);
				_ani->setFPS(30);
				_ani->start();

				SOUND_MANAGER->stop("WORM_DIGDA");
				SOUND_MANAGER->play("WORM_DIGDA", 1);
			}
			else
			{
				return new moveState;
			}
		}
	}

	if (_isGround && !_ani->isPlay()) // ���� ������ �ִϸ��̼� ���� ���
	{
		return new moveState;
	}

	_ani->frameUpdate(TIME_MANAGER->getElapsedTime()); // ���°� �ٲ��� ���� ��� ������ ������Ʈ

	return nullptr;
}

void longJumpState::render(worm& player)
{
	RECT rc = player.getRect();

	float x = (rc.left + rc.right) / 2, y = (rc.top + rc.bottom) / 2;
	POINT pos = _ani->getFramePos();

	CAMERA_MANAGER->aniRender(getMemDC(), _img, x - 30, y - 30, _ani, ((player.getDirection() == DIRECTION::RIGHT) ? true : false));
}
