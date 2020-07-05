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

	for (int y = bottom - 15; y <= bottom + 15; y++)
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

bool worm::move()
{
	float deltaTime = TIME_MANAGER->getElapsedTime();
	float deltaMove = deltaTime * _speed * ((_dir == DIRECTION::LEFT)? -1 : 1);
	float newX = _x;
	float newBottom = _rc.bottom;

	switch (_dir)
	{
	case DIRECTION::LEFT:
	{
		if (ceil(_x) == ceil(_x + deltaMove)) // 움직임 변화 없는 경우
		{
			_x += deltaMove;
			return true;
		}
		for (int x = ceil(_x) - 1 ; x >= _x + deltaMove; x--) // 움직임 변화 있는 경우
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
			if (x == ceil(x + deltaMove)) // 이동 가능 범위 끝까지 이동한 경우
			{
				newX = x + deltaMove;
			}
		}
	}
	break;
	case DIRECTION::RIGHT:
	{
		if (floor(_x) == floor(_x + deltaMove)) // 움직임 변화 없는 경우
		{
			_x += deltaMove;
			return true;
		}
		for (int x = floor(_x) + 1; x <= _x + deltaMove; x++) // 움직임 체크
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
			if (x == floor(x + deltaMove)) // 이동 가능 범위 끝까지 이동한 경우
			{
				newX = x + deltaMove;
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

bool worm::gravityMove()
{
	_gravity += 9.8 * TIME_MANAGER->getElapsedTime(); // 중력 더해주기
	int newBottom = _rc.bottom;
	if (floor(_rc.bottom) == floor(_rc.bottom + _gravity))
	{
		_rc.bottom += _gravity;
		return false;
	}

	for (int y = floor(_rc.bottom) + 1; y < _rc.bottom + _gravity; y++)
	{
		bool isGround = checkPixelAvail(_x, y); //  떨어진 후 밟을 픽셀이 있는지 검사

		if (isGround) // 발판이 있는 경우
		{
			_y = y - (_height / 2);
			_rc = RectMakeCenter(_x, _y, _width, _height);
			return true;
		}
		else // 발판이 없는 경우
		{
			newBottom = y;
		}
		
		if (y == floor(_rc.bottom + _gravity)) // 이동 가능 범위 끝까지 이동한 경우
		{
			newBottom = _rc.bottom + _gravity;
		}
	}

	_y = newBottom - (_height / 2);
	_rc = RectMakeCenter(_x, _y, _width, _height);
	return false;
}

bool worm::isTurn()
{
	return (_wormManager->getCurrentTurnIndex() == _index);
}
