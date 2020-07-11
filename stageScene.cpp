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

	_objectManager = new objectManager;
	_objectManager->init();

	_projectileManager = new projectileManager;
	_projectileManager->init();

	_uiManager = new uiManager;
	_uiManager->init();

	_stageManager->setWormManager(_wormManager);
	_stageManager->setProjectileManager(_projectileManager);
	_stageManager->setObjectManager(_objectManager);
	_stageManager->setUIManager(_uiManager);
	_wormManager->setStageManager(_stageManager);
	_wormManager->setProjectileManager(_projectileManager);
	_wormManager->setObjectManager(_objectManager);
	_wormManager->setUIManager(_uiManager);
	_uiManager->setWormManager(_wormManager);

	_stageManager->makeStage();

	return S_OK;
}

void stageScene::release()
{
	_stageManager->release();
	_wormManager->release();
	_objectManager->release();
	_projectileManager->release();
	_uiManager->release();
}

void stageScene::update()
{
	_stageManager->update();
	_wormManager->update();
	_objectManager->update();
	_projectileManager->update();
	_uiManager->update();

	EFFECT_MANAGER->update();

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
	// ZOOM과 카메라가 필요한 렌더링
	_stageManager->render();
	_wormManager->render();
	_objectManager->render();
	_projectileManager->render();
	EFFECT_MANAGER->render();
	CAMERA_MANAGER->zoom(getMemDC(), _zoom);
	
	// UI 
	_uiManager->render();
}
