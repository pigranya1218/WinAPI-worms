#pragma once
#include "gameNode.h"
#include "stageManager.h"
#include "wormManager.h"
#include "projectileManager.h"
#include "itemManager.h"

class stageScene : public gameNode
{
private:
	stageManager* _stageManager; // stage, UI, 지형, 카메라 관리
	wormManager* _wormManager; // 웜즈 관련 관리 (웜즈, 무기 UI)
	projectileManager* _projectileManager; // 투사체 및 지뢰들 관리
	itemManager* _itemManager; // 아이템 관리
	float _zoom;

public:
	stageScene() {}
	~stageScene() {}

	virtual HRESULT init(int playerNum, int wormsPerPlayer, int turnTime);
	virtual void release();
	virtual void update();
	virtual void render();
};

