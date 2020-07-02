#pragma once
#include "gameNode.h"
#include "state.h"
#include "weapon.h"

class worm : public gameNode
{
private:
	state* _state; // ����
	weapon* _weapon; // ��� ���� ����

	float _x, _y; // ������ ����

public:
	worm() {}
	~worm() {}

	virtual HRESULT init(float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

	weapon* getWeapon() { return _weapon; }
	float getX() { return _x; }
	float getY() { return _y; }
};

