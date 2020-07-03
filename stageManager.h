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
	HDC getGroundDC(); // 웜즈 배치 위치 선정을 위한 알고리즘 땅 확인

	int _offsetBG; // 배경 이미지 LOOP 이동 변수
	float _zoom; // 카메라 배율
	HDC _stageDC; // 스테이지 이미지, 폭파 등 이미지 변경이 이루어질 수 있기에 따로 저장
	HDC _seaDC; // 바다 이미지, 바다가 출렁출렁하게끔...

	// **** 디버깅용 변수들
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

	COLORREF getPixel(int x, int y) { return GetPixel(_stageDC, x, y); }

	void makeStage();

};

