#pragma once
class projectile
{
private:


public:
	projectile() {}
	~projectile() {}

	virtual void release();

	virtual void update();

	virtual void render();

};

