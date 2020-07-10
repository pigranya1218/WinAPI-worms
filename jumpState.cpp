#include "stdafx.h"
#include "allState.h"
#include "worm.h"
#include "wormManager.h"

void jumpState::enter(worm & player)
{
	_img = IMAGE_MANAGER->findImage(getImageKey("JUMP", player.getSlope()));
	_ani = new animation;
	_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
	_ani->setDefPlayFrame(false, false);
	_ani->setFPS(20);
	_ani->start();

	_isGround = false;
	_ready = true;
	_transToLong = false;
}

void jumpState::exit(worm & player)
{
	if (!_transToLong)
	{
		_ani->release();
		SAFE_DELETE(_ani);
	}
}

state * jumpState::update(worm & player)
{
	// DEBUG
	if (KEY_MANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		player.setPlayerPos(CAMERA_MANAGER->getAbsoluteL(_ptMouse.x), CAMERA_MANAGER->getAbsoluteT(_ptMouse.y));
		return new idleState;
	}

	if (player.isTurn())
	{
		if (_ready) // ���� �غ�ܰ��� ��
		{
			if (KEY_MANAGER->isOnceKeyDown(VK_LCONTROL)) // �� ������ ��ȯ
			{
				_transToLong = true;
				longJumpState* newState = new longJumpState;
				newState->setCurrentAni(_ani);
				return newState;
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
		bool isLanded = player.gravityMove(0, 0.6); // ���� ��� �̵�
		if(isLanded)
		{
			_isGround = true;
			if (player.getGravity() >= 5) // ���� ��� �ε��� ���
			{
				// ����� �� �� ����
				player.setDamage(player.getGravity());
				player.setWaiting();

				// ���� ������ �ִϸ��̼� ���
				player.updateSlope();
				player.setGravity(0);
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
