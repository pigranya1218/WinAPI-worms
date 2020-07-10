#pragma once
#include "gameNode.h"
#include <queue>

class wormManager;
class projectileManager;
class uiManager;

enum class STAGE_STATE
{
	WORM_TURN, // ������ ��, �÷��� ��
	WAITING_TURN, // ������ �Ͽ��� ó������ ���� �͵� ó���ϱ�
	UPDATE_HP, // ������� HP ��� ó��
	UPDATE_DEAD, // ������� ���� ó��
	READY_TURN // ���� ���� ���� �غ�
};

class stageManager : public gameNode
{
private:
	wormManager* _wormManager;
	projectileManager* _projectileManager;
	uiManager* _uiManager;

	int _playerNum; // �÷��̾� ���� 
	int _wormsPerPlayer; // �÷��̾�� �Ҵ�Ǵ� ���� ����
	int _turnPlayer; // ���� ������ �÷��̾� 
	int _turnWorm; // ���� ������ ����
	int _turnIndex; //  ���� ������ ���� �ε��� ��ȣ
	queue<int> _turnQueue;

	STAGE_STATE _state; // ���� ����

	float _turnMaxTime; // Ÿ�̸� �ʱ�ȭ ��
	float _turnTime; // Ÿ�̸� ����

	float _turnWind; // ���� �ٶ� (-1 ~ 1)

	void makeWorld(); // �� �׸���
	void makeWorms(); // �� ������ ���� ��ġ�� ���� �����ϱ�
	HDC getGroundDC(); // ���� ��ġ ��ġ ������ ���� �˰��� �� Ȯ��

	int _offsetBG; // ��� �̹��� LOOP �̵� ����
	int _offsetSea; // �ٴ� �̹��� LOOP �̵� ����
	float _zoom; // ī�޶� ����
	HDC _stageDC; // �������� �̹���, ���� �� �̹��� ������ �̷���� �� �ֱ⿡ ���� ����
	HDC _stageBackDC; // ���ķ� ����� �׵θ��� ǥ���ϱ� ���� DC
	HDC _stageShadowDC; // ���ķ� ����� ��ü���� ǥ���ϱ� ���� DC
	HBRUSH _magentaBrush;
	HPEN _magentaPen;

	vector<int> _wormsDamages; // ������� ����� ó���� ��Ƶδ� ����
	int _damageFrame; // ���� ����� ���� ������
	bool _wormsWaiting; // ��� Ȱ���� �� �ִ��� üũ

	// ������ �̸���
	vector<string> _wormNames = { "Kiin", "Ikksu", "Rookie", "Faker", "Nuguri", 
							"Fate", "Tarzan", "Clid", "BDD", "Chovy",
							"Mickey", "Teddy", "Mystic", "Ruler", "Effort",
							"Canna", "Cuzz", "Knight", "Kanavi", "TheShy",
							"Fly", "Uzi", "Bang", "Wolf", "Bengi",
							"Kakao", "KKW", "Aiming", "Tusin", "Keria",
							"Deft"};

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
	void setProjectileManager(projectileManager* projectileManager);
	void setUIManager(uiManager* uiManager);

	int getPlayerNum() { return _playerNum; }
	int getWormsPerPlayer() { return _wormsPerPlayer; }
	int getCurrentTurnIndex() { return (!_wormsWaiting)? _turnIndex : -1;}
	float getWind() { return _turnWind; }
	bool isTurn() { return (_state == STAGE_STATE::WORM_TURN); }
	void setNextTurnIndex();
	void setTurnWind();
	void setWormDamage(int index, int damage);
	void setWaiting();

	COLORREF getPixel(int x, int y) { return GetPixel(_stageBackDC, x, y); }

	void pixelBomb(float x, float y, float damage, float width);

	void makeStage();
};

