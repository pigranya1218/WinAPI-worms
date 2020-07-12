#include "stdafx.h"
#include "object.h"

void object::release()
{
}

void object::update()
{
}

void object::render()
{
}

void object::hit(float angle, float power)
{
	_angle = angle;
	_power = power;
	_gravity = 0;
}
