#pragma once
#include "gameNode.h"

class uiManager : public gameNode
{
private:
	bool _timerVisible = false;
	int _timer = 0;

	bool _windVisible = false;
	float _wind = 0;

	void drawTimer();
	void drawWind();


public:
	uiManager() {}
	~uiManager() {}

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setTimerVisible(bool visible) { _timerVisible = visible; }
	void setTimer(int timer) { _timer = timer; }

	void setWindVisible(bool visible) { _windVisible = visible; }
	void setWind(float wind) { _wind = wind; }
};

