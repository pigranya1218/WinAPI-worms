#include "stdafx.h"
#include "allState.h"
#include "worm.h"

void jumpState::enter(worm & player)
{
	_img = IMAGE_MANAGER->findImage(getImageKey("JUMP", player.getSlope()));
	_ani = new animation;
	_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
	_ani->setDefPlayFrame(false, false);
	_ani->setFPS(20);
	_ani->start();

	_startX = player.getX();
	_startY = player.getY();

	_isGround = false;

	_ready = true;
}

void jumpState::exit(worm & player)
{
	SAFE_DELETE(_ani);
}

state * jumpState::update(worm & player)
{
	if (player.isTurn())
	{
		if (_ready) // ���� �غ�ܰ��� ��
		{
			if (KEY_MANAGER->isOnceKeyDown(VK_SPACE)) // �� ������ ��ȯ
			{
				return new longJumpState;
			}
		}
	}
	
	if (!_ani->isPlay() && _ready) // ���� ���·� ��ȯ
	{
		_img = IMAGE_MANAGER->findImage("FALLEN");
		_ready = false;

		_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
		_ani->setDefPlayFrame(false, true);
		_ani->setFPS(30);
		_ani->start();
	
		player.setAngle( ((player.getDirection() == DIRECTION::LEFT)? PI - _angle : _angle) ); // �÷��̾��� ���� ���� ����
		player.setGravity(0.0); // �÷��̾��� �߷� ����
		player.setPower(_power); // �÷��̾��� ���ư��� �Ŀ� ����, 1�ʿ� ������ �Ŀ��� �Ÿ���ŭ �̵�
	}

	if (!_ready && !_isGround) // ������ ������ ���
	{
		bool isLanded = player.gravityMove(_startX, _startY); // ���� ��� �̵�
		if(isLanded)
		{
			_isGround = true;
			if (player.getGravity() >= 6) // ���� ��� �ε��� ���
			{
				player.updateSlope(); // ���� ���� ����� ���� ���
				_img = IMAGE_MANAGER->findImage(getImageKey("FALLEN_TWANG", player.getSlope()));
				_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
				_ani->setDefPlayFrame(false, false);
				_ani->setFPS(20);
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

void jumpState::render(worm & player)
{
	RECT rc = player.getRect();

	float x = (rc.left + rc.right) / 2, y = (rc.top + rc.bottom) / 2;
	POINT pos = _ani->getFramePos();

	CAMERA_MANAGER->aniRender(getMemDC(), _img, x - 30, y - 30, _ani, ((player.getDirection() == DIRECTION::RIGHT) ? true : false));
}
