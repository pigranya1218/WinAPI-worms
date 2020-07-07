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
	SAFE_DELETE(_ani);
}

state* longJumpState::update(worm& player)
{
	// DEBUG
	if (KEY_MANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		player.setPlayerPos(CAMERA_MANAGER->getAbsoluteL(_ptMouse.x), CAMERA_MANAGER->getAbsoluteT(_ptMouse.y));
		return new idleState;
	}

	float xMove = (player.getDirection() == DIRECTION::LEFT)? 0.2 : -0.2;

	if (player.isTurn())
	{
	}

	if (!_ani->isPlay() && _ready) // ���� ���·� ��ȯ
	{
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
		bool isLanded = player.gravityMove(xMove); // ���� ��� �̵�
		if (isLanded) // ������ ���
		{
			_isGround = true;
			if (player.getGravity() >= 8) // ���� ��� �ε��� ���
			{
				player.updateSlope(); // ���� ���� ����� ���� ���
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
