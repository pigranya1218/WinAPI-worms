#pragma once
#include "gameNode.h"

class stageManager;
class wormManager;

class projectile : public gameNode
{
protected:
	stageManager* _stageManager;
	wormManager* _wormManager;

	image* _img;
	float _x, _y;
	float _width;
	float _height;
	float _gravity;
	float _wind;
	float _angle;
	float _angleDisplay;
	float _power;
	float _damage;
	float _frameAngle;
	float _frameAngleOffset;
	float _bombWidth;


	bool _isAngleFrame;
	bool _isWindEffected;
	bool _checkPixel;
	bool _checkWorm;
	bool _isFinish;

	bool checkPixelAvail(int x, int y);
	bool checkWormAvail(int x, int y);
	int getFrameIndex();

public:
	projectile() {}
	~projectile() {}

	void setImage(image* img) { _img = img; _frameAngle = PI2 / 16; _frameAngleOffset = _frameAngle / 2; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	void setWidth(float width) { _width = width; }
	void setHeight(float height) { _height = height; }
	void setGravity(float gravity) { _gravity = gravity; }
	void setWind(float wind) { _gravity = wind; }
	void setAngle(float angle) { _angle = angle; }
	void setPower(float power) { _power = power; }
	void setDamage(float damage) { _damage = damage; }
	void setBombWidth(float bombWidth) { _bombWidth = bombWidth; }

	void init(stageManager* stageManager, wormManager* wormManager, float x, float y, float width, float height, 
		float angle, float power, float damage, float bombWidth, bool isAngleFrame, bool isWindEffected, bool checkPixel, bool checkWorm)
	{
		_stageManager = stageManager;
		_wormManager = wormManager;
		_angleDisplay = angle;
		_isFinish = false;
		_isAngleFrame = isAngleFrame;
		_isWindEffected = isWindEffected;
		_checkPixel = checkPixel;
		_checkWorm = checkWorm;

		setX(x);
		setY(y);
		setWidth(width);
		setHeight(height);
		setGravity(0);
		setWind(0);
		setAngle(angle);
		setPower(power);
		setDamage(damage);
		setBombWidth(bombWidth);

	}

	float getX() { return _x; }
	float getY() { return _y; }
	float getGravity() { return _gravity; }
	float getAngle() { return _angle; }
	float getPower() { return _power; }

	bool isFinish() { return _isFinish; }

	bool gravityMove(float xPower); // true : ÇÈ¼¿°ú Ãæµ¹

	void release();

	virtual void update();

	void render();

};

