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
	_uiManager = uiManager;
}

void wormManager::addWorms(int index, string name, float x, float y)
{
	worm* newWorm = new worm;
	newWorm->init(this, index, name, x, y);
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

bool wormManager::checkFreshDead(int index)
{
	if (_worms[index]->isFreshDead())
	{
		return true;
	}
	return false;
}

bool wormManager::checkDead(int index)
{
	return _worms[index]->isDead();
}

void wormManager::setDead(int index)
{
	_worms[index]->setDead();
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
			wormDamage(i, floor(power * ratio));
		}
	}
}

void wormManager::wormDamage(int index, int damage)
{
	_stageManager->setWormDamage(index, damage);
}

void wormManager::wormHpDiscount(int index, int discount)
{
	_worms[index]->discountHp(discount);
}

bool wormManager::wormHpZero(int index)
{
	return _worms[index]->checkHpZero();
}

void wormManager::renderWormUI(float x, float y, int hp, int index, string name)
{
	int playerIndex = index % _stageManager->getPlayerNum();
	_uiManager->drawWormUI(x, y, hp, playerIndex, name);
}

vector<int> wormManager::getTeamHp()
{
	vector<int> result(_stageManager->getPlayerNum(), 0);

	for (int i = 0; i < _worms.size(); i++)
	{
		result[i % _stageManager->getPlayerNum()] += _worms[i]->getHp();
	}

	return result;
}

int wormManager::getTeamMaxHp()
{
	return _worms[0]->getMaxHp() * _stageManager->getWormsPerPlayer();
}



bool wormManager::checkAllStop()
{
	for (int i = 0; i < _worms.size(); i++)
	{
		if (!_worms[i]->isStop())
		{
			return false;
		}
	}
	return true;
}
