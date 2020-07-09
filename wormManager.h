#pragma once
#include "gameNode.h"
#include "worm.h"

class stageManager;
class uiManager;

class wormManager : public gameNode
{
private:
	vector<worm*> _worms;
	stageManager* _stageManager;
	uiManager* _uiManager;

public:
	wormManager() {}
	~wormManager() {}

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setStageManager(stageManager* stageManager);
	void setUIManager(uiManager* uiManager);

	stageManager* getStageManager() { return _stageManager;}

	void addWorms(int index, float x, float y);
	COLORREF getPixel(int x, int y);
	int	getCurrentTurnIndex();
	bool checkCollisionPixel(RECT object);
	void wormBomb(float x, float y, float damage, float width);
	void wormDamage(int index, int damage);
};

