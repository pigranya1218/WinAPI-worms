#pragma once
#include "bazookaWeapon.h"
#include "homing.h"

class homingWeapon : public bazookaWeapon
{
private:
	virtual void shot(worm& shooter);
	float _pointX;
	float _pointY;

	image* _markerImg;
	animation* _markerAni;

public:
	homingWeapon() { _weaponName = "HOMING"; }
	~homingWeapon() {}

	virtual void enter(worm& player);
	virtual void exit(worm& player);
	virtual WEAPON_FINISH_TYPE update(worm& player);
	virtual void render(worm& player);
};

