#include "stdafx.h"
#include "weapon.h"
#include "worm.h"

float weapon::getRealAngle(worm& player, float angle)
{
	float realAngle = 0;
	if (player.getDirection() == DIRECTION::LEFT)
	{
		realAngle = (PI - angle) + PI / 2;
	} 
	else
	{
		realAngle = angle - PI / 2;
	}
	
	return realAngle;
}

void weapon::enter(worm & player)
{
}

void weapon::exit(worm & player)
{
}

WEAPON_FINISH_TYPE weapon::update(worm & player)
{
	return WEAPON_FINISH_TYPE::FINISH;
}

void weapon::render(worm & player)
{
}
