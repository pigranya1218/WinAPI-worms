#include "stdafx.h"
#include "stageScene.h"

HRESULT stageScene::init(int playerNum, int wormsPerPlayer, int turnTime)
{
	CAMERA_MANAGER->setConfig(20, 20, WINSIZEX, WINSIZEY, 20, 20, (2000 - 20) - WINSIZEX, (1125 - 20) - WINSIZEY);

	_zoom = 1;

	_stageManager = new stageManager;
	_stageManager->init(playerNum, wormsPerPlayer, turnTime);

	_wormManager = new wormManager;
	_wormManager->init();

	_projectileManager = new projectileManager;
	_projectileManager->init();

	_itemManager = new itemManager;
	_itemManager->init();

	_stageManager->setWormManager(_wormManager);
	_wormManager->setStageManager(_stageManager);

	_stageManager->makeStage();

	return S_OK;
}

void stageScene::release()
{
	_stageManager->release();
	_wormManager->release();
	_projectileManager->release();
	_itemManager->release();
}

void stageScene::update()
{
	_stageManager->update();
	_wormManager->update();
	_projectileManager->update();
	_itemManager->update();

	if (KEY_MANAGER->isStayKeyDown('Z'))
	{
		_zoom = min(2, _zoom + 0.01);
	}
	if (KEY_MANAGER->isStayKeyDown('X'))
	{
		_zoom = max(1, _zoom - 0.01);
	}

	CAMERA_MANAGER->setX(CAMERA_MANAGER->getX());
	CAMERA_MANAGER->setY(CAMERA_MANAGER->getY());
}

void stageScene::render()
{
	_stageManager->render();
	_wormManager->render();
	_projectileManager->render();
	_itemManager->render();
	CAMERA_MANAGER->zoom(getMemDC(), _zoom);
}
