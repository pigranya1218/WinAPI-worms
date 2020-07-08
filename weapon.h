#pragma once
#include "gameNode.h"
#include "projectile.h"

class worm;

enum class WEAPON_FINISH_TYPE
{
	ATTACK, // 아직 공격 중
	MOVING, // 움직이는 중
	JUMPING, // 점프 중
	FINISH_BUT_MOVE, // 공격 종료이고, 움직일 여유가 있음. 하지만 공격은 불가능 (다이너마이트 등)
	FINISH // 공격 종료
};

enum class WEAPON_STATE
{
	BEGIN, // 무기 꺼내는 상태
	IDLE, // 각도 조정이나 이동하는 상태
	GAGING, // 기 모으는 상태 (스페이스바 누르고 있는 상태)
	SHOOTING, // 쏜 상태, 투사체가 있다면 투사체가 폭파할때까지 기다리는 상태
	WAITING, // 무언가가 일어나고 끝날때까지 기다리는 상태
	FINISH, // 무기 넣는 상태
};

class weapon : public gameNode
{
protected:
	float getRealAngle(worm& player, float angle);
	WEAPON_STATE _state;
	float _blobOffset = (float)100 / 16; // 게이지 오프셋

private:

public:
	weapon() {}
	~weapon() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual WEAPON_FINISH_TYPE update(worm& player); // 종료되었는가

	virtual void render(worm& player);
};

