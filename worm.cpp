#include "stdafx.h"
#include "worm.h"
#include "allState.h"
#include "wormManager.h"
#include <cmath>

bool worm::checkMoveAvail(int x, int y)
{
	for (int checkY = y - _height; checkY < y; checkY++) // ���� �����ִ��� üũ
	{
		COLORREF checkRGB = _wormManager->getPixel(x, checkY);

		int checkR = GetRValue(checkRGB);
		int checkG = GetGValue(checkRGB);
		int checkB = GetBValue(checkRGB);

		if (!(checkR == 255 && checkG == 0 && checkB == 255)) // ����Ÿ�� �ƴ϶�� ���� �����ִٴ� ��
		{
			return false; 
		}
	}

	return true;
}

int worm::checkGroundAvail(int x, int y)
{
	for (int bot = y - _offsetClimb; bot <= y + _offsetClimb; bot++) // ���������� ���� �ִ��� üũ
	{
		if (checkPixelAvail(x, bot))
		{
			return bot;
		}
	}

	return -1; // ���� ������ ����
}

bool worm::checkPixelAvail(int x, int y)
{
	COLORREF rgb = _wormManager->getPixel(x, y);

	int r = GetRValue(rgb);
	int g = GetGValue(rgb);
	int b = GetBValue(rgb);

	if (!(r == 255 && g == 0 && b == 255)) // ���� �� ����
	{
		return true;
	}

	return false;
}

HRESULT worm::init(wormManager* wormManager, int index, float x, float y) // x ��ǥ�� �ٴ� ����
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

// �߽����� �������� ���� �ִ� ���� ���� ���� ��ȭ�� �ִ��� �ľ���
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

	if (yPos != -1) // �ٶ󺸴� ���⿡ �ȼ� �ִ� ���
	{
		if (yPos <= bottom - _offsetSlope) // �ٶ󺸴� �ȼ��� �� ���� ��ġ�� �ִ� ���
		{
			_slope = SLOPE::UP;
		}
		else if (yPos >= bottom + _offsetSlope) // �ٶ󺸴� �ȼ��� �� ���� ��ġ�� �ִ� ���
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
		if (ceil(_x) == ceil(_x + deltaMove)) // ������ ��ȭ ���� ���
		{
			_x += deltaMove;
			return true;
		}
		for (int x = ceil(_x) - 1 ; x >= _x + deltaMove; x--) // ������ ��ȭ �ִ� ���
		{
			bool isMoveAvail = false;
			int newGround = checkGroundAvail(x, newBottom); //  �̵� �� ���� ���� �ִ��� �˻�

			if (newGround == -1) // ���� ���� ����
			{
				isMoveAvail = checkMoveAvail(x, newBottom); // �̵��� �������� �˻�
			}
			else // ���� ���� ����
			{
				isMoveAvail = checkMoveAvail(x, newGround); // �̵��� �������� �˻�
			}

			if (isMoveAvail) // �̵��� ������ ���
			{
				if (newGround != -1) // ������ �ִ� ���
				{
					newX = x;
					newBottom = newGround;
				}
				else // ������ ���� ��� -> �������� �� (FALLEN STATE)
				{
					_x = x;
					_y = newBottom - (_height / 2);
					_rc = RectMakeCenter(_x, _y, _width, _height);
					return false;
				}
			}
			else // �̵��� �Ұ����� ���
			{
				newX = x + 1;
				break;
			}
			if (x == ceil(x + deltaMove)) // �̵� ���� ���� ������ �̵��� ���
			{
				newX = x + deltaMove;
			}
		}
	}
	break;
	case DIRECTION::RIGHT:
	{
		if (floor(_x) == floor(_x + deltaMove)) // ������ ��ȭ ���� ���
		{
			_x += deltaMove;
			return true;
		}
		for (int x = floor(_x) + 1; x <= _x + deltaMove; x++) // ������ üũ
		{
			bool isMoveAvail = false;
			int newGround = checkGroundAvail(x, newBottom); //  �̵� �� ���� ���� �ִ��� �˻�

			if (newGround == -1) // ���� ���� ����
			{
				isMoveAvail = checkMoveAvail(x, newBottom); // �̵��� �������� �˻�
			}
			else // ���� ���� ����
			{
				isMoveAvail = checkMoveAvail(x, newGround); // �̵��� �������� �˻�
			}
			
			if (isMoveAvail) // �̵��� ������ ���
			{
				if (newGround != -1) // ������ �ִ� ���
				{
					newX = x;
					newBottom = newGround;
				}
				else // ������ ���� ��� -> �������� �� (FALLEN STATE)
				{
					_x = x;
					_y = newBottom - (_height / 2);
					_rc = RectMakeCenter(_x, _y, _width, _height);
					return false;
				}
			}
			else // �̵��� �Ұ����� ���
			{
				newX = x - 1;
				break;
			}
			if (x == floor(x + deltaMove)) // �̵� ���� ���� ������ �̵��� ���
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
	_gravity += 9.8 * TIME_MANAGER->getElapsedTime(); // �߷� �����ֱ�
	int newBottom = _rc.bottom;
	if (floor(_rc.bottom) == floor(_rc.bottom + _gravity))
	{
		_rc.bottom += _gravity;
		return false;
	}

	for (int y = floor(_rc.bottom) + 1; y < _rc.bottom + _gravity; y++)
	{
		bool isGround = checkPixelAvail(_x, y); //  ������ �� ���� �ȼ��� �ִ��� �˻�

		if (isGround) // ������ �ִ� ���
		{
			_y = y - (_height / 2);
			_rc = RectMakeCenter(_x, _y, _width, _height);
			return true;
		}
		else // ������ ���� ���
		{
			newBottom = y;
		}
		
		if (y == floor(_rc.bottom + _gravity)) // �̵� ���� ���� ������ �̵��� ���
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
