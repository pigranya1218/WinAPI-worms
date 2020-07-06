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
	state* _state; // 상태
	weapon* _weapon; // 사용 중인 무기
	DIRECTION _dir; // 바라보는 방향
	SLOPE	_slope; // 현재 서있는 각도

	float _width = 20, _height = 20; // 웜즈 충돌 범위
	float _speed = 0.4; // 웜즈의 프레임당 이동 픽셀
	float _x, _y; // 웜즈의 중점
	int _offsetClimb = 6; // 오를 수 있는 픽셀 차
	int _offsetSlope = 1; // 기울어짐 표현하는 픽셀차
	float _angle;
	float _power;
	float _gravity; // 현재 받고 있는 중력
	RECT _rc; // 충돌 범위

	bool checkMoveAvail(int x, int y); // 움직일 수 있는지 x, y 픽셀과 _dir 기준으로 검사
	int checkGroundAvail(int x, int y); // 밟을 땅이 있는지 _y와 _dir 기준으로 검사
	bool checkPixelAvail(int x, int y); // 밟을 땅이 있는지 x와 y 검사

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
	void updateSlope(); // 기울기 업데이트, 움직임이 끝난 뒤 사용
	void reverseSlope(); // 기울기를 반대로 설정해주는 메서드, 좌우 방향이 바뀔 때 사용
	
	bool move(); // 웜즈를 바라보고 있는 방향으로 이동시킴, false라면 fallen 상태로 돌리기
	bool gravityMove(float xPower); // 중력 계산이 필요한 이동, true라면 땅에 부딪혔다는 뜻
	bool isTurn();

	float getGravity() { return _gravity; }
	float getAngle() { return _angle; }
	float getPower() { return _power; }
	void setGravity(float gravity) { _gravity = gravity; }
	void setAngle(float angle) { _angle = angle; }
	void setPower(float power) { _power = power; }



	// ** DEBUG
	void setPlayerPos(float x, float y) { _x = x, _y = y, _rc = RectMakeCenter(_x, _y, _width, _height); }
};

