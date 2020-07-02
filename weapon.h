#pragma once

class worm;

class weapon
{
private:

public:
	weapon() {}
	~weapon() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual bool update(worm& player); // 종료되었는가

	virtual void render(worm& player);
};

