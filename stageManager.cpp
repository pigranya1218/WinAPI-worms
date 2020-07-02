#include "stdafx.h"
#include "stageManager.h"
#include "wormManager.h"

HRESULT stageManager::init(int playerNum, int wormsPerPlayer, int turnTime)
{
	_playerNum = playerNum;
	_wormsPerPlayer = wormsPerPlayer;

	_turnPlayer = RND->getInt(_playerNum);
	_turnWorm = RND->getInt(_wormsPerPlayer);

	_turnTime = turnTime;

	return S_OK;
}

void stageManager::release()
{
}

void stageManager::update()
{
	if (KEY_MANAGER->isStayKeyDown(VK_LEFT))
	{
		CAMERA_MANAGER->setX(CAMERA_MANAGER->getX() - 5);
	}
	if (KEY_MANAGER->isStayKeyDown(VK_RIGHT))
	{
		CAMERA_MANAGER->setX(CAMERA_MANAGER->getX() + 5);
	}
	if (KEY_MANAGER->isStayKeyDown(VK_UP))
	{
		CAMERA_MANAGER->setY(CAMERA_MANAGER->getY() - 5);
	}
	if (KEY_MANAGER->isStayKeyDown(VK_DOWN))
	{
		CAMERA_MANAGER->setY(CAMERA_MANAGER->getY() + 5);
	}
}

void stageManager::render()
{
	// 배경 그리기
	CAMERA_MANAGER->render(getMemDC(), _backgroundDC, 0, 0, IMAGE_MANAGER->findImage("BACKGROUND")->getWidth(), IMAGE_MANAGER->findImage("BACKGROUND")->getHeight());
	// 땅 그리기
	CAMERA_MANAGER->render(getMemDC(), _stageDC, 0, 0, IMAGE_MANAGER->findImage("STAGE")->getWidth(), IMAGE_MANAGER->findImage("STAGE")->getHeight());
}

void stageManager::setWormManager(wormManager * wormManager)
{
	_wormManager = wormManager;
}

void stageManager::makeStage()
{
	makeWorld();
	makeWorms();
}

void stageManager::makeWorld() // 맵 그리기
{
	// 배경 이미지
	_backgroundDC = CreateCompatibleDC(getMemDC());
	SelectObject(_backgroundDC, (HBITMAP) CreateCompatibleBitmap(getMemDC(), IMAGE_MANAGER->findImage("BACKGROUND")->getWidth(), IMAGE_MANAGER->findImage("BACKGROUND")->getHeight()));
	BitBlt(_backgroundDC, 0, 0, IMAGE_MANAGER->findImage("BACKGROUND")->getWidth(), IMAGE_MANAGER->findImage("BACKGROUND")->getHeight(),
		IMAGE_MANAGER->findImage("BACKGROUND")->getMemDC(), 0, 0, SRCCOPY);

	// 땅 이미지
	_stageDC = CreateCompatibleDC(getMemDC());
	SelectObject(_stageDC, (HBITMAP)CreateCompatibleBitmap(getMemDC(), IMAGE_MANAGER->findImage("STAGE")->getWidth(), IMAGE_MANAGER->findImage("STAGE")->getHeight()));
	TransparentBlt(_stageDC, 0, 0, IMAGE_MANAGER->findImage("STAGE")->getWidth(), IMAGE_MANAGER->findImage("STAGE")->getHeight(),
		IMAGE_MANAGER->findImage("STAGE")->getMemDC(), 0, 0, IMAGE_MANAGER->findImage("STAGE")->getWidth(), IMAGE_MANAGER->findImage("STAGE")->getHeight(), RGB(255, 0, 255));
}

void stageManager::makeWorms() // 맵 곳곳에 웜즈 만들기
{
	// 만들어야 하는 웜즈 수
	int WormsSize = _playerNum * _wormsPerPlayer;

	// 랜덤으로 위치를 잡아 차례로 생성하기 



}

