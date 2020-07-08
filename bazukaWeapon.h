#pragma once
#include "weapon.h"

class bazukaWeapon : public weapon
{
private:
	image* _weaponImg;
	image* _aimImg;
	image* _blobImg;
	animation* _ani;
	projectile* _projectile;

	float _angle = PI / 2; // ������ ���� 
	float _angleOffset = PI / 31; // ���� ���� ������
	float _frameAngleOffset = _angleOffset / 2;
	int _gage = 0; // ������ ������
	float _power = 12; // ������ �Ŀ�
	float _damage = 40; // ������ �����
	float _bombWidth = 130; // ������ ���� �ݰ�
	float _aimRadius = 90;
	
	void shoot(worm& shooter);

public:
	bazukaWeapon() {}
	~bazukaWeapon() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual WEAPON_FINISH_TYPE update(worm& player); // ����Ǿ��°�

	virtual void render(worm& player);
};

