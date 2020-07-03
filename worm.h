#pragma once
#include "gameNode.h"
#include "state.h"
#include "weapon.h"

enum class DIRECTION
{
	LEFT,
	RIGHT
};

enum class SLOPE
{
	UP,
	NORMAL,
	DOWN
};

class stageManager;

class worm : public gameNode
{
private:
	stageManager* _stageManager;

	int _index;
	state* _state; // ����
	weapon* _weapon; // ��� ���� ����
	DIRECTION _dir; // �ٶ󺸴� ����
	SLOPE	_slope; // ���� ���ִ� ����

	float _width = 30, _height = 30; // ���� �浹 ����
	float _speed; // ������ �̵� �ӵ�
	float _x, _y; // ������ ����
	int _offsetClimb; // ���� �� �ִ� �ȼ� ��
	float _gravity; // ���� �ް� �ִ� �߷�
	RECT _rc; // �浹 ����

public:
	worm() {}
	~worm() {}

	virtual HRESULT init(stageManager* stageManager, int index, float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

	weapon* getWeapon() { return _weapon; }
	float getX() { return _x; }
	float getY() { return _y; }
	float getHeight() { return _height; }
	RECT getRect() { return _rc; }

	DIRECTION getDirection() { return _dir; }
	SLOPE	getSlope() { return _slope; }
	void setDirection(DIRECTION dir) { _dir = dir; updateSlope(); };
	void updateSlope();
	
	void move(); // ��� �ٶ󺸰� �ִ� �������� �̵���Ŵ

	bool isTurn();
};

