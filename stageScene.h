#pragma once
#include "gameNode.h"
#include "stageManager.h"
#include "wormManager.h"
#include "projectileManager.h"
#include "objectManager.h"

class stageScene : public gameNode
{
private:
	stageManager* _stageManager; // stage, UI, ����, ī�޶� ����
	wormManager* _wormManager; // ���� ���� ���� (����, ���� UI)
	projectileManager* _projectileManager; // ����ü �� ���ڵ� ����
	objectManager* _objectManager; // ������, ���� �� �� ���� ������ ����
	float _zoom;

public:
	stageScene() {}
	~stageScene() {}

	virtual HRESULT init(int playerNum, int wormsPerPlayer, int turnTime);
	virtual void release();
	virtual void update();
	virtual void render();
};

