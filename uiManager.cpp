#include "stdafx.h"
#include "uiManager.h"

// 타이머를 그림
void uiManager::drawTimer()
{

}

// 바람을 그림
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
	if (_timerVisible) // 타이머 그릴지
	{
		drawTimer();
	}

	if (_windVisible) // 바람 그릴지
	{
		drawWind();
	}
}
