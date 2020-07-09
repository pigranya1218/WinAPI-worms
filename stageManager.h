#pragma once
#include "gameNode.h"

class wormManager;
class uiManager;

class stageManager : public gameNode
{
private:
	wormManager* _wormManager;
	uiManager* _uiManager;

	int _playerNum; // �÷��̾� ���� 
	int _wormsPerPlayer; // �÷��̾�� �Ҵ�Ǵ� ���� ����
	int _turnPlayer; // ���� ������ �÷��̾� 
	int _turnWorm; // ���� ������ ����

	float _turnTime; // Ÿ�̸� ����

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

	vector<int> _wormsDamages; // ������� ����� ó���� ��Ƶδ� ����
	bool _wormsWaiting; // ��� Ȱ���� �� �ִ��� üũ

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
	void setUIManager(uiManager* uiManager);

	int getPlayerNum() { return _playerNum; }
	int getWormsPerPlayer() { return _wormsPerPlayer; }
	int getCurrentTurnIndex() { return (!_wormsWaiting)?(_turnPlayer * _wormsPerPlayer + _turnWorm):-1;}

	COLORREF getPixel(int x, int y) { return GetPixel(_stageBackDC, x, y); }

	void pixelBomb(float x, float y, float damage, float width);

	void makeStage();
};

