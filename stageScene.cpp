#include "stdafx.h"
#include "stageScene.h"

HRESULT stageScene::init(int playerNum, int wormsPerPlayer, int turnTime)
{
	CAMERA_MANAGER->setConfig(0, 0, WINSIZEX, WINSIZEY, 0, 0, 2000 - WINSIZEX, 1125 - WINSIZEY);

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
}

void stageScene::render()
{
	_stageManager->render();
	_wormManager->render();
	_projectileManager->render();
	_itemManager->render();
}
