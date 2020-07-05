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

class wormManager;

class worm : public gameNode
{
private:
	wormManager* _wormManager;

	int _index;
	state* _state; // ����
	weapon* _weapon; // ��� ���� ����
	DIRECTION _dir; // �ٶ󺸴� ����
	SLOPE	_slope; // ���� ���ִ� ����

	float _width = 20, _height = 20; // ���� �浹 ����
	float _speed = 15; // ������ �ʴ� �̵� �ȼ�
	float _x, _y; // ������ ����
	int _offsetClimb = 6; // ���� �� �ִ� �ȼ� ��
	int _offsetSlope = 1; // ������ ǥ���ϴ� �ȼ���
	float _gravity; // ���� �ް� �ִ� �߷�
	RECT _rc; // �浹 ����

	bool checkMoveAvail(int x, int y); // ������ �� �ִ��� x, y �ȼ��� _dir �������� �˻�
	int checkGroundAvail(int x, int y); // ���� ���� �ִ��� _y�� _dir �������� �˻�
	bool checkPixelAvail(int x, int y); // ���� ���� �ִ��� x�� y �˻�

public:
	worm() {}
	~worm() {}

	virtual HRESULT init(wormManager* stageManager, int index, float x, float y);
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
	void setDirection(DIRECTION dir) { _dir = dir;};
	void updateSlope(); // ���� ������Ʈ, �������� ���� �� ���
	void reverseSlope(); // ���⸦ �ݴ�� �������ִ� �޼���, �¿� ������ �ٲ� �� ���
	
	bool move(); // ��� �ٶ󺸰� �ִ� �������� �̵���Ŵ, false��� fallen ���·� ������
	bool gravityMove(); // �߷� ����� �ʿ��� �̵�, true��� ���� �ε����ٴ� ��
	bool isTurn();

	void setGravity(float gravity) { _gravity = gravity; }
	float getGravity() { return _gravity; }



	// ** DEBUG
	void setPlayerPos(float x, float y) { _x = x, _y = y, _rc = RectMakeCenter(_x, _y, _width, _height); }
};

