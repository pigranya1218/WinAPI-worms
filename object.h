#pragma once
#include "gameNode.h"

class stageManager;
class wormManager;

class object : public gameNode
{
protected:
	stageManager* _stageManager;
	wormManager* _wormManager;

	bool _isFinish;
	bool _isUpdate;

public:
	object() {}
	~object() {}

	virtual void release();

	virtual void update();

	virtual void render();

	bool isFinish() { return _isFinish; }

	bool isUpdate() { return _isUpdate; }
};

