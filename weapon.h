#pragma once
#include "gameNode.h"

class worm;

enum class WEAPON_FINISH_TYPE
{
	ATTACK, // ���� ���� ��
	MOVING, // �����̴� ��
	JUMPING, // ���� ��
	FINISH_BUT_MOVE, // ���� �����̰�, ������ ������ ����. ������ ������ �Ұ��� (���̳ʸ���Ʈ ��)
	FINISH // ���� ����
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

	virtual WEAPON_FINISH_TYPE update(worm& player); // ����Ǿ��°�

	virtual void render(worm& player);
};

