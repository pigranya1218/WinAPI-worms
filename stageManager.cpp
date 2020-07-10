#include "stdafx.h"
#include "stageManager.h"
#include "wormManager.h"
#include "projectileManager.h"
#include "uiManager.h"

HRESULT stageManager::init(int playerNum, int wormsPerPlayer, int turnTime)
{
	_playerNum = playerNum;
	_wormsPerPlayer = wormsPerPlayer;

	_turnPlayer = RND->getInt(_playerNum);
	_turnWorm = RND->getInt(_wormsPerPlayer);

	int playerCount = 0;
	int wormCount = 0;

	while (true)
	{
		
		playerCount = 0;

		while (true)
		{
			int index = _turnPlayer * _wormsPerPlayer + _turnWorm;
			_turnQueue.push(index);

			_turnPlayer = (_turnPlayer + 1) % _playerNum;
			playerCount++;

			if (playerCount == playerNum)
			{
				break;
			}
		}

		_turnWorm = (_turnWorm + 1) % _wormsPerPlayer;
		wormCount++;

		if (wormCount == wormsPerPlayer)
		{
			break;
		}
	}

	_offsetBG = 0;

	_turnMaxTime = turnTime;
	_turnWind = 0;
	_zoom = 1;

	_magentaBrush = CreateSolidBrush(RGB(255, 0, 255));
	_magentaPen = CreatePen(BS_SOLID, 1, RGB(255, 0, 255));

	_state = STAGE_STATE::READY_TURN;
	_wormsDamages.resize(_playerNum * _wormsPerPlayer);

	return S_OK;
}

void stageManager::release()
{
}

void stageManager::update()
{
	_offsetBG += (_turnWind >= 0) ? -1 : 1; // 배경 이동
	_offsetSea += (_turnWind >= 0)? -1 : 1; // 바다 이동

	if (KEY_MANAGER->isStayKeyDown('A'))
	{
		CAMERA_MANAGER->setX(CAMERA_MANAGER->getX() - 5);
	}
	if (KEY_MANAGER->isStayKeyDown('D'))
	{
		CAMERA_MANAGER->setX(CAMERA_MANAGER->getX() + 5);
	}
	if (KEY_MANAGER->isStayKeyDown('W'))
	{
		CAMERA_MANAGER->setY(CAMERA_MANAGER->getY() - 5);
	}
	if (KEY_MANAGER->isStayKeyDown('S'))
	{
		CAMERA_MANAGER->setY(CAMERA_MANAGER->getY() + 5);
	}

	switch (_state)
	{
	case STAGE_STATE::READY_TURN: // 다음 턴을 위한 준비
	{
		setTurnWind();
		setNextTurnIndex();
		_turnTime = _turnMaxTime;
		_state = STAGE_STATE::WORM_TURN;
	}
	break;
	case STAGE_STATE::UPDATE_DEAD: // 죽음 처리, 경우에 따라 다시 waiting turn으로 돌아갈 수 있음
	{
		bool isFinish = true;
		for (int i = 0; i < _playerNum * _wormsPerPlayer; i++)
		{
			if (_wormManager->checkFreshDead(i))
			{
				_wormManager->setDead(i);
				_state = STAGE_STATE::WAITING_TURN;
				isFinish = false;
				break;
			}
		}
		if (isFinish)
		{
			_state = STAGE_STATE::READY_TURN;
		}
	}
	break;
	case STAGE_STATE::UPDATE_HP: // 대미지 처리
	{
		bool isFinish = true;
		for (int i = 0; i < _playerNum * _wormsPerPlayer; i++)
		{
			if (_wormsDamages[i] > 0 && !_wormManager->wormHpZero(i))
			{
				_wormsDamages[i] -= 1;
				_wormManager->wormHpDiscount(i, 1);
				isFinish = false;
			}
		}
		if (isFinish)
		{
			if (_damageFrame++ == 50)
			{
				_state = STAGE_STATE::UPDATE_DEAD;
			}
		}
	}
	break;
	case STAGE_STATE::WAITING_TURN:
	{
		if (_wormManager->checkAllStop() && _projectileManager->checkZero()) // 움직임이 끝날때까지 대기
		{
			_turnIndex = -1;
			_damageFrame = 0;
			_state = STAGE_STATE::UPDATE_HP;
		}
	}
	break;
	case STAGE_STATE::WORM_TURN:
	{
		// 타이머가 다 되었는지 파악하기
		if (_turnTime <= 0) 
		{
			_turnTime = 0;
			_state = STAGE_STATE::WAITING_TURN;
		}

		// 타이머 업데이트
		if (_turnTime > 0)
		{
			_turnTime -= TIME_MANAGER->getElapsedTime();
		}
	}
	break;
	}

	_uiManager->setTimer(ceil(_turnTime)); // UI 타이머 변경
	_uiManager->setTimerVisible(true);
	_uiManager->setWind(_turnWind); // UI 타이머 변경
	_uiManager->setWindVisible(true);
	
	_uiManager->setTeamHpVisible(true);
	_uiManager->setTeamHp(_wormManager->getTeamMaxHp(), _wormManager->getTeamHp());
}

void stageManager::render()
{
	int width = IMAGE_MANAGER->findImage("STAGE")->getWidth();
	int height = IMAGE_MANAGER->findImage("STAGE")->getHeight();

	// 배경 그리기
	RECT bgRect = { 0, 0, width, height};
	CAMERA_MANAGER->loopRender(getMemDC(), IMAGE_MANAGER->findImage("BACKGROUND"), bgRect, _offsetBG, 0);

	// 땅 그리기
	CAMERA_MANAGER->render(getMemDC(), _stageShadowDC, 0, 0, width, height, true, RGB(255, 0, 255));
	CAMERA_MANAGER->render(getMemDC(), _stageBackDC, 0, 0, width, height, true, RGB(255, 0, 255));
	CAMERA_MANAGER->render(getMemDC(), _stageDC, 0, 0, width, height, true, RGB(255, 0, 255));
	
	// 바다 그리기
	bgRect = { 0, height - IMAGE_MANAGER->findImage("SEA")->getHeight(), width, height};
	CAMERA_MANAGER->loopRender(getMemDC(), IMAGE_MANAGER->findImage("SEA"), bgRect, _offsetSea, 0);
	// IMAGE_MANAGER->findImage("redTest")->alphaRedRender(getMemDC(), 122);

	// CAMERA_MANAGER->render(getMemDC(), _tempDC, 0, 0, IMAGE_MANAGER->findImage("STAGE")->getWidth(), IMAGE_MANAGER->findImage("STAGE")->getHeight());
}

void stageManager::setWormManager(wormManager * wormManager)
{
	_wormManager = wormManager;
}

void stageManager::setProjectileManager(projectileManager* projectileManager)
{
	_projectileManager = projectileManager;
}

void stageManager::setUIManager(uiManager * uiManager)
{
	_uiManager = uiManager;
}

void stageManager::setNextTurnIndex()
{
	while (!_turnQueue.empty())
	{
		int index = _turnQueue.front();
		_turnQueue.pop();
		if (!_wormManager->checkDead(index))
		{
			_turnQueue.push(index);
			_turnIndex = index;
			return;
		}
	}

	_turnIndex = -1;
}

void stageManager::setTurnWind()
{
	int randomCount = RND->getInt(5);
	if (randomCount != 0) // wind change
	{
		_turnWind = RND->getFromFloatTo(-1, 1); // 랜덤 값 얻기
		_turnWind = round(_turnWind * 10) / 10; // 소수 첫째자리까지만
	}
}

void stageManager::setWormDamage(int index, int damage)
{
	_wormsDamages[index] += damage;
}

void stageManager::setWaiting()
{
	_state = STAGE_STATE::WAITING_TURN;
}

void stageManager::pixelBomb(float x, float y, float damage, float width) // 픽셀 폭파시키기
{
	SelectObject(_stageDC, _magentaBrush);
	SelectObject(_stageDC, _magentaPen);
	SelectObject(_stageBackDC, _magentaBrush);
	SelectObject(_stageBackDC, _magentaPen);
	SelectObject(_stageShadowDC, _magentaBrush);
	SelectObject(_stageShadowDC, _magentaPen);

	float radius = width / 2;
	RECT bombCircle = {x - radius, y - radius, x + radius, y + radius};
	Ellipse(_stageDC, bombCircle);
	float radiusMiddle = radius * 0.9;
	RECT backBombCircle = { x - radiusMiddle, y - radiusMiddle, x + radiusMiddle, y + radiusMiddle };
	Ellipse(_stageBackDC, backBombCircle);
	float radiusSmall = radius * 0.7;
	RECT shadowBombCircle = { x - radiusSmall, y - radiusSmall, x + radiusSmall, y + radiusSmall };
	Ellipse(_stageShadowDC, shadowBombCircle);
}

void stageManager::makeStage()
{
	makeWorld();
	makeWorms();
	setNextTurnIndex();
	setTurnWind();
}

void stageManager::makeWorld() // 맵 복사
{
	// 땅 이미지
	int width = IMAGE_MANAGER->findImage("STAGE")->getWidth();
	int height = IMAGE_MANAGER->findImage("STAGE")->getHeight();
	_stageDC = CreateCompatibleDC(getMemDC());
	HBITMAP originBitMap1 = static_cast<HBITMAP>(SelectObject(_stageDC, (HBITMAP)CreateCompatibleBitmap(getMemDC(), width, height)));
	
	BitBlt(_stageDC, 0, 0, width, height,
		IMAGE_MANAGER->findImage("STAGE")->getMemDC(), 0, 0, SRCCOPY);

	_stageBackDC = CreateCompatibleDC(getMemDC());
	HBITMAP originBitMap2 = static_cast<HBITMAP>(SelectObject(_stageBackDC, (HBITMAP)CreateCompatibleBitmap(getMemDC(), width, height)));
	BitBlt(_stageBackDC, 0, 0, width, height,
		IMAGE_MANAGER->findImage("STAGE_BACK")->getMemDC(), 0, 0, SRCCOPY);

	_stageShadowDC = CreateCompatibleDC(getMemDC());
	HBITMAP originBitMap3 = static_cast<HBITMAP>(SelectObject(_stageShadowDC, (HBITMAP)CreateCompatibleBitmap(getMemDC(), width, height)));
	BitBlt(_stageShadowDC, 0, 0, width, height,
		IMAGE_MANAGER->findImage("STAGE_SHADOW")->getMemDC(), 0, 0, SRCCOPY);

	/*for (int x = 0; x < IMAGE_MANAGER->findImage("STAGE")->getWidth(); x++)
	{
		for (int y = 0; y < IMAGE_MANAGER->findImage("STAGE")->getHeight(); y++)
		{
			COLORREF stageRGB = GetPixel(_stageDC, x, y);

			int r = GetRValue(stageRGB);
			int g = GetGValue(stageRGB);
			int b = GetBValue(stageRGB);

			if (r == 255 && g == 0 && b == 255)
			{
				SetPixel(_stageBackDC, x, y, RGB(255, 0, 255));
			}
			else
			{
				SetPixel(_stageBackDC, x, y, RGB(0, 0, 0));
			}
		}
	}*/

	DeleteObject(originBitMap1);
	DeleteObject(originBitMap2);
	DeleteObject(originBitMap3);
}

void stageManager::makeWorms() // 맵 곳곳에 웜즈 만들기
{
	int wormSize = _playerNum * _wormsPerPlayer; // 만들어야 하는 웜즈 수
	HDC stageDC = IMAGE_MANAGER->findImage("STAGE")->getMemDC();
	int stageWidth = IMAGE_MANAGER->findImage("STAGE")->getWidth();
	int stageHeight = IMAGE_MANAGER->findImage("STAGE")->getHeight();
	vector<pair<int, int>> alreadyExist; // 각 웜즈를 떨어뜨려 생성시키기 위해
	vector<bool> alreadyNames (_wormNames.size(), false);

	// 랜덤으로 위치를 잡아 차례로 생성하기 
	for (int i = 0; i < wormSize; i++)
	{
		bool isAvailPosition = false;
		bool isAvailName = false;
		int posX;
		int posY;
		string name;
		while (!isAvailName)
		{
			int nameIndex = RND->getInt(_wormNames.size());
			if (alreadyNames[nameIndex]) continue;
			isAvailName = true;
			alreadyNames[nameIndex] = true;
			name = _wormNames[nameIndex];
		}

		while (!isAvailPosition)
		{
			bool isValidX = false;
			int randomX; // 소환될 X 위치 결정 과정
			while (!isValidX)
			{
				isValidX = true;
				randomX = RND->getFromIntTo(100, stageWidth - 100);
				for (int j = 0; j < alreadyExist.size(); j++)
				{
					if (alreadyExist[j].first <= randomX && randomX <= alreadyExist[j].second)
					{
						isValidX = false;
						break;
					}
				}
			}
			
			for (int y = 0; y < stageHeight - 190; y++) // 소환될 Y 위치 결정 과정
			{
				COLORREF sourceRGB = GetPixel(stageDC, randomX, y);

				int R = GetRValue(sourceRGB);
				int G = GetGValue(sourceRGB);
				int B = GetBValue(sourceRGB);

				if (!(R == 255 && G == 0 && B == 255))
				{
					posX = randomX;
					posY = y;
					isAvailPosition = true;
					break;
				}
			}
		}
		alreadyExist.push_back(make_pair(posX - 60, posX + 60));
		_wormManager->addWorms(i, name, posX, posY); // 소환
	}
}

// 밟을 수 있는 땅을 얻는 함수, 디버깅용(오래 걸림)
HDC stageManager::getGroundDC()
{
	HDC groundDC = CreateCompatibleDC(getMemDC());
	int width = IMAGE_MANAGER->findImage("STAGE")->getWidth();
	int height = IMAGE_MANAGER->findImage("STAGE")->getHeight();
	SelectObject(groundDC, (HBITMAP)CreateCompatibleBitmap(getMemDC(), width, height));

	for (int y = 0; y < height - 190; y++)
	{
		for (int x = 0; x < width; x++)
		{
			COLORREF sourceRGB = GetPixel(IMAGE_MANAGER->findImage("STAGE")->getMemDC(), x, y);
			COLORREF upperRGB = GetPixel(IMAGE_MANAGER->findImage("STAGE")->getMemDC(), x, y - 1);

			int R = GetRValue(sourceRGB);
			int G = GetGValue(sourceRGB);
			int B = GetBValue(sourceRGB);

			int upperR = GetRValue(upperRGB);
			int upperG = GetGValue(upperRGB);
			int upperB = GetBValue(upperRGB);
		
			if ((!(R == 255 && G == 0 && B == 255)) &&
				(upperR == 255 && upperG == 0 && upperB == 255))
			{
				SetPixel(groundDC, x, y, RGB(0, 255, 0)); // 밟을 수 있는 땅은 초록색으로 라인을 그림
			}
		}
	}

	return groundDC;
}

