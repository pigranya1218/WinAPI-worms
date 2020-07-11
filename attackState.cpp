#include "stdafx.h"
#include "allState.h"
#include "worm.h"
#include "stageManager.h"


void attackState::enter(worm& player)
{
	player.getWeapon()->enter(player);
}

void attackState::exit(worm& player)
{
}

state* attackState::update(worm& player)
{
	WEAPON_FINISH_TYPE isFinish = player.getWeapon()->update(player); // ������ ����� �����°�
	switch (isFinish)
	{
	case WEAPON_FINISH_TYPE::ATTACK: // ���� ����������
		return nullptr;
	case WEAPON_FINISH_TYPE::FINISH_BUT_MOVE:
	{
		player.getStageManager()->setTimer(5);
		player.setAttackAvail(false);
	}
	case WEAPON_FINISH_TYPE::MOVING:
		return new moveState;
	case WEAPON_FINISH_TYPE::JUMPING:
		return new jumpState;
	case WEAPON_FINISH_TYPE::FINISH:
		player.removeWeapon();
		return new idleState;
	}
}

void attackState::render(worm& player)
{
	player.getWeapon()->render(player);
}
