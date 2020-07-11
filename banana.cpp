#include "stdafx.h"
#include "banana.h"
#include "stageManager.h"
#include "wormManager.h"


bool banana::checkMoveAvail(int x, int bot)
{
	for (int checkY = bot - _height; checkY < bot; checkY++) // 앞이 막혀있는지 체크
	{
		COLORREF checkRGB = _wormManager->getPixel(x, checkY);

		int checkR = GetRValue(checkRGB);
		int checkG = GetGValue(checkRGB);
		int checkB = GetBValue(checkRGB);

		if (!(checkR == 255 && checkG == 0 && checkB == 255)) // 마젠타가 아니라면 앞이 막혀있다는 것
		{
			return false;
		}
	}

	return true;
}

bool banana::checkPixelAvail(int x, int bot)
{
	COLORREF rgb = _wormManager->getPixel(x, bot);

	int r = GetRValue(rgb);
	int g = GetGValue(rgb);
	int b = GetBValue(rgb);

	if (!(r == 255 && g == 0 && b == 255)) // 밟을 수 있음
	{
		return true;
	}

	return false;
}

void banana::makeBanana(float angle)
{
	banana* _projectile = new banana;

	float initGravity = 0;
	float initAngle = angle;
	float initPower = 6;

	float initX = _x;
	float initY = _y;
	float widthX = _width * 0.5;
	float widthY = _width * 0.5;
	float initBombWidth = _bombWidth * 0.6;
	float initDamage = _damage * 0.6;

	_projectile->init(_stageManager, _wormManager, initX, initY, widthX, widthY, initAngle, initPower, initDamage, initBombWidth, true, true, true, true, true);
	_projectile->setImage(IMAGE_MANAGER->findImage("PROJECTILE_BANANA_SMALL"));
	_projectile->setAnimation();
	_projectile->setLevel(1);

	_wormManager->shoot(_projectile);
}

bool banana::gravityMove(float xPower)
{
	_gravity += 0.08; // 중력 업데이트
	float deltaX = cosf(_angle) * _power + xPower; // X축 이동값
	float deltaY = -sinf(_angle) * _power + _gravity; // Y축 이동값

	if (deltaY == 0)
	{
		deltaY = 0.0001;
	}
	float ratio = abs(deltaX / deltaY); // y축 변화에 대한 x축 변화의 비율

	float newX = _x;
	float newBottom = _y + (_height / 2);
	float yMove = newBottom + deltaY;

	if (deltaY >= 0) // deltaY가 양수일 때 (아래로 떨어질 때 or 떨어지지 않을 때)
	{
		if (abs(deltaY) >= 1) // ratio 비율대로 계산할 때
		{
			for (int bot = floor(newBottom); bot <= floor(yMove); bot++)
			{
				bool isMoveAvailX = false; // 움직일 수 있는지 검사
				bool isPixelAvail = false; // 앉을 땅이 있는지 검사
				bool isMoveAvailY = true;
				float firstDeltaX = (floor(newBottom) + 1 - newBottom);
				float xMove = newX;

				if (deltaX >= 0) // deltaX가 양수일 때 (오른쪽으로 이동할 때, 이동하지 않을 때)
				{
					if (bot == floor(newBottom) + 1)
					{
						xMove += ratio * firstDeltaX;
					}
					else if (bot == floor(yMove))
					{
						xMove += ratio * (yMove - floor(yMove));
					}
					else
					{
						xMove += ratio; // y 이동값에 대한 x 이동값
					}

					for (int x = floor(newX); x <= floor(xMove); x++)
					{
						//if (bot == floor(newBottom) && x == floor(newX)) continue;

						isMoveAvailX = checkMoveAvail(x, bot); // 이동 가능한지 체크

						if (!isMoveAvailX) // 이동이 불가능하다면
						{
							if (x == floor(newX))
							{
								isMoveAvailY = false;
								newX = x;
							}
							else
							{
								newX = x - 1; // 확인되었던 x로 돌아가기
							}
							_angle = PI - _angle;
							_power *= _powerChange;
							break;
						}

						isPixelAvail = checkPixelAvail(x, bot); // 밟을 땅이 있는지 체크

						if (isPixelAvail) // 밟을 땅이 있다면
						{
							newX = x;
							break;
						}

						if (x == floor(xMove)) // 이동할 수 있는 만큼 다 이동한 경우
						{
							newX = xMove;
						}
					}
				}
				else // deltaX가 음수일 때 (왼쪽으로 이동할 때)
				{
					if (bot == floor(newBottom))
					{
						xMove -= ratio * firstDeltaX;
					}
					else if (bot == floor(yMove))
					{
						xMove -= ratio * (yMove - floor(yMove));
					}
					else
					{
						xMove -= ratio; // y 이동값에 대한 x 이동값
					}
					for (int x = floor(newX); x >= floor(xMove); x--)
					{
						//if (bot == floor(newBottom) && x == floor(newX)) continue;
						isMoveAvailX = checkMoveAvail(x, bot);

						if (!isMoveAvailX) // 이동이 불가능하다면
						{
							if (x == floor(newX))
							{
								isMoveAvailY = false;
								newX = x;
							}
							else
							{
								newX = x + 1; // 확인되었던 x로 돌아가기
							}
							_angle = PI - _angle;
							_power *= _powerChange;
							break;
						}

						isPixelAvail = checkPixelAvail(x, bot); // 밟을 땅이 있는지 체크

						if (isPixelAvail) // 밟을 땅이 있다면
						{
							newX = x;
							break;
						}

						if (x == floor(xMove)) // 이동할 수 있는 만큼 다 이동한 경우
						{
							newX = xMove;
						}
					}
				}

				if (!isMoveAvailY)
				{
					newBottom = bot + 1;
					break;
				}
				if (isPixelAvail) // x축 이동 계산 중 착지한 경우
				{
					_x = newX;
					_y = bot - (_height / 2);
					_power *= _powerChange;
					return true;
				}

				if (bot == floor(yMove)) // 이동할 수 있는 만큼 다 이동한 경우
				{
					newBottom = yMove;
				}
			}
		}
		else // x축만 계산이 필요할 때
		{
			float xMove = newX + deltaX;
			int bot = floor(newBottom);
			bool isMoveAvail = false;
			bool isPixelAvail = false;

			if (deltaX >= 0) // deltaX가 양수일 때 (오른쪽으로 이동할 때, 이동하지 않을 때)
			{
				for (int x = floor(newX); x <= floor(xMove); x++)
				{
					isMoveAvail = checkMoveAvail(x, bot); // 이동 가능한지 체크

					if (!isMoveAvail) // 이동이 불가능하다면
					{
						if (x == floor(newX))
						{
							newBottom -= 1;
							newX = x;
						}
						else
						{
							newX = x - 1; // 확인되었던 x로 돌아가기
						}
						_angle = PI - _angle;
						_power *= _powerChange;
						break;
					}

					isPixelAvail = checkPixelAvail(x, bot); // 밟을 땅이 있는지 체크

					if (isPixelAvail) // 밟을 땅이 있다면
					{
						newX = x;
						break;
					}

					if (x == floor(xMove)) // 이동할 수 있는 만큼 다 이동한 경우
					{
						newX = xMove;
					}
				}
			}
			else // deltaX가 음수일 때 (왼쪽으로 이동할 때)
			{
				for (int x = floor(newX); x >= floor(xMove); x--)
				{
					isMoveAvail = checkMoveAvail(x, bot);

					if (!isMoveAvail) // 이동이 불가능하다면
					{
						if (x == floor(newX))
						{
							newBottom -= 1;
							newX = x;
						}
						else
						{
							newX = x + 1; // 확인되었던 x로 돌아가기
						}
						_angle = PI - _angle;
						_power *= _powerChange;
						break;
					}

					isPixelAvail = checkPixelAvail(x, bot); // 밟을 땅이 있는지 체크

					if (isPixelAvail) // 밟을 땅이 있다면
					{
						newX = x;
						break;
					}

					if (x == floor(xMove)) // 이동할 수 있는 만큼 다 이동한 경우
					{
						newX = xMove;
					}
				}
			}

			if (isPixelAvail) // x축 이동 계산 중 착지한 경우
			{
				_x = newX;
				_y = newBottom - (_height / 2);
				_power *= _powerChange;
				return true;
			}

			if (isMoveAvail) // 이동할 수 있는 만큼 다 이동한 경우
			{
				newBottom += deltaY;
			}
		}
	}
	else // deltaY가 음수일 때 (위로 이동할 때), 위로 이동할 때는 착지 체크 안함
	{
		if (abs(deltaY) >= 1) // y축 이동이 있어 ratio 비율대로 계산할 때
		{
			for (int bot = floor(newBottom); bot >= floor(yMove); bot--)
			{
				bool isMoveAvailY = true; // Y 축 이동이 가능한지 검사하는 불 변수
				bool isMoveAvailX = false; // X 축 이동이 가능한지 검사하는 불 변수

				float firstDeltaX = (newBottom - floor(newBottom));
				float xMove = newX;

				if (deltaX >= 0) // deltaX가 양수일 때 (오른쪽으로 이동할 때, 이동하지 않을 때)
				{
					if (bot == floor(newBottom) - 1)
					{
						xMove += ratio * firstDeltaX;
					}
					else if (bot != floor(yMove))
					{
						xMove += ratio * abs(bot + 1 - yMove);
					}
					else
					{
						xMove += ratio; // y 이동값에 대한 x 이동값
					}

					for (int x = floor(newX); x <= floor(xMove); x++)
					{
						isMoveAvailX = checkMoveAvail(x, bot); // x축 이동이 가능한지 검사

						if (!isMoveAvailX) // x축 이동이 불가능할 때
						{
							if (x == floor(newX))
							{
								newX = x;
								isMoveAvailY = false; // y 축 이동 더 할 수 없음
							}
							else
							{
								newX = x - 1;
							}
							_angle = PI - _angle;
							_power *= _powerChange;
							break;
						}

						if (x == floor(xMove)) // 이동할 수 있는 만큼 다 이동한 경우
						{
							newX = xMove;
						}
					}
				}
				else // deltaX가 음수일 때 (왼쪽으로 이동할 때)
				{
					if (bot == floor(newBottom) - 1)
					{
						xMove -= ratio * firstDeltaX;
					}
					else if (bot == floor(yMove))
					{
						xMove -= ratio * abs(bot + 1 - yMove);
					}
					else
					{
						xMove -= ratio; // y 이동값에 대한 x 이동값
					}

					for (int x = floor(newX); x >= floor(xMove); x--)
					{
						isMoveAvailX = checkMoveAvail(x, bot);

						if (!isMoveAvailX) // X 축 이동이 안될 때
						{
							if (x == floor(newX))
							{
								isMoveAvailY = false; // Y 축 이동 더 할 수 없음
								newX = x;
							}
							else
							{
								newX = x + 1;
							}
							_angle = PI - _angle;
							_power *= _powerChange;
							break;
						}

						if (x == floor(xMove)) // 이동할 수 있는 만큼 다 이동한 경우
						{
							newX = xMove;
						}
					}
				}

				if (!isMoveAvailY) // y축으로 더 이상 이동할 수 없는 경우
				{
					newBottom = bot - 1;
					break;
				}

				if (bot == floor(yMove)) // 이동할 수 있는 만큼 다 이동한 경우
				{
					newBottom = yMove;
				}
			}
		}
		else // x축만 계산이 필요할 때 
		{
			float xMove = newX + deltaX;
			int bot = floor(newBottom);
			bool isMoveAvailX = true;

			if (deltaX >= 0) // deltaX가 양수일 때 (오른쪽으로 이동할 때, 이동하지 않을 때)
			{
				for (int x = floor(newX); x <= floor(xMove); x++)
				{
					isMoveAvailX = checkMoveAvail(x, bot); // x축 이동이 가능한지 검사

					if (!isMoveAvailX) // x축 이동이 불가능할 때
					{
						if (floor(newX) == x)
						{
							newX = x;
						}
						else
						{
							newX = x - 1;
						}
						_angle = PI - _angle;
						_power *= _powerChange;
						break;
					}
					if (x == floor(xMove)) // 이동할 수 있는 만큼 다 이동한 경우
					{
						newX = xMove;
					}
				}
			}
			else // deltaX가 음수일 때 (왼쪽으로 이동할 때)
			{
				for (int x = floor(newX); x >= floor(xMove); x--)
				{
					isMoveAvailX = checkMoveAvail(x, bot);

					if (!isMoveAvailX) // X 축 이동이 안될 때
					{
						if (x != floor(newX))
						{
							newX = x + 1;
						}
						else
						{
							newX = x;
						}
						_angle = PI - _angle;
						_power *= _powerChange;
						break;
					}

					if (x == floor(xMove)) // 이동할 수 있는 만큼 다 이동한 경우
					{
						newX = xMove;
					}
				}
			}
			if (isMoveAvailX) // x의 이동이 전부 가능할 때
			{
				newBottom += deltaY;
			}
		}
	}

	_x = newX;
	_y = newBottom - (_height / 2);
	return false;
}

void banana::update()
{
	if (!_isFinish)
	{
		EFFECT_MANAGER->play("EFFECT_HEXHAUST", _x, _y, 20 * _level , 20 * _level);
		_isGround = gravityMove(_stageManager->getWind() * 0.2);

		if (_isGround) // 폭파시키기
		{
			if (++_bombCount >= _maxBombCount)
			{
				EFFECT_MANAGER->play("EFFECT_CIRCLE", _x, _y, _bombWidth, _bombWidth);
				EFFECT_MANAGER->play("EFFECT_ELIPSE", _x, _y, _bombWidth + 30, _bombWidth + 30);
				EFFECT_MANAGER->play("EFFECT_EX_POW", _x, _y - 50, 50, 50);

				_stageManager->pixelBomb(_x, _y, _damage, _bombWidth); // 픽셀 폭파시키기
				_wormManager->wormBomb(_x, _y, _damage, _bombWidth); // 맞은 웜즈 날라가게 하기

				_isFinish = true;
				if (_level > 1)
				{
					float angle = PI * 0.85;
					for (int i = 0; i < 5; i++)
					{
						makeBanana(angle);
						angle -= PI * 0.15;
					}
				}
			}
			else
			{
				_gravity = 0;
			}
		} 
		else
		{
			_ani->frameUpdate(TIME_MANAGER->getElapsedTime());
		}
	}
}

void banana::render()
{
	CAMERA_MANAGER->aniRender(getMemDC(), _img, _x - (_img->getFrameWidth() / 2), _y - (_img->getFrameHeight() / 2), _ani, false);
}
