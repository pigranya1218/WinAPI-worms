#include "stdafx.h"
#include "donkey.h"
#include "stageManager.h"
#include "wormManager.h"

void donkey::update()
{
	if (!_isFinish)
	{
		float wind = _stageManager->getWind() * 0.2;
		bool isBomb = gravityMove(wind);

		if (isBomb) // ���Ľ�Ű��
		{
			EFFECT_MANAGER->play("EFFECT_CIRCLE", _x, _y, _bombWidth, _bombWidth);
			EFFECT_MANAGER->play("EFFECT_ELIPSE", _x, _y, _bombWidth + 30, _bombWidth + 30);
			EFFECT_MANAGER->play("EFFECT_EX_POW", _x, _y - 50, 50, 50);

			_stageManager->pixelBomb(_x, _y, _damage, _bombWidth); // �ȼ� ���Ľ�Ű��
			_wormManager->wormBomb(_x, _y, _damage, _bombWidth); // ���� ���� ���󰡰� �ϱ�
			
			_gravity = -3;
		}

		if (_y >= 1000)
		{
			_isFinish = true;
		}
	}
}
