#pragma once
#include "gameNode.h"
#include "item.h"

class itemManager : public gameNode
{
private:
	vector<item*> _items;

public:
	itemManager() {}
	~itemManager() {}

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};

