#include "stdafx.h"
#include "wormManager.h"
#include "stageManager.h"
#include "uiManager.h"

HRESULT wormManager::init()
{
	return S_OK;
}

void wormManager::release()
{
	for (int i = 0; i < _worms.size(); i++)
	{
		_worms[i]->release();
	}
}

void wormManager::update()
{
	for (int i = 0; i < _worms.size(); i++)
	{
		_worms[i]->update();
	}
}

void wormManager::render()
{
	for (int i = 0; i < _worms.size(); i++)
	{
		_worms[i]->render();
	}
}

void wormManager::setStageManager(stageManager * stageManager)
{
	_stageManager = stageManager;
}

void wormManager::setUIManager(uiManager * uiManager)
{
}

void wormManager::addWorms(int index, float x, float y)
{
	worm* newWorm = new worm;
	newWorm->init(this, index, x, y);
	_worms.push_back(newWorm);
}

COLORREF wormManager::getPixel(int x, int y)
{
	return _stageManager->getPixel(x, y);
}

int wormManager::getCurrentTurnIndex()
{
	return _stageManager->getCurrentTurnIndex();
}

bool wormManager::checkCollisionPixel(RECT object)
{
	RECT temp;
	for (int i = 0; i < _worms.size(); i++)
	{
		worm* currWorm = _worms[i];
		
		if (IntersectRect(&temp, &currWorm->getRect(), &object))
		{
			return true;
		}
	}

	return false;
}

void wormManager::wormBomb(float x, float y, float power, float width)
{
	MYCIRCLE bombCircle = {x, y, width / 2};

	for (int i = 0; i < _worms.size(); i++)
	{
		RECT wormRC = _worms[i]->getRect();

		if (TTYONE_UTIL::isCollision(bombCircle, wormRC)) // 폭파반경과 웜즈의 충돌판정 검사
		{
			float distance = TTYONE_UTIL::getDistance(x, y, _worms[i]->getX(), _worms[i]->getY());
			float ratio = 1.4 - (distance / (width * 0.5)); // 거리비율
			ratio = min(1, ratio); // 최소 비율 조정

			float angle = TTYONE_UTIL::getAngle(x, y, _worms[i]->getX(), _worms[i]->getY());
			float degree = angle * (180 / PI);
 			_worms[i]->hit(angle, power * 0.2 * ratio);
		}
	}
}

void wormManager::wormDamage(int index, int damage)
{

}