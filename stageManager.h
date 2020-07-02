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

	HDC _backgroundDC; // ��� �̹���
	HDC _stageDC; // �������� �̹���

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

	void makeStage();

};

