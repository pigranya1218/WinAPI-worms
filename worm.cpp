#include "stdafx.h"
#include "worm.h"
#include "allState.h"
#include "wormManager.h"
#include <cmath>

bool worm::checkMoveAvail(int x, int y)
{
	for (int checkY = y - _height; checkY < y; checkY++) // 앞이 막혀있는지 체크
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

int worm::checkGroundAvail(int x, int y)
{
	for (int bot = y - _offsetClimb; bot <= y + _offsetClimb; bot++) // 위에서부터 땅이 있는지 체크
	{
		if (checkPixelAvail(x, bot))
		{
			return bot;
		}
	}

	return -1; // 밟을 발판이 없음
}

bool worm::checkPixelAvail(int x, int y)
{
	COLORREF rgb = _wormManager->getPixel(x, y);

	int r = GetRValue(rgb);
	int g = GetGValue(rgb);
	int b = GetBValue(rgb);

	if (!(r == 255 && g == 0 && b == 255)) // 밟을 수 있음
	{
		return true;
	}

	return false;
}

HRESULT worm::init(wormManager* wormManager, int index, float x, float y) // x 좌표와 바닥 높이
{
	_wormManager = wormManager;

	_index = index;
	_x = x;
	_y = y - (_height / 2);
	_rc = RectMakeCenter(_x, _y, _width, _height);
	
	int random = RND->getInt(2);
	_dir = (random == 0) ? DIRECTION::RIGHT : DIRECTION::LEFT;
	updateSlope();

	_state = new idleState();
	_state->enter(*this);

	return S_OK;
}

void worm::release()
{

}

void worm::update()
{
	state* state = _state->update(*this);
	if (state != nullptr)
	{
		_state->exit(*this);
		state->enter(*this);
		delete _state;
		_state = state;
	}
}

void worm::render()
{
	_state->render(*this);


	// DEBUG CODE
	/* 
	CAMERA_MANAGER->rectangle(getMemDC(), _rc);
	char _buffer[50];
	sprintf_s(_buffer, "SLOPE : %s", ((_slope == SLOPE::UP) ? "UP" : ((_slope == SLOPE::NORMAL) ? "NORMAL" : "DOWN")));
	TextOut(getMemDC(), 0, 0, _buffer, strlen(_buffer));
	sprintf_s(_buffer, "xPos : %f", _x);
	TextOut(getMemDC(), 0, 20, _buffer, strlen(_buffer));
	*/
}

// 중심점을 기준으로 보고 있는 방향 봐서 각도 변화가 있는지 파악함
void worm::updateSlope()
{
	int bottom = _rc.bottom;
	int xPos = (_dir == DIRECTION::LEFT)?(_x - 2):(_x + 2);
	int yPos = -1;

	for (int y = bottom - 6; y <= bottom + 6; y++)
	{
		COLORREF rgb = _wormManager->getPixel(xPos, y);

		int r = GetRValue(rgb);
		int g = GetGValue(rgb);
		int b = GetBValue(rgb);

		if (!(r == 255 && g == 0 && b == 255))
		{
			yPos = y;
			break;
		}
	}

	if (yPos != -1) // 바라보는 방향에 픽셀 있는 경우
	{
		if (yPos <= bottom - _offsetSlope) // 바라보는 픽셀이 더 높은 위치에 있는 경우
		{
			_slope = SLOPE::UP;
		}
		else if (yPos >= bottom + _offsetSlope) // 바라보는 픽셀이 더 낮은 위치에 있는 경우
		{
			_slope = SLOPE::DOWN;
		}
		else
		{
			_slope = SLOPE::NORMAL;
		}
	}
}

void worm::reverseSlope()
{
	if (_slope == SLOPE::UP)
	{
		_slope == SLOPE::DOWN;
	}
	else if (_slope == SLOPE::DOWN)
	{
		_slope == SLOPE::UP;
	}
}

bool worm::move() // true : 땅이 있음, false : 땅이 없어서 추락할 예정
{
	float deltaX = _speed * ((_dir == DIRECTION::LEFT)? -1 : 1);
	float newX = _x;
	float newBottom = _rc.bottom;

	switch (_dir)
	{
	case DIRECTION::LEFT:
	{
		if (ceil(_x) == ceil(_x + deltaX)) // 움직임 변화 없는 경우
		{
			_x += deltaX;
			return true;
		}
		for (int x = ceil(_x) - 1 ; x >= _x + deltaX; x--) // 움직임 변화 있는 경우
		{
			bool isMoveAvail = false;
			int newGround = checkGroundAvail(x, newBottom); //  이동 후 밟을 땅이 있는지 검사

			if (newGround == -1) // 밟을 땅이 없음
			{
				isMoveAvail = checkMoveAvail(x, newBottom); // 이동이 가능한지 검사
			}
			else // 밟을 땅이 있음
			{
				isMoveAvail = checkMoveAvail(x, newGround); // 이동이 가능한지 검사
			}

			if (isMoveAvail) // 이동이 가능한 경우
			{
				if (newGround != -1) // 발판이 있는 경우
				{
					newX = x;
					newBottom = newGround;
				}
				else // 발판이 없는 경우 -> 떨어져야 함 (FALLEN STATE)
				{
					_x = x;
					_y = newBottom - (_height / 2);
					_rc = RectMakeCenter(_x, _y, _width, _height);
					return false;
				}
			}
			else // 이동이 불가능한 경우
			{
				newX = x + 1;
				break;
			}
			if (x == ceil(x + deltaX)) // 이동 가능 범위 끝까지 이동한 경우
			{
				newX = x + deltaX;
			}
		}
	}
	break;
	case DIRECTION::RIGHT:
	{
		if (floor(_x) == floor(_x + deltaX)) // 움직임 변화 없는 경우
		{
			_x += deltaX;
			return true;
		}
		for (int x = floor(_x) + 1; x <= _x + deltaX; x++) // 움직임 체크
		{
			bool isMoveAvail = false;
			int newGround = checkGroundAvail(x, newBottom); //  이동 후 밟을 땅이 있는지 검사

			if (newGround == -1) // 밟을 땅이 없음
			{
				isMoveAvail = checkMoveAvail(x, newBottom); // 이동이 가능한지 검사
			}
			else // 밟을 땅이 있음
			{
				isMoveAvail = checkMoveAvail(x, newGround); // 이동이 가능한지 검사
			}
			
			if (isMoveAvail) // 이동이 가능한 경우
			{
				if (newGround != -1) // 발판이 있는 경우
				{
					newX = x;
					newBottom = newGround;
				}
				else // 발판이 없는 경우 -> 떨어져야 함 (FALLEN STATE)
				{
					_x = x;
					_y = newBottom - (_height / 2);
					_rc = RectMakeCenter(_x, _y, _width, _height);
					return false;
				}
			}
			else // 이동이 불가능한 경우
			{
				newX = x - 1;
				break;
			}
			if (x == floor(x + deltaX)) // 이동 가능 범위 끝까지 이동한 경우
			{
				newX = x + deltaX;
			}
		}
	}
	break;
	}

	_x = newX;
	_y = newBottom - (_height / 2);
	_rc = RectMakeCenter(_x, _y, _width, _height);
	updateSlope();
	return true;
}

bool worm::gravityMove(float startX, float startY) // false : 아직 떨어지는 중 , true : 착지함
{
	_gravity += 0.1; // 중력 업데이트
	float deltaX = cosf(_angle) * _power; // X축 이동값
	float deltaY = -sinf(_angle) * _power + _gravity; // Y축 이동값
	float deltaDistance = sqrt(pow(deltaX, 2) + pow(deltaY, 2)); // 현재 이동하는 거리
	float angle = atan2f(deltaX, deltaY); // 현재 이동하는 각도

	assert(deltaY != 0); // 혹시 모를 예외 체크
	float ratio = abs(deltaX / deltaY); // y축 변화에 대한 x축 변화의 비율
	
	float newX = _x;
	float newBottom = _rc.bottom;

	// 1. 이동이 가능한지 체크
	// 2. 이동이 가능할 때 발판이 있는지 체크
	
	float yMove = newBottom + deltaY;

	if (deltaY >= 0) // deltaY가 양수일 때 (아래로 떨어질 때 or 떨어지지 않을 때)
	{
		for (int bot = floor(newBottom); bot <= floor(yMove); bot++)
		{
			bool isMoveAvail = false; // 움직일 수 있는지 검사
			bool isPixelAvail = false; // 앉을 땅이 있는지 검사
			
			float xMove = newX;

			if (deltaX >= 0) // deltaX가 양수일 때 (오른쪽으로 이동할 때, 이동하지 않을 때)
			{
				if (bot != floor(newBottom))
				{
					xMove += ratio; // y 이동값에 대한 x 이동값
				}
				for (int x = floor(newX) ; x <= floor(xMove); x++)
				{
					if (bot == floor(newBottom) && x == floor(newX)) continue;

					isMoveAvail = checkMoveAvail(x, bot); // 이동 가능한지 체크

					if (!isMoveAvail)
					{
						newX = x - 1;
						break;
					}
					
					isPixelAvail = checkPixelAvail(x, bot); // 밟을 땅이 있는지 체크

					if (isPixelAvail) // 시작 지점 땅은 무시
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
				if (bot != floor(newBottom))
				{
					xMove -= ratio; // y 이동값에 대한 x 이동값
				}
				for (int x = ceil(newX); x >= ceil(xMove); x--)
				{
					if (bot == floor(newBottom) && x == ceil(newX)) continue;

					isMoveAvail = checkMoveAvail(x, bot);

					if (!isMoveAvail)
					{
						newX = x + 1;
						break;
					}

					isPixelAvail = checkPixelAvail(x, bot); // 밟을 땅이 있는지 체크

					if (isPixelAvail && !(x == startX && bot == startY)) // 시작 지점 땅은 무시
					{
						newX = x;
						break;
					}

					if (x == ceil(xMove)) // 이동할 수 있는 만큼 다 이동한 경우
					{
						newX = xMove;
					}
				}
			}

			if (isPixelAvail) // x축 이동 계산 중 착지한 경우
			{
				_x = newX;
				_y = bot - (_height / 2);
				_rc = RectMakeCenter(_x, _y, _width, _height);
				return true;
			}

			if (bot == floor(yMove)) // 이동할 수 있는 만큼 다 이동한 경우
			{
				newBottom = yMove;
				if (deltaX >= 0)
				{
					newX += ratio * abs(yMove - floor(yMove));
				}
				else
				{
					newX -= ratio * abs(yMove - floor(yMove));
				}

			}

		}
	}
	else // deltaY가 음수일 때 (위로 이동할 때)
	{
		for (int bot = ceil(newBottom); bot >= ceil(yMove); bot--)
		{
			bool isMoveAvailY = true; // Y 축 이동이 가능한지 검사하는 불 변수
			bool isMoveAvailX = false; // X 축 이동이 가능한지 검사하는 불 변수
			
			float xMove = newX;

			if (deltaX >= 0) // deltaX가 양수일 때 (오른쪽으로 이동할 때, 이동하지 않을 때)
			{
				if (bot != ceil(newBottom))
				{
					xMove += ratio; // y 이동값에 대한 x 이동값
				}

				for (int x = floor(newX); x <= floor(xMove); x++)
				{
					isMoveAvailX = checkMoveAvail(x, bot);

					if (!isMoveAvailX) // x축 이동이 불가능할 때
					{
						if (x != floor(newX)) // x 축 이동이 하나도 안될 때
						{
							newX = x - 1;
						}
						isMoveAvailY = false; // y 축 이동도 불가능하다
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
				if (bot != ceil(newBottom))
				{
					xMove -= ratio; // y 이동값에 대한 x 이동값
				}

				for (int x = ceil(newX); x >= ceil(xMove); x--)
				{
					isMoveAvailX = checkMoveAvail(x, bot);

					if (!isMoveAvailX) // X 축 이동이 안될 때
					{
						if (x != ceil(newX)) // X 축 이동이 하나도 안된 경우
						{
							newX = x + 1;
						}
						isMoveAvailY = false; // Y 축 이동도 불가능함
						break;
					}

					if (x == ceil(xMove)) // 이동할 수 있는 만큼 다 이동한 경우
					{
						newX = xMove;
					}
				}
			}

			if (!isMoveAvailY) // y축으로 더 이상 이동할 수 없는 경우
			{
				if (bot != ceil(newBottom))
				{
					newBottom = bot + 1;
				}
				break;
			}

			if (bot == ceil(yMove)) // 이동할 수 있는 만큼 다 이동한 경우
			{
				newBottom = yMove;
				if (deltaX >= 0)
				{
					newX += ratio * abs(yMove - ceil(yMove));
				}
				else
				{
					newX -= ratio * abs(yMove - ceil(yMove));
				}
			}
		}
	}

	_x = newX;
	_y = newBottom - (_height / 2);
	_rc = RectMakeCenter(_x, _y, _width, _height);
	return false;
}

bool worm::isTurn()
{
	return (_wormManager->getCurrentTurnIndex() == _index);
}
