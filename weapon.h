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

	virtual bool update(worm& player); // ����Ǿ��°�

	virtual void render(worm& player);
};

