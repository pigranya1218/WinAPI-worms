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
	float _damage; // ������ �����
	float _bombWidth; // ������ ���� �ݰ�

	float _x, _y; // ����Ʈ ��ǥ

	void shot(worm& shooter);
	void setWaiting(worm& player);

public:
	donkeyWeapon() {}
	~donkeyWeapon() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual WEAPON_FINISH_TYPE update(worm& player); // ����Ǿ��°�

	virtual void render(worm& player);
};

