#include "stdafx.h"
#include "worm.h"
#include "allState.h"
#include "stageManager.h"
#include "wormManager.h"

HRESULT worm::init(stageManager* stageManager, int index, float x, float y) // x ��ǥ�� �ٴ� ����
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

// �߽����� �������� ����, �������� ���� ���� ��ȭ�� �ִ��� �ľ���
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

	if (yPos.size() == 1) // �ȼ� ��ĭ���� ���
	{
		if (tempX == _x - 1) // ���� �ȼ��� ���� ���
		{
			if (yPos[0] < bottom - offset) // ���� �ȼ��� �� ���� ��ġ�� �ִ� ���
			{
				_slope = (_dir == DIRECTION::LEFT) ? SLOPE::UP : SLOPE::DOWN;
			}
			else if (yPos[0] > bottom + offset) // ���� �ȼ��� �� ���� ��ġ�� �ִ� ���
			{
				_slope = (_dir == DIRECTION::LEFT) ? SLOPE::DOWN : SLOPE::UP;
			}
		}
		else // ������ �ȼ��� ���� ���
		{
			if (yPos[0] < bottom - offset) // ������ �ȼ��� �� ���� ��ġ�� �ִ� ���
			{
				_slope = (_dir == DIRECTION::LEFT) ? SLOPE::DOWN : SLOPE::UP;
			}
			else if (yPos[0] > bottom + offset) // ������ �ȼ��� �� ���� ��ġ�� �ִ� ���
			{
				_slope = (_dir == DIRECTION::LEFT) ? SLOPE::UP : SLOPE::DOWN;
			}
		}
	}
	else if (yPos.size() == 2)// �ȼ� ��ĭ�� ���
	{
		if (yPos[0] < bottom - offset && yPos[1] > bottom + offset) //  \ �밢���� ���
		{
			_slope = (_dir == DIRECTION::LEFT) ? SLOPE::UP : SLOPE::DOWN;
		}
		else if (yPos[1] < bottom - offset && yPos[0] > bottom + offset) // / �밢���� ���
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
