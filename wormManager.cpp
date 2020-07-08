#include "stdafx.h"
#include "wormManager.h"
#include "stageManager.h"

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
