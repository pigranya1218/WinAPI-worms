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
	float _angle; // ������ ���� 
	float _angleOffset; // ���� ���� ������
	float _frameAngleOffset;
	int _gauge; // ������ ������
	float _power; // ������ �Ŀ�
	float _damage; // ������ �����
	float _bombWidth; // ������ ���� �ݰ�
	float _aimRadius;
	
	virtual void shot(worm& shooter);
	virtual void setWaiting(worm& player);

public:
	bazookaWeapon() {}
	~bazookaWeapon() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual WEAPON_FINISH_TYPE update(worm& player); // ����Ǿ��°�

	virtual void render(worm& player);
};

