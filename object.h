#pragma once
class object
{
private:

public:
	object() {}
	~object() {}

	virtual void release();

	virtual void update();

	virtual void render();
};

