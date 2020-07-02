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
	// ��� �׸���
	CAMERA_MANAGER->render(getMemDC(), _backgroundDC, 0, 0, IMAGE_MANAGER->findImage("BACKGROUND")->getWidth(), IMAGE_MANAGER->findImage("BACKGROUND")->getHeight());
	// �� �׸���
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

void stageManager::makeWorld() // �� �׸���
{
	// ��� �̹���
	_backgroundDC = CreateCompatibleDC(getMemDC());
	SelectObject(_backgroundDC, (HBITMAP) CreateCompatibleBitmap(getMemDC(), IMAGE_MANAGER->findImage("BACKGROUND")->getWidth(), IMAGE_MANAGER->findImage("BACKGROUND")->getHeight()));
	BitBlt(_backgroundDC, 0, 0, IMAGE_MANAGER->findImage("BACKGROUND")->getWidth(), IMAGE_MANAGER->findImage("BACKGROUND")->getHeight(),
		IMAGE_MANAGER->findImage("BACKGROUND")->getMemDC(), 0, 0, SRCCOPY);

	// �� �̹���
	_stageDC = CreateCompatibleDC(getMemDC());
	SelectObject(_stageDC, (HBITMAP)CreateCompatibleBitmap(getMemDC(), IMAGE_MANAGER->findImage("STAGE")->getWidth(), IMAGE_MANAGER->findImage("STAGE")->getHeight()));
	TransparentBlt(_stageDC, 0, 0, IMAGE_MANAGER->findImage("STAGE")->getWidth(), IMAGE_MANAGER->findImage("STAGE")->getHeight(),
		IMAGE_MANAGER->findImage("STAGE")->getMemDC(), 0, 0, IMAGE_MANAGER->findImage("STAGE")->getWidth(), IMAGE_MANAGER->findImage("STAGE")->getHeight(), RGB(255, 0, 255));
}

void stageManager::makeWorms() // �� ������ ���� �����
{
	// ������ �ϴ� ���� ��
	int WormsSize = _playerNum * _wormsPerPlayer;

	// �������� ��ġ�� ��� ���ʷ� �����ϱ� 



}

