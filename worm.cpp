#include "stdafx.h"
#include "worm.h"
#include "allState.h"
#include "stageManager.h"
#include "wormManager.h"
#include <cmath>

HRESULT worm::init(stageManager* stageManager, int index, float x, float y) // x 좌표와 바닥 높이
{
	_stageManager = stageManager;

	_index = index;
	_x = x;
	_y = y - (_height / 2);
	_rc = RectMakeCenter(_x, _y, _width, _height);
	
	_offsetClimb = 4;

	int random = RND->getInt(2);
	_dir = (random == 0) ? DIRECTION::RIGHT : DIRECTION::LEFT;
	updateSlope();

	_state = new idleState();
	_state->enter(*this);
	
	_speed = 15;
	

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
}

// 중심점을 기준으로 왼쪽, 오른쪽을 봐서 각도 변화가 있는지 파악함
void worm::updateSlope()
{
	vector<int> yPos;
	COLORREF upperRGB;
	COLORREF RGB;
	int bottom = _rc.bottom;
	int tempX;
	for (int x = _x - 1; x <= _x + 1; x += 2)
	{
		upperRGB = _stageManager->getPixel(x, bottom - 5);

		for (int y = bottom - 4; y <= bottom + 4; y++)
		{
			COLORREF rgb = _stageManager->getPixel(x, y);

			int r = GetRValue(rgb);
			int g = GetGValue(rgb);
			int b = GetBValue(rgb);

			int upperR = GetRValue(upperRGB);
			int upperG = GetGValue(upperRGB);
			int upperB = GetBValue(upperRGB);

			if ((upperR == 255 && upperG == 0 && upperB == 255) &&
				!(r == 255 && g == 0 && b == 255))
			{
				yPos.push_back(y);
				tempX = x;
				break;
			}

			upperRGB = rgb;

		}
	}

	_slope = SLOPE::NORMAL;

	if (yPos.size() == 1) // 픽셀 두칸뿐인 경우
	{
		if (tempX == _x - 1) // 왼쪽 픽셀이 남은 경우
		{
			if (yPos[0] <= bottom - _offsetClimb) // 왼쪽 픽셀이 더 높은 위치에 있는 경우
			{
				_slope = (_dir == DIRECTION::LEFT) ? SLOPE::UP : SLOPE::DOWN;
			}
			else if (yPos[0] >= bottom + _offsetClimb) // 왼쪽 픽셀이 더 낮은 위치에 있는 경우
			{
				_slope = (_dir == DIRECTION::LEFT) ? SLOPE::DOWN : SLOPE::UP;
			}
		}
		else // 오른쪽 픽셀이 남은 경우
		{
			if (yPos[0] <= bottom - _offsetClimb) // 오른쪽 픽셀이 더 높은 위치에 있는 경우
			{
				_slope = (_dir == DIRECTION::LEFT) ? SLOPE::DOWN : SLOPE::UP;
			}
			else if (yPos[0] >= bottom + _offsetClimb) // 오른쪽 픽셀이 더 낮은 위치에 있는 경우
			{
				_slope = (_dir == DIRECTION::LEFT) ? SLOPE::UP : SLOPE::DOWN;
			}
		}
	}
	else if (yPos.size() == 2)// 픽셀 세칸인 경우
	{
		if (yPos[0] <= bottom - _offsetClimb && yPos[1] >= bottom + _offsetClimb) //  \ 대각선인 경우
		{
			_slope = (_dir == DIRECTION::LEFT) ? SLOPE::UP : SLOPE::DOWN;
		}
		else if (yPos[1] <= bottom - _offsetClimb && yPos[0] >= bottom + _offsetClimb) // / 대각선인 경우
		{
			_slope = (_dir == DIRECTION::LEFT) ? SLOPE::DOWN : SLOPE::UP;
		}
	}
}

void worm::move()
{
	float deltaTime = TIME_MANAGER->getElapsedTime();
	float deltaMove = deltaTime * _speed;
	bool isStop = false;
	float newX = _x;
	float newBottom = _rc.bottom;

	if (static_cast<int>(_x) == static_cast<int>(deltaMove + _x)) // 움직임 변화 없는 경우
	{
		_x = _x + deltaMove;
		return;
	}

	switch (_dir)
	{
	case DIRECTION::LEFT:
	{
		for (int x = static_cast<int>(_x) - 1; x >= _x - deltaMove; x--)
		{
			COLORREF upperRGB = _stageManager->getPixel(x, newBottom - _offsetClimb - 1);
			bool isGroundExist = false;
			for (int bot = newBottom - _offsetClimb; bot <= newBottom + _offsetClimb; bot++) // 위에서부터 발판이 있는지 체크
			{
				COLORREF rgb = _stageManager->getPixel(x, bot);

				int r = GetRValue(rgb);
				int g = GetGValue(rgb);
				int b = GetBValue(rgb);

				int upperR = GetRValue(upperRGB);
				int upperG = GetGValue(upperRGB);
				int upperB = GetBValue(upperRGB);

				if (!(r == 255 && g == 0 && b == 255) &&
					(upperR == 255 && upperG == 0 && upperB == 255))
				{
					newBottom = bot;
					isGroundExist = true;
					newX = x;
					break;
				}

				upperRGB = rgb;
			}

			if (!isGroundExist) // 발판이 없다면 떨어진다
			{
				break;
			}

			if (static_cast<int>(x) == static_cast<int>(x + deltaMove)) // 이동 가능 범위 끝까지 이동한 경우
			{
				newX = x + deltaMove;
			}
		}
	}
	break;
	case DIRECTION::RIGHT:
	{
		for (int x = static_cast<int>(_x) + 1; x <= _x + deltaMove; x++)
		{
			COLORREF upperRGB = _stageManager->getPixel(x, newBottom - _offsetClimb - 1);
			bool isGroundExist = false;
			for (int bot = newBottom - _offsetClimb; bot <= newBottom + _offsetClimb; bot++) // 위에서부터 발판이 있는지 체크
			{
				COLORREF rgb = _stageManager->getPixel(x, bot);

				int r = GetRValue(rgb);
				int g = GetGValue(rgb);
				int b = GetBValue(rgb);

				int upperR = GetRValue(upperRGB);
				int upperG = GetGValue(upperRGB);
				int upperB = GetBValue(upperRGB);

				if (!(r == 255 && g == 0 && b == 255) &&
					(upperR == 255 && upperG == 0 && upperB == 255))
				{
					newBottom = bot;
					isGroundExist = true;
					newX = x;
					break;
				}

				upperRGB = rgb;
			}

			if (!isGroundExist) // 발판이 없다면 떨어진다
			{
				break;
			}

			if (x == static_cast<int>(x + deltaMove)) // 이동 가능 범위 끝까지 이동한 경우
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
}

bool worm::isTurn()
{
	return (_stageManager->getCurrentTurnIndex() == _index);
}
