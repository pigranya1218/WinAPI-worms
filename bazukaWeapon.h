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

	float _angle; // ������ ���� 
	float _angleOffset; // ���� ���� ������
	float _frameAngleOffset;
	int _gage; // ������ ������
	float _power; // ������ �Ŀ�
	float _damage; // ������ �����
	float _bombWidth; // ������ ���� �ݰ�
	float _aimRadius;
	
	void shoot(worm& shooter);

public:
	bazukaWeapon() {}
	~bazukaWeapon() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual WEAPON_FINISH_TYPE update(worm& player); // ����Ǿ��°�

	virtual void render(worm& player);
};

