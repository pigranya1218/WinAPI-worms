#pragma once
#include "gameNode.h"

class wormManager;

class stageManager : public gameNode
{
private:
	wormManager* _wormManager;

	int _playerNum; // �÷��̾� ���� 
	int _wormsPerPlayer; // �÷��̾�� �Ҵ�Ǵ� ���� ����
	int _turnPlayer; // ���� ������ �÷��̾� 
	int _turnWorm; // ���� ������ ����

	int _turnTime; // Ÿ�̸� ����

	void makeWorld(); // �� �׸���
	void makeWorms(); // �� ������ ���� ��ġ�� ���� �����ϱ�
	HDC getGroundDC(); // ���� ��ġ ��ġ ������ ���� �˰��� �� Ȯ��

	int _offsetBG; // ��� �̹��� LOOP �̵� ����
	float _zoom; // ī�޶� ����
	HDC _stageDC; // �������� �̹���, ���� �� �̹��� ������ �̷���� �� �ֱ⿡ ���� ����
	HDC _stageBackDC; // ���ķ� ����� �׵θ��� ǥ���ϱ� ���� DC
	HDC _stageShadowDC; // ���ķ� ����� ��ü���� ǥ���ϱ� ���� DC
	HBRUSH _magentaBrush;
	HPEN _magentaPen;

	// **** ������ ������
	HDC _tempDC;

public:
	stageManager() {}
	~stageManager() {}

	virtual HRESULT init(int playerNum, int wormsPerPlayer, int turnTime);
	virtual void release();
	virtual void update();
	virtual void render();

	void setWormManager(wormManager* wormManager);

	int getPlayerNum() { return _playerNum; }
	int getWormsPerPlayer() { return _wormsPerPlayer; }
	int getCurrentTurnIndex() { return _turnPlayer * _wormsPerPlayer + _turnWorm;}

	COLORREF getPixel(int x, int y) { return GetPixel(_stageBackDC, x, y); }

	void bomb(float x, float y, float damage, float width);

	void makeStage();
};

