#include "stdafx.h"
#include "stageManager.h"
#include "wormManager.h"
#include "projectileManager.h"
#include "objectManager.h"
#include "mineObject.h"
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
	_offsetBG += (_turnWind >= 0) ? -1 : 1; // ��� �̵�
	_offsetSea += (_turnWind >= 0)? -1 : 1; // �ٴ� �̵�

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
	case STAGE_STATE::READY_TURN: // ���� ���� ���� �غ�
	{
		setTurnWind();
		setNextTurnIndex();
		_turnTime = _turnMaxTime;
		_state = STAGE_STATE::WORM_TURN;
		_wormManager->setWeaponsUI(_turnIndex); // ���� UI
		_wormManager->setWormsAttackAvail();
		CAMERA_MANAGER->setX(_wormManager->getWormPosX(_turnIndex));
		CAMERA_MANAGER->setY(_wormManager->getWormPosY(_turnIndex));
	}
	break;
	case STAGE_STATE::UPDATE_DEAD: // ���� ó��, ��쿡 ���� �ٽ� waiting turn���� ���ư� �� ����
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
	case STAGE_STATE::UPDATE_HP: // ����� ó��
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
		if (_wormManager->checkAllStop() && _projectileManager->checkZero() && _objectManager->checkNoUpdate()) // �������� ���������� ���
		{
			_turnIndex = -1;
			_damageFrame = 0;
			_state = STAGE_STATE::UPDATE_HP;
		}
	}
	break;
	case STAGE_STATE::WORM_TURN:
	{
		// Ÿ�̸Ӱ� �� �Ǿ����� �ľ��ϱ�
		if (_turnTime <= 0) 
		{
			_turnTime = 0;
			_state = STAGE_STATE::WAITING_TURN;
			_uiManager->setWeaponVisible(false);
		}

		// Ÿ�̸� ������Ʈ
		if (_turnTime > 0)
		{
			_turnTime -= TIME_MANAGER->getElapsedTime();
		}

		if (KEY_MANAGER->isOnceKeyDown(VK_RBUTTON))
		{
			_wormManager->toggleWeapons();
		}
	}
	break;
	}

	_uiManager->setTimer(ceil(_turnTime)); // UI Ÿ�̸� ����
	_uiManager->setTimerVisible(true);
	_uiManager->setWind(_turnWind); // UI �ٶ� ����
	_uiManager->setWindVisible(true);
	_uiManager->setTeamHpVisible(true); // UI �� ü�¹� ����
	_uiManager->setTeamHp(_wormManager->getTeamMaxHp(), _wormManager->getTeamHp());
}

void stageManager::render()
{
	int width = IMAGE_MANAGER->findImage("STAGE")->getWidth();
	int height = IMAGE_MANAGER->findImage("STAGE")->getHeight();

	// ��� �׸���
	RECT bgRect = { 0, 0, width, height};
	CAMERA_MANAGER->loopRender(getMemDC(), IMAGE_MANAGER->findImage("BACKGROUND"), bgRect, _offsetBG, 0);

	// �� �׸���
	CAMERA_MANAGER->render(getMemDC(), _stageShadowDC, 0, 0, width, height, true, RGB(255, 0, 255));
	CAMERA_MANAGER->render(getMemDC(), _stageBackDC, 0, 0, width, height, true, RGB(255, 0, 255));
	CAMERA_MANAGER->render(getMemDC(), _stageDC, 0, 0, width, height, true, RGB(255, 0, 255));
	
	// �ٴ� �׸���
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

void stageManager::setObjectManager(objectManager* objectManager)
{
	_objectManager = objectManager;
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
		_turnWind = RND->getFromFloatTo(-1, 1); // ���� �� ���
		_turnWind = round(_turnWind * 10) / 10; // �Ҽ� ù°�ڸ�������
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

void stageManager::pixelBomb(float x, float y, float damage, float width) // �ȼ� ���Ľ�Ű��
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
	makeWormsMines();
	setNextTurnIndex();
	setTurnWind();
	_wormManager->setWeaponsUI(_turnIndex); // ���� UI
	_uiManager->setWeaponVisible(false);
}

void stageManager::makeWorld() // �� ����
{
	// �� �̹���
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

void stageManager::makeWormsMines() // �� ������ ���� �����
{
	int wormSize = _playerNum * _wormsPerPlayer; // ������ �ϴ� ���� ��
	HDC stageDC = IMAGE_MANAGER->findImage("STAGE")->getMemDC();
	int stageWidth = IMAGE_MANAGER->findImage("STAGE")->getWidth();
	int stageHeight = IMAGE_MANAGER->findImage("STAGE")->getHeight();
	vector<pair<int, int>> alreadyExist; // �� ��� ����߷� ������Ű�� ����
	vector<bool> alreadyNames (_wormNames.size(), false);

	// �������� ��ġ�� ��� ���ʷ� �����ϱ� 
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
			int randomX; // ��ȯ�� X ��ġ ���� ����
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
			
			for (int y = 0; y < stageHeight - 190; y++) // ��ȯ�� Y ��ġ ���� ����
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
		_wormManager->addWorms(i, name, posX, posY); // ��ȯ
	}


	int mineSize = 5; // ������ �ϴ� ���� ��

	// �������� ��ġ�� ��� ���ʷ� �����ϱ� 
	for (int i = 0; i < mineSize; i++)
	{
		bool isAvailPosition = false;
		int posX;
		int posY;

		while (!isAvailPosition)
		{
			bool isValidX = false;
			int randomX; // ��ȯ�� X ��ġ ���� ����
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

			for (int y = 0; y < stageHeight - 190; y++) // ��ȯ�� Y ��ġ ���� ����
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
		alreadyExist.push_back(make_pair(posX - 50, posX + 50));

		mineObject* mine = new mineObject;
		mine->init(this, _wormManager, _objectManager, posX, posY - 4, 0, 0);
		_objectManager->pushObject(mine);
	}
}

// ���� �� �ִ� ���� ��� �Լ�, ������(���� �ɸ�)
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
				SetPixel(groundDC, x, y, RGB(0, 255, 0)); // ���� �� �ִ� ���� �ʷϻ����� ������ �׸�
			}
		}
	}

	return groundDC;
}

