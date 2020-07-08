#pragma once
#include "gameNode.h"

class stageManager;
class wormManager;

class projectile : public gameNode
{
private:
	stageManager* _stageManager;
	wormManager* _wormManager;

	image* _img;
	animation* _ani;
	float _x, _y;
	float _gravity;
	float _wind;
	float _angle;
	float _angleDisplay;
	float _power;
	float _frameAngle;
	float _frameAngleOffset;

	bool checkPixelAvail(int x, int y);
	int getFrameIndex();

public:
	projectile() {}
	~projectile() {}

	void setImage(image* img) { _img = img; _frameAngle = PI2 / 16; _frameAngleOffset = _frameAngle / 2; }
	void setAnimation(animation* ani) { _ani = ani; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	void setGravity(float gravity) { _gravity = gravity; }
	void setWind(float wind) { _gravity = wind; }
	void setAngle(float angle) { _angle = angle; }
	void setPower(float power) { _power = power; }

	void init(stageManager* stageManager, wormManager* wormManager, float x, float y, float angle, float power)
	{
		_stageManager = stageManager;
		_wormManager = wormManager;
		_angleDisplay = angle;

		setX(x);
		setY(y);
		setGravity(0);
		setWind(0);
		setAngle(angle);
		setPower(power);
	}

	float getX() { return _x; }
	float getY() { return _y; }
	float getGravity() { return _gravity; }
	float getAngle() { return _angle; }
	float getPower() { return _power; }

	bool gravityMove(float xPower); // true : ÇÈ¼¿°ú Ãæµ¹

	void release();

	void update();

	void render();

};

