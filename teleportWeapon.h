#pragma once
#include "weapon.h"

class teleportWeapon : public weapon
{
private:
	image* _wormImg;
	animation* _wormAni;

	image* _markerImg;
	animation* _markerAni;
	
	WEAPON_STATE _state;

	float _x, _y; // 포인트 좌표
	bool _isAvail;

	void setWaiting(worm& player);
	bool checkAvail(worm& player, float newX, float newY);

public:
	teleportWeapon() {}
	~teleportWeapon() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual WEAPON_FINISH_TYPE update(worm& player); // 종료되었는가

	virtual void render(worm& player);
};

