#pragma once
class item
{
private:

public:
	item() {}
	~item() {}

	virtual void release();

	virtual void update();

	virtual void render();
};

