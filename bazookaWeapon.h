#pragma once
#include "weapon.h"

class bazookaWeapon : public weapon
{
protected:
	image* _weaponImg;
	image* _aimImg;
	image* _blobImg;
	animation* _ani;

	string _weaponName = "BAZOOKA";
	float _angle; // 무기의 각도 
	float _angleOffset; // 무기 각도 오프셋
	float _frameAngleOffset;
	int _gauge; // 무기의 충전률
	float _power; // 무기의 파워
	float _damage; // 무기의 대미지
	float _bombWidth; // 무기의 폭파 반경
	float _aimRadius;
	
	virtual void shot(worm& shooter);
	virtual void setWaiting(worm& player);

public:
	bazookaWeapon() {}
	~bazookaWeapon() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual WEAPON_FINISH_TYPE update(worm& player); // 종료되었는가

	virtual void render(worm& player);
};

