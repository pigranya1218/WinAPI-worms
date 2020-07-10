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
	stageManager* _stageManager; // stage, ����, ī�޶� ����
	wormManager* _wormManager; // ���� ���� ���� (����, ���� UI)
	objectManager* _objectManager; // ������, ���� �� �� ���� ������ ����
	projectileManager* _projectileManager; // ����ī �� ���� ����ü ���� ����
	uiManager* _uiManager; // UI ����
	float _zoom;

public:
	stageScene() {}
	~stageScene() {}

	virtual HRESULT init(int playerNum, int wormsPerPlayer, int turnTime);
	virtual void release();
	virtual void update();
	virtual void render();
};

