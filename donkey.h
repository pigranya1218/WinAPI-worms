#pragma once
#include "projectile.h"

class donkey : public projectile
{
public:
	donkey() {}
	~donkey() {}
	
	virtual void update();
};

