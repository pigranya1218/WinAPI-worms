#pragma once
#include "gameNode.h"


class worm;
enum class SLOPE;

string getImageKey(string key, SLOPE slope);

class state : public gameNode
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

