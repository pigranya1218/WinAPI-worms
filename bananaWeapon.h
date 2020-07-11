#pragma once
#include "bazookaWeapon.h"
#include "banana.h"

class bananaWeapon : public bazookaWeapon
{
private:
	virtual void shot(worm& shooter);

public:
	bananaWeapon() { _weaponName = "BANANA"; }
	~bananaWeapon() {}

	virtual void enter(worm& player);
	virtual WEAPON_FINISH_TYPE update(worm& player);
};

