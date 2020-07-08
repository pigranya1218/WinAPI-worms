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
	stageManager* getStageManager() { return _stageManager;}

	void addWorms(int index, float x, float y);
	COLORREF getPixel(int x, int y);
	int	getCurrentTurnIndex();
	bool checkCollisionPixel(RECT object);
};

