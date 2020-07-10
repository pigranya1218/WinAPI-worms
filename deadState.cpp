#include "stdafx.h"
#include "deadState.h"
#include "worm.h"
#include "stageManager.h"
#include "wormManager.h"

void deadState::enter(worm & player)
{
	_img = IMAGE_MANAGER->findImage("DEAD");
	_ani = new animation;
	_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
	_ani->setDefPlayFrame(false, false);
	_ani->setFPS(20);
	_ani->start();

	_isBomb = false;
	_damage = 30;
	_bombWidth = 80;
}

void deadState::exit(worm & player)
{
	SAFE_DELETE(_ani);
}

state * deadState::update(worm & player)
{

	if (!_isBomb) // ���� �����ϱ� ��
	{

		if (!_ani->isPlay()) // �����Ѵ�!!!!
		{
			_isBomb = true;

			float x = player.getX(), y = player.getY();
			EFFECT_MANAGER->play("EFFECT_CIRCLE", x, y, _bombWidth, _bombWidth);
			EFFECT_MANAGER->play("EFFECT_ELIPSE", x, y, _bombWidth + 25, _bombWidth + 25);
			EFFECT_MANAGER->play("EFFECT_EX_POW", x, y - 35, 36, 36);

			player.getStageManager()->pixelBomb(x, y, _damage, _bombWidth); // �ȼ� ���Ľ�Ű��
			player.getWormManager()->wormBomb(x, y, _damage, _bombWidth); // ���� ���� ���󰡰� �ϱ�
			player.setGravity(3.4); 
			player.setAngle(PI / 2); 

			char buffer[50];
			sprintf_s(buffer, "GRAVE%d", (RND->getInt(5) + 1));
			_img = IMAGE_MANAGER->findImage(buffer);
			_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
			_ani->setDefPlayFrame(true, true);
			_ani->setFPS(15);
			_ani->start();
		}
	}
	else // �����Ͽ���
	{
		_isGround = player.gravityMove(0, 0.2);
		if (_isGround) // Ȥ�ö� ��ݹ����� �����̰�!
		{
			player.setGravity(0);
		}
	}

	_ani->frameUpdate(TIME_MANAGER->getElapsedTime());
	return nullptr;
}

void deadState::render(worm & player)
{
	float x = player.getX(), y = player.getY();
	
	if (_isBomb)
	{
		CAMERA_MANAGER->aniRender(getMemDC(), _img, x - 30, y - 22, _ani, ((player.getDirection() == DIRECTION::RIGHT) ? true : false));
	}
	else
	{
		CAMERA_MANAGER->aniRender(getMemDC(), _img, x - 30, y - 30, _ani, ((player.getDirection() == DIRECTION::RIGHT) ? true : false));
	}
}

bool deadState::isStop()
{
	if (_isBomb)
	{
		if (_isGround)
		{
			return true;
		}
	}
	return false;
}
