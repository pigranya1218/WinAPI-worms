#include "stdafx.h"
#include "homing.h"
#include "stageManager.h"
#include "wormManager.h"
#include "objectManager.h"

void homing::update()
{
	if (!_isFinish)
	{
		if (_homingCount++ >= 50) // 바추카와 같다
		{
			if (!_isHoming)
			{
				_angle = TTYONE_UTIL::getAngle(_x, _y, _pointX, _pointY);
				float degree = _angle * (180 / PI);
				_isHoming = true;
				_power = 20;

				SOUND_MANAGER->stop("WEAPON_HOMING_ON");
				SOUND_MANAGER->play("WEAPON_HOMING_ON", 1);
			}
		}
		else
		{
			_ani->frameUpdate(TIME_MANAGER->getElapsedTime());
		}

		EFFECT_MANAGER->play("EFFECT_HEXHAUST", _x, _y, 40, 40);
		bool isBomb = gravityMove(0);

		if (isBomb) // 폭파시키기
		{
			SOUND_MANAGER->stop("WEAPON_HOMING_ON");
			EFFECT_MANAGER->play("EFFECT_CIRCLE", _x, _y, _bombWidth, _bombWidth);
			EFFECT_MANAGER->play("EFFECT_ELIPSE", _x, _y, _bombWidth + 30, _bombWidth + 30);
			EFFECT_MANAGER->play("EFFECT_EX_POW", _x, _y - 50, 50, 50);

			_stageManager->pixelBomb(_x, _y, _damage, _bombWidth); // 픽셀 폭파시키기
			_wormManager->wormBomb(_x, _y, _damage, _bombWidth); // 맞은 웜즈 날라가게 하기
			_objectManager->objectBomb(_x, _y, _damage, _bombWidth);
			_objectManager->makeBombEffect(_x, _y, _bombWidth);

			_isFinish = true;
		}
	}
}

void homing::render()
{
	if (_isHoming)
	{
		_img = IMAGE_MANAGER->findImage("PROJECTILE_HOMING_RED");
		projectile::render(); // 빨간 유도 미사일 뿅뿅 날라간다
	}
	else 
	{
		// 파란 유도미사일 빙글빙글 날아간다
		CAMERA_MANAGER->aniRender(getMemDC(), _img, _x - (_img->getFrameWidth() / 2), _y - (_img->getFrameHeight() / 2), _ani, false);
	}
}

