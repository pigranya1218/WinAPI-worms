#include "stdafx.h"
#include "uiManager.h"

// Ÿ�̸Ӹ� �׸�
void uiManager::drawTimer()
{

}

// �ٶ��� �׸�
void uiManager::drawWind()
{

}

HRESULT uiManager::init()
{
	return S_OK;
}

void uiManager::release()
{
}

void uiManager::update()
{
}

void uiManager::render()
{
	if (_timerVisible) // Ÿ�̸� �׸���
	{
		drawTimer();
	}

	if (_windVisible) // �ٶ� �׸���
	{
		drawWind();
	}
}
