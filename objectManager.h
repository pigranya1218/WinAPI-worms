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

	void objectBomb(float x, float y, float power, float width);

	bool checkNoUpdate();

	void makeBombEffect(int x, int y, int width);

};

