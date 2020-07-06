#include "stdafx.h"
#include "worm.h"
#include "allState.h"
#include "wormManager.h"
#include <cmath>

bool worm::checkMoveAvail(int x, int bottom)
{
	for (int checkY = bottom - _height; checkY < bottom; checkY++) // ���� �����ִ��� üũ
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

int worm::checkGroundAvail(int x, int bottom)
{
	for (int bot = bottom - _offsetClimb; bot <= bottom + _offsetClimb; bot++) // ���������� ���� �ִ��� üũ
	{
		if (checkPixelAvail(x, bot))
		{
			return bot;
		}
	}

	return -1; // ���� ������ ����
}

bool worm::checkPixelAvail(int x, int bottom)
{
	COLORREF rgb = _wormManager->getPixel(x, bottom);

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
	// RECT rect = RectMakeCenter(_x, _rc.bottom, 2, 2);
	// CAMERA_MANAGER->rectangle(getMemDC(), _rc);

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

bool worm::move() // true : ���� ����, false : ���� ��� �߶��� ����
{
	float deltaX = _speed * ((_dir == DIRECTION::LEFT)? -1 : 1);
	float newX = _x;
	float newBottom = _rc.bottom;

	if (floor(_x) == floor(_x + deltaX)) // ������ ��ȭ ���� ���
	{
		_x += deltaX;
		return true;
	}

	int offset = (_dir == DIRECTION::LEFT) ? -1 : 1;

	for (int x = floor(_x) + offset; ((_dir == DIRECTION::LEFT) ? (x >= floor(_x + deltaX)) : (x <= floor(_x + deltaX))); x += offset) // ������ ��ȭ �ִ� ���
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
			newX = x - offset;
			break;
		}

		if (x == floor(_x + deltaX)) // �̵� ���� ���� ������ �̵��� ���
		{
			newX = _x + deltaX;
		}
	}
	
	_x = newX;
	_y = newBottom - (_height / 2);
	_rc = RectMakeCenter(_x, _y, _width, _height);
	updateSlope();
	return true;
}

bool worm::gravityMove(float xPower) // false : ���� �������� �� , true : ������
{
	_gravity += 0.1; // �߷� ������Ʈ
	float deltaX = cosf(_angle) * _power + xPower; // X�� �̵���
	float deltaY = -sinf(_angle) * _power + _gravity; // Y�� �̵���

	assert(deltaY != 0); // Ȥ�� �� ���� üũ
	float ratio = abs(deltaX / deltaY); // y�� ��ȭ�� ���� x�� ��ȭ�� ����
	
	float newX = _x;
	float newBottom = _rc.bottom;
	float yMove = newBottom + deltaY;

	if (deltaY >= 0) // deltaY�� ����� �� (�Ʒ��� ������ �� or �������� ���� ��)
	{
		if (floor(newBottom) != floor(yMove)) // ratio ������� ����� ��
		{
			for (int bot = floor(newBottom); bot <= floor(yMove); bot++)
			{
				bool isMoveAvailX = false; // ������ �� �ִ��� �˻�
				bool isPixelAvail = false; // ���� ���� �ִ��� �˻�
				bool isMoveAvailY = true;
				float xMove = newX;

				if (deltaX >= 0) // deltaX�� ����� �� (���������� �̵��� ��, �̵����� ���� ��)
				{
					if (bot != floor(newBottom))
					{
						xMove += ratio; // y �̵����� ���� x �̵���
					}
					for (int x = floor(newX); x <= floor(xMove); x++)
					{
						if (bot == floor(newBottom) && x == floor(newX)) continue;
						
						isMoveAvailX = checkMoveAvail(x, bot); // �̵� �������� üũ

						if (!isMoveAvailX) // �̵��� �Ұ����ϴٸ�
						{
							if (x == floor(newX))
							{
								isMoveAvailY = false;
							}
							else
							{
								newX = x - 1; // Ȯ�εǾ��� x�� ���ư���
							}
							_angle = PI - _angle;
							_power *= 0.8;
							break;
						}

						isPixelAvail = checkPixelAvail(x, bot); // ���� ���� �ִ��� üũ

						if (isPixelAvail) // ���� ���� �ִٸ�
						{
							newX = x;
							break;
						}

						if (x == floor(xMove)) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
						{
							newX = xMove;
						}
					}
				}
				else // deltaX�� ������ �� (�������� �̵��� ��)
				{
					if (bot != floor(newBottom))
					{
						xMove -= ratio; // y �̵����� ���� x �̵���
					}
					for (int x = ceil(newX); x >= ceil(xMove); x--)
					{
						if (bot == floor(newBottom) && x == ceil(newX)) continue;
						isMoveAvailX = checkMoveAvail(x, bot);

						if (!isMoveAvailX) // �̵��� �Ұ����ϴٸ�
						{
							if (x == ceil(newX))
							{
								isMoveAvailY = false;
							}
							else
							{
								newX = x + 1; // Ȯ�εǾ��� x�� ���ư���
							}
							_angle = PI - _angle;
							_power *= 0.8;
							break;
						}

						isPixelAvail = checkPixelAvail(x, bot); // ���� ���� �ִ��� üũ

						if (isPixelAvail) // ���� ���� �ִٸ�
						{
							newX = x;
							break;
						}

						if (x == ceil(xMove)) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
						{
							newX = xMove;
						}
					}
				}

				if (!isMoveAvailY)
				{
					newBottom = bot;
					break;
				}
				if (isPixelAvail) // x�� �̵� ��� �� ������ ���
				{
					_x = newX;
					_y = bot - (_height / 2);
					_rc = RectMakeCenter(_x, _y, _width, _height);
					return true;
				}

				if (bot == floor(yMove)) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
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
		else // x�ุ ����� �ʿ��� ��
		{
			float xMove = newX + deltaX;
			int bot = floor(newBottom);
			bool isMoveAvail = false;
			bool isPixelAvail = false;

			if (deltaX >= 0) // deltaX�� ����� �� (���������� �̵��� ��, �̵����� ���� ��)
			{
				for (int x = floor(newX); x <= floor(xMove); x++)
				{
					isMoveAvail = checkMoveAvail(x, bot); // �̵� �������� üũ

					if (!isMoveAvail) // �̵��� �Ұ����ϴٸ�
					{
						if (x == floor(newX))
						{
							newBottom -= 1;
						}
						else
						{
							newX = x - 1; // Ȯ�εǾ��� x�� ���ư���
						}
						_angle = PI - _angle;
						_power *= 0.8;
						break;
					}

					isPixelAvail = checkPixelAvail(x, bot); // ���� ���� �ִ��� üũ

					if (isPixelAvail) // ���� ���� �ִٸ�
					{
						newX = x;
						break;
					}

					if (x == floor(xMove)) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
					{
						newX = xMove;
					}
				}
			}
			else // deltaX�� ������ �� (�������� �̵��� ��)
			{
				for (int x = ceil(newX); x >= ceil(xMove); x--)
				{
					isMoveAvail = checkMoveAvail(x, bot);

					if (!isMoveAvail) // �̵��� �Ұ����ϴٸ�
					{
						if (x == ceil(newX))
						{
							newBottom -= 1;
						}
						else
						{
							newX = x + 1; // Ȯ�εǾ��� x�� ���ư���
						}
						_angle = PI - _angle;
						_power *= 0.8;
						break;
					}

					isPixelAvail = checkPixelAvail(x, bot); // ���� ���� �ִ��� üũ

					if (isPixelAvail) // ���� ���� �ִٸ�
					{
						newX = x;
						break;
					}

					if (x == ceil(xMove)) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
					{
						newX = xMove;
					}
				}
			}

			if (isPixelAvail) // x�� �̵� ��� �� ������ ���
			{
				_x = newX;
				_y = newBottom - (_height / 2);
				_rc = RectMakeCenter(_x, _y, _width, _height);
				return true;
			}

			if (isMoveAvail) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
			{
				newBottom += deltaY;
			}
		}
	}
	else // deltaY�� ������ �� (���� �̵��� ��), ���� �̵��� ���� ���� üũ ����
	{
		if (ceil(newBottom) != ceil(yMove)) // ratio ������� ����� ��
		{
			for (int bot = ceil(newBottom); bot >= ceil(yMove); bot--)
			{
				bool isMoveAvailY = true; // Y �� �̵��� �������� �˻��ϴ� �� ����
				bool isMoveAvailX = false; // X �� �̵��� �������� �˻��ϴ� �� ����

				float xMove = newX;

				if (deltaX >= 0) // deltaX�� ����� �� (���������� �̵��� ��, �̵����� ���� ��)
				{
					if (bot != ceil(newBottom)) // ù ������ �ƴ� ��� ratio��ŭ ������
					{
						xMove += ratio; // y �̵����� ���� x �̵���
					}

					for (int x = floor(newX); x <= floor(xMove); x++)
					{
						isMoveAvailX = checkMoveAvail(x, bot); // x�� �̵��� �������� �˻�

						if (!isMoveAvailX) // x�� �̵��� �Ұ����� ��
						{
							if(x != floor(newX))
							{
								newX = x - 1;
							}
							_angle = PI - _angle;
							_power *= 0.8; 
							isMoveAvailY = false; // y �� �̵� �� �� �� ����
							break;
						}

						if (x == floor(xMove)) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
						{
							newX = xMove;
						}
					}
				}
				else // deltaX�� ������ �� (�������� �̵��� ��)
				{
					if (bot != ceil(newBottom))
					{
						xMove -= ratio; // y �̵����� ���� x �̵���
					}

					for (int x = ceil(newX); x >= ceil(xMove); x--)
					{
						isMoveAvailX = checkMoveAvail(x, bot);

						if (!isMoveAvailX) // X �� �̵��� �ȵ� ��
						{
							if (x != ceil(newX))
							{
								newX = x + 1;
							}
							_angle = PI - _angle;
							_power *= 0.8;
							isMoveAvailY = false; // Y �� �̵� �� �� �� ����
							break;
						}

						if (x == ceil(xMove)) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
						{
							newX = xMove;
						}
					}
				}

				if (!isMoveAvailY) // y������ �� �̻� �̵��� �� ���� ���
				{
					//if (bot != ceil(newBottom)) // y�� �̵��� �����ߴ� �������� �ǵ���
					//{
					//	newBottom = bot + 1;
					//}
					break;
				}

				if (bot == ceil(yMove)) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
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
		else // x�ุ ����� �ʿ��� �� 
		{
			float xMove = newX + deltaX;
			bool isMoveAvailX = true;
			if (deltaX >= 0) // deltaX�� ����� �� (���������� �̵��� ��, �̵����� ���� ��)
			{
				for (int x = floor(newX) + 1; x <= floor(xMove); x++)
				{
					isMoveAvailX = checkMoveAvail(x, newBottom); // x�� �̵��� �������� �˻�

					if (!isMoveAvailX) // x�� �̵��� �Ұ����� ��
					{
						newX = x - 1;
						_angle = PI - _angle;
						_power *= 0.8;
						break;
					}
					if (x == floor(xMove)) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
					{
						newX = xMove;
					}
				}
			}
			else // deltaX�� ������ �� (�������� �̵��� ��)
			{
				for (int x = ceil(newX) - 1; x >= ceil(xMove); x--)
				{
					isMoveAvailX = checkMoveAvail(x, newBottom);

					if (!isMoveAvailX) // X �� �̵��� �ȵ� ��
					{
						newX = x + 1;
						_angle = PI - _angle;
						_power *= 0.8;
						break;
					}

					if (x == ceil(xMove)) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
					{
						newX = xMove;
					}
				}
			}
			if (isMoveAvailX) // x�� �̵��� ���� ������ ��
			{
				newBottom += deltaY;
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
