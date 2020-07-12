#pragma once
#include "gameNode.h"
#include "state.h"
#include "weapon.h"

enum class DIRECTION
{
	LEFT,
	RIGHT
};

enum class SLOPE
{
	UP,
	NORMAL,
	DOWN
};

enum class WEAPON_CODE;

class wormManager;
class objectManager;

class worm : public gameNode
{
private:
	wormManager* _wormManager;

	int _index;
	string _name;
	state* _state; // ����
	weapon* _weapon; // ��� ���� ����
	DIRECTION _dir; // �ٶ󺸴� ����
	SLOPE	_slope; // ���� ���ִ� ����
	bool _canAttack; // ������ ��������

	float _width = 20, _height = 20; // ���� �浹 ����
	float _speed = 0.4; // ������ �����Ӵ� �̵� �ȼ�
	float _x, _y; // ������ ����
	int _offsetClimb = 6; // ���� �� �ִ� �ȼ� ��
	int _offsetSlope = 1; // ������ ǥ���ϴ� �ȼ���
	int _maxHp = 100; // �ִ� HP
	int _currHp = _maxHp; // ���� HP
	float _angle; // ���� �����̴� ����
	float _displayAngle; // ���� ���̴� ����
	float _power; // ���� �����̴� �Ŀ�
	float _gravity; // ���� �ް� �ִ� �߷�
	RECT _rc; // �浹 ����

	map<WEAPON_CODE, int> _weaponCount;

public:
	worm() {}
	~worm() {}

	virtual HRESULT init(wormManager* stageManager, int index, string name, float x, float y);
	virtual void release();
	virtual void update();
	virtual void render();

	weapon* getWeapon() { return _weapon; }
	float getX() { return _x; }
	float getY() { return _y; }
	float getWidth() { return _width; }
	float getHeight() { return _height; }
	RECT getRect() { return _rc; }

	bool checkMoveAvail(int x, int y); // ������ �� �ִ��� x, y �ȼ��� _dir �������� �˻�
	int checkGroundAvail(int x, int y); // ���� ���� �ִ��� _y�� _dir �������� �˻�
	bool checkPixelAvail(int x, int y); // ���� ���� �ִ��� x�� y �˻�

	DIRECTION getDirection() { return _dir; }
	SLOPE	getSlope() { return _slope; }
	void setDirection(DIRECTION dir) { _dir = dir;};
	void updateSlope(); // ���� ������Ʈ, �������� ���� �� ���
	void reverseSlope(); // ���⸦ �ݴ�� �������ִ� �޼���, �¿� ������ �ٲ� �� ���
	
	bool move(); // ��� �ٶ󺸰� �ִ� �������� �̵���Ŵ, false��� fallen ���·� ������
	bool gravityMove(float xPower, float powerChange); // �߷� ����� �ʿ��� �̵�, true��� ���� �ε����ٴ� ��
	bool isTurn();

	float getGravity() { return _gravity; }
	float getAngle() { return _angle; }
	float getDisplayAngle() { return _displayAngle; }
	float getPower() { return _power; }
	void setGravity(float gravity) { _gravity = gravity; }
	void setAngle(float angle) { _angle = angle; }
	void setPower(float power) { _power = power; }
	void setDead();

	bool isFreshDead();
	bool isDead();
	bool isStop();

	stageManager* getStageManager();
	wormManager* getWormManager();
	objectManager* getObjectManager();

	void hit(float angle, float power);
	void setDamage(int damage);
	void discountHp(int discount) { _currHp = max(0, _currHp - discount); }
	void adjustWeapon(WEAPON_CODE weaponCode, int diff) { _weaponCount[weaponCode] += diff; }
	bool checkHpZero() { return (_currHp == 0); }
	void renderUI(); // hp�� �̸� �׸��� �Լ�
	void setWaiting();
	void removeWeapon();

	map<WEAPON_CODE, int> getWeaponCount() { return _weaponCount; }
	void setWeapon(weapon* weapon);

	int getHp() { return _currHp; }
	int getMaxHp() { return _maxHp; }

	void setAttackAvail(bool canAttack) { _canAttack = canAttack; }
	bool getAttackAvail() {return _canAttack;}

	// ** DEBUG
	void setPlayerPos(float x, float y) { _x = x, _y = y, _rc = RectMakeCenter(_x, _y, _width, _height); }
};

