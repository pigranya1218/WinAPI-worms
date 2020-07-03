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
	state* _state; // 상태
	weapon* _weapon; // 사용 중인 무기
	DIRECTION _dir; // 바라보는 방향
	SLOPE	_slope; // 현재 서있는 각도

	float _width = 30, _height = 30; // 웜즈 충돌 범위
	float _speed; // 웜즈의 이동 속도
	float _x, _y; // 웜즈의 중점
	int _offsetClimb; // 오를 수 있는 픽셀 차
	float _gravity; // 현재 받고 있는 중력
	RECT _rc; // 충돌 범위

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
	
	void move(); // 웜즈를 바라보고 있는 방향으로 이동시킴

	bool isTurn();
};

