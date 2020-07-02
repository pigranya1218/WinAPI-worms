#pragma once

class worm;

class state
{
private:


public:
	state() {}
	~state() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);
};

