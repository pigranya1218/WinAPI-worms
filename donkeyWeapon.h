#pragma once
#include "weapon.h"
#include "donkey.h"

class donkeyWeapon : public weapon
{

private:
	image* _wormImg;
	animation* _wormAni;

	image* _markerImg;
	animation* _markerAni;

	WEAPON_STATE _state;
	float _power;
	float _damage; // 무기의 대미지
	float _bombWidth; // 무기의 폭파 반경

	float _x, _y; // 포인트 좌표

	void shoot(worm& shooter);

public:
	donkeyWeapon() {}
	~donkeyWeapon() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual WEAPON_FINISH_TYPE update(worm& player); // 종료되었는가

	virtual void render(worm& player);
};

