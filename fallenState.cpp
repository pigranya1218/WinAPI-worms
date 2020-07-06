#include "stdafx.h"
#include "allState.h"
#include "worm.h"

void fallenState::enter(worm& player)
{
	_img = IMAGE_MANAGER->findImage("FALLEN");
	_ani = new animation;
	_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
	_ani->setDefPlayFrame(true, true);
	_ani->setFPS(30);
	_ani->start();
	_isGround = false;

}

void fallenState::exit(worm& player)
{
	SAFE_DELETE(_ani);
}

state* fallenState::update(worm& player)
{
	// ���� ���� ������ ��������
	if (!_isGround)
	{
		_isGround = player.gravityMove(0);
		if (_isGround)
		{
			_gravity = player.getGravity(); // ������ ����� �߷°� ����
			if (_gravity >= 8) // 20 �̻��� �߷°����� ���� ������ ��
			{
				player.updateSlope(); // ���� ���� ����� ���� ���
				_img = IMAGE_MANAGER->findImage(getImageKey("FALLEN_TWANG", player.getSlope()));
				_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
				_ani->setDefPlayFrame(false, false);
				_ani->setFPS(20);
				_ani->start();
			}
			else // ���� ���������� ���� �߷°����� �׳� moveState�� �ٷ� �̵�
			{
				return new moveState; 
			}
		}
	}
	else
	{
		if (!_ani->isPlay()) // ���� ���� �ִϸ��̼� ������ moveState �̵�
		{
			return new moveState;
		}
	}
	
	_ani->frameUpdate(TIME_MANAGER->getElapsedTime()); // ���°� �ٲ��� ���� ��� ������ ������Ʈ
	return nullptr;
}

void fallenState::render(worm& player)
{
	RECT rc = player.getRect();

	float x = (rc.left + rc.right) / 2, y = (rc.top + rc.bottom) / 2;
	POINT pos = _ani->getFramePos();

	CAMERA_MANAGER->aniRender(getMemDC(), _img, x - 30, y - 30, _ani, ((player.getDirection() == DIRECTION::RIGHT) ? true : false));
}
