#pragma once
#include "weapon.h"
#include "mineObject.h"

class mineWeapon : public weapon
{
private:
	image* _wormImg;
	animation* _wormAni;

	WEAPON_STATE _state;

	void shot(worm& shooter);
	void setWaiting(worm& player);

public:
	mineWeapon() {}
	~mineWeapon() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual WEAPON_FINISH_TYPE update(worm& player);

	virtual void render(worm& player);

};

