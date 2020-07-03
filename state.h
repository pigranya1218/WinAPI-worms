#pragma once
#include "gameNode.h"


class worm;
enum class SLOPE;

class state : public gameNode
{
protected:
	string getImageKey(string key, SLOPE slope);

private:
	

public:
	state() {}
	~state() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual state* update(worm& player);

	virtual void render(worm& player);
};

