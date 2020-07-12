#include "stdafx.h"
#include "uiManager.h"
#include "allWeapon.h"
#include "wormManager.h"
#include "queue"

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

	RECT windRcL = { WINSIZEX - 404, WINSIZEY - 34, WINSIZEX - 202, WINSIZEY };
	RECT windRcR = { WINSIZEX - 202, WINSIZEY - 34, WINSIZEX, WINSIZEY };
	Rectangle(getMemDC(), windRcL);
	Rectangle(getMemDC(), windRcR);
	image* windImage;
	float width = 198;
	width *= abs(_wind);
	if (_wind >= 0) // RIGHT
	{
		windImage = IMAGE_MANAGER->findImage("WIND_RIGHT");
		_windOffset -= 1;
		_windOffset %= windImage->getWidth();
		
		RECT windImageRC = { WINSIZEX - 200, WINSIZEY - 32, WINSIZEX - 200 + width, WINSIZEY - 2 };

		windImage->loopRender(getMemDC(), windImageRC, _windOffset, 0);
	}
	else // LEFT
	{
		windImage = IMAGE_MANAGER->findImage("WIND_LEFT");
		_windOffset += 1;
		_windOffset %= windImage->getWidth();
		
		RECT windImageRC = { WINSIZEX - 204 - width, WINSIZEY - 32, WINSIZEX - 204, WINSIZEY - 2 };

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

	priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > queue; // 체력이 작은 팀부터 아래에 띄우기 위함
	for (int i = 0; i < _teamHps.size(); i++)
	{
		queue.push(make_pair(_teamHps[i], i));
	}

	while (!queue.empty())
	{
		pair<int, int> curr = queue.top();
		queue.pop();

		if (curr.first == 0) continue;
		
		int teamIndex = curr.second;

		SetTextColor(getMemDC(), _fontColors[teamIndex]);
		SelectObject(getMemDC(), _teamBrush[teamIndex]);
		SelectObject(getMemDC(), _black1Pen);

		int width = 80 * ((float)_teamHps[teamIndex] / _maxHp);
		RECT teamHp = { x , y - 10, x + width, y + 10 };
		Rectangle(getMemDC(), teamHp);

		SelectObject(getMemDC(), _blackBrush);
		SelectObject(getMemDC(), _white1Pen);
		RECT nameRc = { x - 80, y - 10, x - 1, y + 10 };
		Rectangle(getMemDC(), nameRc);

		sprintf_s(buffer, "PLAYER %d", (teamIndex + 1));
		TextOut(getMemDC(), x - 5, y - 8, buffer, strlen(buffer));

		y -= 20;
	}
}

void uiManager::drawWeapons()
{
	SelectObject(getMemDC(), _blackBrush);
	SelectObject(getMemDC(), _weaponFont);
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	SetTextAlign(getMemDC(), TA_RIGHT);

	int width = 35;
	int height = 35;
	int x = WINSIZEX - (width / 2);
	int y = (WINSIZEY - 38) - (height / 2);
	_weaponRC.clear();
	for (auto iter = _weaponCount.begin(); iter != _weaponCount.end(); iter++)
	{
		RECT weaponRc = RectMakeCenter(x, y, width, height);
		_weaponRC.push_back(make_pair(weaponRc, iter->first));

		if (PtInRect(&weaponRc, _ptMouse))
		{
			SelectObject(getMemDC(), _white2Pen);
		}
		else
		{
			SelectObject(getMemDC(), _white1Pen);
		}
		Rectangle(getMemDC(), weaponRc);

		char buffer[20];
		image* img = IMAGE_MANAGER->findImage("WEAPON_ICON_" + _weaponName[iter->first]);

		if (iter->second == -1) // 무한 사용 가능 무기
		{
			img->render(getMemDC(), x - (img->getWidth() / 2), y - (img->getHeight() / 2));
		}
		else 
		{
			if (iter->second == 0) // 무기가 없음
			{
				img->alphaRender(getMemDC(), x - (img->getWidth() / 2), y - (img->getHeight() / 2), 60);
			}
			else
			{
				img->render(getMemDC(), x - (img->getWidth() / 2), y - (img->getHeight() / 2));
			}
			sprintf_s(buffer, "%d", iter->second);
			TextOut(getMemDC(), weaponRc.right - 2, weaponRc.bottom - 14, buffer, strlen(buffer));
		}
		
		x -= width;
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
	_weaponFont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "궁서");

	for (int i = 0; i < _fontColors.size(); i++)
	{
		_teamBrush.push_back(CreateSolidBrush(_fontColors[i]));
	}

	_weaponName.insert(make_pair(WEAPON_CODE::BANANA, "BANANA"));
	_weaponName.insert(make_pair(WEAPON_CODE::BAZOOKA, "BAZOOKA"));
	_weaponName.insert(make_pair(WEAPON_CODE::DONKEY, "DONKEY"));
	_weaponName.insert(make_pair(WEAPON_CODE::TELEPORT, "TELEPORT"));
	_weaponName.insert(make_pair(WEAPON_CODE::MINE, "MINE"));
	_weaponName.insert(make_pair(WEAPON_CODE::HOMING, "HOMING"));

	return S_OK;
}

void uiManager::release()
{
}

void uiManager::update()
{
	if (_weaponVisible)
	{
		if (KEY_MANAGER->isStayKeyDown(VK_LBUTTON))
		{
			for (int i = 0; i < _weaponRC.size(); i++)
			{
				RECT weaponRC = _weaponRC[i].first;
				WEAPON_CODE weaponCode = _weaponRC[i].second;
				if (PtInRect(&weaponRC, _ptMouse) && (_weaponCount[weaponCode] == -1 || _weaponCount[weaponCode] > 0))
				{
					_wormManager->setWeaponsWorm(_weaponRC[i].second);
					_weaponVisible = false;
					break;
				}
			}
		}
	}
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

	if (_weaponVisible)
	{
		drawWeapons();
	}
}

void uiManager::setTeamHp(int maxHp, vector<int> hps)
{
	_maxHp = maxHp;
	_teamHps = hps;
}

void uiManager::setWeapons(map<WEAPON_CODE, int> weaponCount)
{
	_weaponCount = weaponCount;
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

}
