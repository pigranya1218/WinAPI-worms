#pragma once
#include "gameNode.h"
#include "projectile.h"

class projectileManager : public gameNode
{
private:
	vector<projectile*> _projectiles;

public:
	projectileManager() {}
	~projectileManager() {}

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void addProjectile(projectile* projectile);
	bool checkZero() { return _projectiles.size() == 0; }
};
