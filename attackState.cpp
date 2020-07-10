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
	WEAPON_FINISH_TYPE isFinish = player.getWeapon()->update(player); // ������ ����� �����°�
	switch (isFinish)
	{
	case WEAPON_FINISH_TYPE::ATTACK: // ���� ����������
		return nullptr;
	case WEAPON_FINISH_TYPE::MOVING:
	case WEAPON_FINISH_TYPE::FINISH_BUT_MOVE:
		return new moveState;
	case WEAPON_FINISH_TYPE::JUMPING:
		return new jumpState;
	case WEAPON_FINISH_TYPE::FINISH:
		return new idleState;
	}
}

void attackState::render(worm& player)
{
	player.getWeapon()->render(player);
}
