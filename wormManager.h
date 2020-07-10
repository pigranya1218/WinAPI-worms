#pragma once
#include "gameNode.h"
#include "worm.h"
#include "projectile.h"

class stageManager;
class uiManager;
class projectileManager;

class wormManager : public gameNode
{
private:
	vector<worm*> _worms;
	stageManager* _stageManager;
	projectileManager* _projectileManager;
	uiManager* _uiManager;

public:
	wormManager() {}
	~wormManager() {}

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setStageManager(stageManager* stageManager);
	void setProjectileManager(projectileManager* projectileManager);
	void setUIManager(uiManager* uiManager);

	stageManager* getStageManager() { return _stageManager;}

	void addWorms(int index, string name, float x, float y);
	COLORREF getPixel(int x, int y);
	int	getCurrentTurnIndex();
	bool checkCollisionPixel(RECT object);
	bool checkFreshDead(int index);
	bool checkDead(int index);
	void setDead(int index);
	void wormBomb(float x, float y, float damage, float width);
	void wormDamage(int index, int damage);
	void wormHpDiscount(int index, int discount);
	bool wormHpZero(int index);
	void renderWormUI(float x, float y, int hp, int index, string name);
	vector<int> getTeamHp();
	int getTeamMaxHp();

	void shoot(projectile* projectile);

	bool checkAllStop();
};

