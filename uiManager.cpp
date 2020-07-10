#include "stdafx.h"
#include "uiManager.h"

// 타이머를 그림
void uiManager::drawTimer()
{
	SetBkMode(getMemDC(), TRANSPARENT);
	SetTextAlign(getMemDC(), TA_CENTER);
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	SelectObject(getMemDC(), _blackBrush);
	SelectObject(getMemDC(), _white2Pen);
	SelectObject(getMemDC(), _timerFont);

	RECT timerRc = { 0, WINSIZEY - 70, 140, WINSIZEY };
	Rectangle(getMemDC(), timerRc);

	char buffer[20];
	sprintf_s(buffer, "%d", _timer);
	TextOut(getMemDC(), 70, WINSIZEY - 65, buffer, strlen(buffer));
}

// 바람을 그림
void uiManager::drawWind()
{
	SelectObject(getMemDC(), _blackBrush);
	SelectObject(getMemDC(), _white2Pen);

	RECT windRcL = { WINSIZEX - 204, WINSIZEY - 34, WINSIZEX - 102, WINSIZEY };
	RECT windRcR = { WINSIZEX - 102, WINSIZEY - 34, WINSIZEX, WINSIZEY };
	Rectangle(getMemDC(), windRcL);
	Rectangle(getMemDC(), windRcR);
	image* windImage;
	float width = 98;
	width *= abs(_wind);
	if (_wind >= 0) // RIGHT
	{
		windImage = IMAGE_MANAGER->findImage("WIND_RIGHT");
		_windOffset -= 1;
		_windOffset %= windImage->getWidth();
		
		RECT windImageRC = { WINSIZEX - 100, WINSIZEY - 32, WINSIZEX - 100 + width, WINSIZEY - 2 };

		windImage->loopRender(getMemDC(), windImageRC, _windOffset, 0);
	}
	else // LEFT
	{
		windImage = IMAGE_MANAGER->findImage("WIND_LEFT");
		_windOffset += 1;
		_windOffset %= windImage->getWidth();
		
		RECT windImageRC = { WINSIZEX - 104 - width, WINSIZEY - 32, WINSIZEX - 104, WINSIZEY - 2 };

		windImage->loopRender(getMemDC(), windImageRC, _windOffset, 0);
	}
}

void uiManager::drawTeamHp()
{
	SelectObject(getMemDC(), _wormFont);
	SetTextAlign(getMemDC(), TA_RIGHT); // 오른쪽 정렬

	int x = WINSIZEX / 2 - 20;
	int y = WINSIZEY - 10;
	char buffer[50];

	for (int i = _teamHps.size() - 1; i >= 0; i--)
	{
		if (_teamHps[i] == 0) continue;
		SetTextColor(getMemDC(), _fontColors[i]);
		SelectObject(getMemDC(), _teamBrush[i]);
		SelectObject(getMemDC(), _black1Pen);
		
		int width = 80 * ((float) _teamHps[i] / _maxHp);
		RECT teamHp = {x , y - 10, x + width, y + 10};
		Rectangle(getMemDC(), teamHp);

		SelectObject(getMemDC(), _blackBrush);
		SelectObject(getMemDC(), _white1Pen);
		RECT nameRc = {x - 80, y - 10, x - 1, y + 10};
		Rectangle(getMemDC(), nameRc);

		sprintf_s(buffer, "PLAYER %d", (i + 1));
		TextOut(getMemDC(), x - 5, y - 8, buffer, strlen(buffer));

		y -= 20;
	}

}

HRESULT uiManager::init()
{
	_blackBrush = CreateSolidBrush(RGB(0, 0, 0));

	_white1Pen = CreatePen(BS_SOLID, 1, RGB(255, 255, 255));
	_white2Pen = CreatePen(BS_SOLID, 2, RGB(255, 255, 255));
	_black1Pen = CreatePen(BS_SOLID, 1, RGB(0, 0, 0));

	_timerFont = CreateFont(60, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "궁서");
	_wormFont = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "궁서");

	for (int i = 0; i < _fontColors.size(); i++)
	{
		_teamBrush.push_back(CreateSolidBrush(_fontColors[i]));
	}

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

	if (_teamHpVisible)
	{
		drawTeamHp();
	}
}

void uiManager::setTeamHp(int maxHp, vector<int> hps)
{
	_maxHp = maxHp;
	_teamHps = hps;
}

void uiManager::drawWormUI(float x, float y, int hp, int playerIndex, string name)
{
	SetTextColor(getMemDC(), _fontColors[playerIndex]); // 플레이어마다 폰트 색깔 변경
	SetTextAlign(getMemDC(), TA_CENTER);
	SelectObject(getMemDC(), _blackBrush);
	SelectObject(getMemDC(), _white1Pen);
	SelectObject(getMemDC(), _wormFont);

	char buffer[100];
	sprintf_s(buffer, "%d", hp);

	RECT hpRc = { x - 20, y - 40, x + 20, y - 20 }; // hp RECT
	CAMERA_MANAGER->rectangle(getMemDC(), hpRc);
	TextOut(getMemDC(), CAMERA_MANAGER->getRelativeL(x), CAMERA_MANAGER->getRelativeT(y - 38), buffer, strlen(buffer));

	sprintf_s(buffer, "%s", name.c_str());

	int size = name.size();
	RECT nameRc; // name RECT
	if (size % 2 == 0) // 짝수인 경우
	{
		nameRc = { (int)(x - (6.5 * size)) , (int)y - 65, (int)(x + (6.5 * size)), (int)y - 42 };
	} 
	else
	{
		nameRc = { (int)(x - (6.5 * size) - 6.5) , (int)y - 65, (int)(x + (6.5 * size) + 6.5), (int)y - 42 };
	}

	CAMERA_MANAGER->rectangle(getMemDC(), nameRc);
	TextOut(getMemDC(), CAMERA_MANAGER->getRelativeL(x), CAMERA_MANAGER->getRelativeT(y - 62), buffer, strlen(buffer));

	SetTextColor(getMemDC(), RGB(0, 0, 0)); // 폰트 색깔 원상복귀
}
