#pragma once
#include "object.h"

enum class MINE_STATE
{
	WAITING,
	ACTIVE
};

class mineObject : public object
{
private:
	image* _img;
	animation* _ani;
	float _powerChange = 0.8; // �ε��� ������ ���ҵ� �Ŀ�

	int _waitingCount = 0; // �۵��� ��ٸ��� ���� ī��Ʈ
	int _fireCount = 0; // ������ ��ٸ��� ���� ī��Ʈ
	float _damage = 40; // �����
	float _bombWidth = 120; // ���� �ݰ�
	float _detectWidth = 30; // ���� �ݰ�

	MINE_STATE _state = MINE_STATE::WAITING;

	bool checkMoveAvail(int x, int bot);
	bool checkPixelAvail(int x, int bot);
	bool gravityMove(float xPower);
	bool checkMineRange();
	void bomb();

public:
	mineObject() {}
	~mineObject() {}

	void init(stageManager* stageManager, wormManager* wormManager, objectManager* objectManager, float x, float y, float angle, float power) 
	{
		_stageManager = stageManager;
		_wormManager = wormManager;
		_objectManager = objectManager;
		_x = x;
		_y = y;
		_angle = angle;
		_power = power;
		_width = 2;
		_height = 2;
		_gravity = 0;

		_img = IMAGE_MANAGER->findImage("WEAPON_MINE_OFF");
		_ani = new animation;
		_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
		_ani->setDefPlayFrame(false, true);
		_ani->setFPS(40);
		_ani->start();
		_ani->frameUpdate(RND->getFloat(1));
	}

	virtual void release();

	virtual void update();

	virtual void render();
};

