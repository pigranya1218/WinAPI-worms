#pragma once
#include "gameNode.h"

class worm;

enum class WEAPON_FINISH_TYPE
{
	ATTACK, // 아직 공격 중
	MOVING, // 움직이는 중
	JUMPING, // 점프 중
	FINISH_BUT_MOVE, // 공격 종료이고, 움직일 여유가 있음. 하지만 공격은 불가능 (다이너마이트 등)
	FINISH // 공격 종료
};

class weapon : public gameNode
{
protected:
	float getRealAngle(worm& player, float angle);

private:

public:
	weapon() {}
	~weapon() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual WEAPON_FINISH_TYPE update(worm& player); // 종료되었는가

	virtual void render(worm& player);
};

