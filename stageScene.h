#pragma once
#include "gameNode.h"
#include "stageManager.h"
#include "wormManager.h"
#include "projectileManager.h"
#include "itemManager.h"

class stageScene : public gameNode
{
private:
	stageManager* _stageManager; // stage, UI, ����, ī�޶� ����
	wormManager* _wormManager; // ���� ���� ���� (����, ���� UI)
	projectileManager* _projectileManager; // ����ü �� ���ڵ� ����
	itemManager* _itemManager; // ������ ����

public:
	stageScene() {}
	~stageScene() {}

	virtual HRESULT init(int playerNum, int wormsPerPlayer, int turnTime);
	virtual void release();
	virtual void update();
	virtual void render();
};

