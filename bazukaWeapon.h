#pragma once
#include "weapon.h"

class bazukaWeapon : public weapon
{
private:
	image* _weaponImg;
	image* _aimImg;
	image* _blobImg;
	animation* _ani;

	float _angle = PI / 2; // 무기의 각도 
	float _angleOffset = PI / 31; // 무기 각도 오프셋
	float _frameAngleOffset = _angleOffset / 2;
	float _blobOffset = (float) 100 / 16; // 게이지 오프셋
	int _gage = 0; // 무기의 충전률
	float _power = 50; // 무기의 파워
	float _damage = 40; // 무기의 대미지
	float _bombCircleRadius = 100; // 무기의 폭파 반경
	float _aimRadius = 90;
	
	bool _isGaging = false;

	void shoot(worm& shooter);

public:
	bazukaWeapon() {}
	~bazukaWeapon() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual WEAPON_FINISH_TYPE update(worm& player); // 종료되었는가

	virtual void render(worm& player);
};

