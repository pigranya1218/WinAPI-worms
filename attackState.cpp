#include "stdafx.h"
#include "allState.h"
#include "worm.h"


void attackState::enter(worm& player)
{
	player.getWeapon()->enter(player);
}

void attackState::exit(worm& player)
{
	player.getWeapon()->exit(player);
}

state* attackState::update(worm& player)
{
	bool isFinish = player.getWeapon()->update(player);
	if (isFinish)
	{
		return new idleState();
	} 
	return nullptr;
}

void attackState::render(worm& player)
{
	player.getWeapon()->render(player);
}
