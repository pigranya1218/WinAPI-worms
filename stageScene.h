#pragma once
#include "gameNode.h"
#include "stageManager.h"
#include "wormManager.h"
#include "projectileManager.h"
#include "objectManager.h"
#include "uiManager.h"

class stageScene : public gameNode
{
private:
	stageManager* _stageManager; // stage, 지형, 카메라 관리
	wormManager* _wormManager; // 웜즈 관련 관리 (웜즈, 무기 UI)
	objectManager* _objectManager; // 아이템, 지뢰 등 맵 상의 아이템 관리
	projectileManager* _projectileManager; // 바주카 등 무기 투사체 관련 관리
	uiManager* _uiManager; // UI 관리
	float _zoom;

public:
	stageScene() {}
	~stageScene() {}

	virtual HRESULT init(int playerNum, int wormsPerPlayer, int turnTime);
	virtual void release();
	virtual void update();
	virtual void render();
};

