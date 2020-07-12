#pragma once
#include "gameNode.h"
#include "projectile.h"

class worm;

enum class WEAPON_FINISH_TYPE
{
	ATTACK, // ���� ���� ��
	MOVING, // �����̴� ��
	JUMPING, // ���� ��
	FALLEN_BUT_MOVE, // �������� ��
	FINISH_BUT_MOVE, // ���� �����̰�, ������ ������ ����. ������ ������ �Ұ��� (���̳ʸ���Ʈ ��)
	FINISH // ���� ����
};

enum class WEAPON_STATE
{
	BEGIN, // ���� ������ ����
	IDLE, // ���� �����̳� �̵��ϴ� ����
	GAUGING, // �� ������ ���� (�����̽��� ������ �ִ� ����)
	SHOOTING, // �� ����, ����ü�� �ִٸ� ����ü�� �����Ҷ����� ��ٸ��� ����
	WAITING, // ���𰡰� �Ͼ�� ���������� ��ٸ��� ����
	FINISH, // ���� �ִ� ����
};

class weapon : public gameNode
{
protected:
	float getRealAngle(worm& player, float angle);
	WEAPON_STATE _state;
	float _blobOffset = (float)100 / 16; // ������ ������

private:

public:
	weapon() {}
	~weapon() {}

	virtual void enter(worm& player);

	virtual void exit(worm& player);

	virtual WEAPON_FINISH_TYPE update(worm& player); // ����Ǿ��°�

	virtual void render(worm& player);
};

