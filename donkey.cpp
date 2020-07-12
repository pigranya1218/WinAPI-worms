#include "stdafx.h"
#include "donkey.h"
#include "stageManager.h"
#include "wormManager.h"
#include "objectManager.h"

void donkey::update()
{
	if (!_isFinish)
	{
		float wind = _stageManager->getWind() * 0.2;
		bool isBomb = gravityMove(wind);

		if (isBomb) // 폭파시키기
		{
			SOUND_MANAGER->stop("WEAPON_DONKEY_BOMB");
			SOUND_MANAGER->play("WEAPON_DONKEY_BOMB", 1);
			
			EFFECT_MANAGER->play("EFFECT_CIRCLE", _x, _y, _bombWidth, _bombWidth);
			EFFECT_MANAGER->play("EFFECT_ELIPSE", _x, _y, _bombWidth + 30, _bombWidth + 30);
			EFFECT_MANAGER->play("EFFECT_EX_POW", _x, _y - 50, 50, 50);

			_stageManager->pixelBomb(_x, _y, _damage, _bombWidth); // 픽셀 폭파시키기
			_wormManager->wormBomb(_x, _y, _damage, _bombWidth); // 맞은 웜즈 날라가게 하기
			_objectManager->objectBomb(_x, _y, _damage, _bombWidth);
			_objectManager->makeBombEffect(_x, _y, _bombWidth);

			_gravity = -3;
		}

		if (_y >= 1100)
		{
			_isFinish = true;
		}
	}
}
