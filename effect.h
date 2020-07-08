#pragma once
#include "gameNode.h"

class animation;

class effect : public gameNode
{
protected:
	int _x;
	int _y;
	int _width;
	int _height;

	image*		_effectImage;
	animation*	_effectAnimation;
	BOOL		_isRunning;
	float		_elapsedTime;

public:
	effect();
	~effect();

	virtual HRESULT init(image* effectImage, int frameW, int frameH, int fps);
	virtual void release();
	virtual void update();
	virtual void render();

	void startEffect(int x, int y);
	void startEffect(int x, int y, int width, int height);

	virtual void killEffect();

	BOOL getIsRunning() { return _isRunning; }
};

