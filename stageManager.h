#pragma once
#include "gameNode.h"

class wormManager;

class stageManager : public gameNode
{
private:
	wormManager* _wormManager;

	int _playerNum; // 플레이어 숫자 
	int _wormsPerPlayer; // 플레이어당 할당되는 웜즈 숫자
	int _turnPlayer; // 현재 차례의 플레이어 
	int _turnWorm; // 현재 차례의 웜즈

	int _turnTime; // 타이머 숫자

	void makeWorld(); // 맵 그리기
	void makeWorms(); // 맵 곳곳에 랜덤 위치로 웜즈 지정하기

	HDC _backgroundDC; // 배경 이미지
	HDC _stageDC; // 스테이지 이미지

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

