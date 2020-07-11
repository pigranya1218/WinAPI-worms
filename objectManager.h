#pragma once
#include "gameNode.h"
#include "object.h"

class objectManager : public gameNode
{
private:
	vector<object*> _objects;

public:
	objectManager() {}
	~objectManager() {}

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void pushObject(object* object) { _objects.push_back(object); }

	bool checkNoUpdate();
};

