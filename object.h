#pragma once
#include "gameNode.h"

class stageManager;
class wormManager;
class objectManager;

class object : public gameNode
{
protected:
	stageManager* _stageManager;
	wormManager* _wormManager;
	objectManager* _objectManager;

	bool _isFinish;
	bool _isUpdate;

	float _x, _y;
	float _width, _height;
	float _angle, _power;
	float _gravity;
	RECT _rc;

public:
	object() {}
	~object() {}

	virtual void release();

	virtual void update();

	virtual void render();

	bool isFinish() { return _isFinish; }

	bool isUpdate() { return _isUpdate; }

	float getX() { return _x; }
	float getY() { return _y; }
	RECT getRect() { return RectMakeCenter(_x, _y, _width, _height); }

	void hit(float angle, float power);
};

