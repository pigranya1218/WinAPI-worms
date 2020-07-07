#pragma once
#include "weapon.h"

class bazukaWeapon : public weapon
{
private:
	image* _weaponImg;
	image* _aimImg;
	image* _blobImg;
	animation* _ani;

	float _angle = PI / 2; // ������ ���� 
	float _angleOffset = PI / 31; // ���� ���� ������
	float _frameAngleOffset = _angleOffset / 2;
	float _blobOffset = (float) 100 / 16; // ������ ������
	int _gage = 0; // ������ ������
	float _power = 50; // ������ �Ŀ�
	float _damage = 40; // ������ �����
	float _bombCircleRadius = 100; // ������ ���� �ݰ�
	float _aimRadius = 90;
	
	bool _isGaging = false;

	void shoot(worm& shooter);

public:
	bazukaWeapon() {}
	~bazukaWeapon() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual WEAPON_FINISH_TYPE update(worm& player); // ����Ǿ��°�

	virtual void render(worm& player);
};

