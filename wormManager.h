#pragma once
#include "gameNode.h"
#include "worm.h"

class stageManager;

class wormManager : public gameNode
{
private:
	vector<worm*> _worms;
	stageManager* _stageManager;

public:
	wormManager() {}
	~wormManager() {}

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setStageManager(stageManager* stageManager);
	void addWorms(float x, float y);
};

