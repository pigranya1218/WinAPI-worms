#include "stdafx.h"
#include "worm.h"
#include "allState.h"
#include "stageManager.h"
#include "wormManager.h"

HRESULT worm::init(stageManager* stageManager, int index, float x, float y) // x 좌표와 바닥 높이
{
	_stageManager = stageManager;

	_index = index;
	_x = x;
	_y = y - (_height / 2);
	_rc = RectMakeCenter(_x, _y, _width, _height);
	
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
		upperRGB = _stageManager->getPixel(x, bottom - 11);

		for (int y = bottom - 5; y <= bottom + 5; y++)
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

	int offset = 0;

	if (yPos.size() == 1) // 픽셀 두칸뿐인 경우
	{
		if (tempX == _x - 1) // 왼쪽 픽셀이 남은 경우
		{
			if (yPos[0] < bottom - offset) // 왼쪽 픽셀이 더 높은 위치에 있는 경우
			{
				_slope = (_dir == DIRECTION::LEFT) ? SLOPE::UP : SLOPE::DOWN;
			}
			else if (yPos[0] > bottom + offset) // 왼쪽 픽셀이 더 낮은 위치에 있는 경우
			{
				_slope = (_dir == DIRECTION::LEFT) ? SLOPE::DOWN : SLOPE::UP;
			}
		}
		else // 오른쪽 픽셀이 남은 경우
		{
			if (yPos[0] < bottom - offset) // 오른쪽 픽셀이 더 높은 위치에 있는 경우
			{
				_slope = (_dir == DIRECTION::LEFT) ? SLOPE::DOWN : SLOPE::UP;
			}
			else if (yPos[0] > bottom + offset) // 오른쪽 픽셀이 더 낮은 위치에 있는 경우
			{
				_slope = (_dir == DIRECTION::LEFT) ? SLOPE::UP : SLOPE::DOWN;
			}
		}
	}
	else if (yPos.size() == 2)// 픽셀 세칸인 경우
	{
		if (yPos[0] < bottom - offset && yPos[1] > bottom + offset) //  \ 대각선인 경우
		{
			_slope = (_dir == DIRECTION::LEFT) ? SLOPE::UP : SLOPE::DOWN;
		}
		else if (yPos[1] < bottom - offset && yPos[0] > bottom + offset) // / 대각선인 경우
		{
			_slope = (_dir == DIRECTION::LEFT) ? SLOPE::DOWN : SLOPE::UP;
		}
	}
}

void worm::move()
{
	switch (_dir)
	{
	case DIRECTION::LEFT:
	{
		_x -= _speed * TIME_MANAGER->getElapsedTime();
	}
	break;
	case DIRECTION::RIGHT:
	{
		_x += _speed * TIME_MANAGER->getElapsedTime();
	}
	break;
	}
	_rc = RectMakeCenter(_x, _y, _width, _height);
	updateSlope();
}

bool worm::isTurn()
{
	return (_stageManager->getCurrentTurnIndex() == _index);
}
