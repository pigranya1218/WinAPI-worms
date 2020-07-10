#pragma once
#include "gameNode.h"
#include "projectile.h"

class donkey : public projectile
{
public:
	donkey() {}
	~donkey() {}
	
	virtual void update();
};

