#include "stdafx.h"
#include "worm.h"
#include "allState.h"
#include "allWeapon.h"
#include "wormManager.h"
#include "stageManager.h"
#include <cmath>

class flystate;

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

HRESULT worm::init(wormManager* wormManager, int index, string name, float x, float y) // x ��ǥ�� �ٴ� ����
{
	_wormManager = wormManager;

	_index = index;
	_x = x;
	_y = y - (_height / 2);
	_name = name;
	_rc = RectMakeCenter(_x, _y, _width, _height);

	int random = RND->getInt(2);
	_dir = (random == 0) ? DIRECTION::RIGHT : DIRECTION::LEFT;
	updateSlope();

	_state = new idleState;
	_state->enter(*this);

	_weapon = nullptr;

	_weaponCount.insert(make_pair(WEAPON_CODE::BAZOOKA, -1));
	_weaponCount.insert(make_pair(WEAPON_CODE::DONKEY, 1));
	_weaponCount.insert(make_pair(WEAPON_CODE::BANANA, 1));
	//_weaponCount.insert(make_pair(WEAPON_CODE::FIRESTRK, 1));
	_weaponCount.insert(make_pair(WEAPON_CODE::MINE, 1));
	_weaponCount.insert(make_pair(WEAPON_CODE::HOMING, 2));

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
	//CAMERA_MANAGER->rectangle(getMemDC(), _rc);

	if (_wormManager->getCurrentTurnIndex() != _index && !isDead()) // ü�¹ٶ� �̸� ǥ���ϱ�
	{
		renderUI();
	}
}

void worm::setWeapon(weapon* weapon)
{
	if (!_canAttack) return;
	_state->exit(*this);
	delete _state;
	delete _weapon;
	_state = new attackState;
	_weapon = weapon;
	_state->enter(*this);
}

// �߽����� �������� ���� �ִ� ���� ���� ���� ��ȭ�� �ִ��� �ľ���
void worm::updateSlope()
{
	int bottom = _rc.bottom;
	int xPos = (_dir == DIRECTION::LEFT) ? (_x - 2) : (_x + 2);
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
	float deltaX = _speed * ((_dir == DIRECTION::LEFT) ? -1 : 1);
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

// false : ���� �������� �� , true : ������
bool worm::gravityMove(float xPower, float powerChange) // xPower = x�� ������ ���ϱ�
{
	_gravity += 0.08; // �߷� ������Ʈ
	float deltaX = cosf(_angle) * _power + xPower; // X�� �̵���
	float deltaY = -sinf(_angle) * _power + _gravity; // Y�� �̵���
	_displayAngle = atan2f(-deltaY, deltaX);
	if (_displayAngle < 0) _displayAngle += PI2;

	if (deltaY == 0)
	{
		deltaY = 0.0001;
	}
	float ratio = abs(deltaX / deltaY); // y�� ��ȭ�� ���� x�� ��ȭ�� ����

	float newX = _x;
	float newBottom = _y + (_height / 2);
	float yMove = newBottom + deltaY;

	if (deltaY >= 0) // deltaY�� ����� �� (�Ʒ��� ������ �� or �������� ���� ��)
	{
		if (abs(deltaY) >= 1) // ratio ������� ����� ��
		{
			for (int bot = floor(newBottom); bot <= floor(yMove); bot++)
			{
				bool isMoveAvailX = false; // ������ �� �ִ��� �˻�
				bool isPixelAvail = false; // ���� ���� �ִ��� �˻�
				bool isMoveAvailY = true;
				float firstDeltaX = (floor(newBottom) + 1 - newBottom);
				float xMove = newX;

				if (deltaX >= 0) // deltaX�� ����� �� (���������� �̵��� ��, �̵����� ���� ��)
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
						xMove += ratio; // y �̵����� ���� x �̵���
					}

					for (int x = floor(newX); x <= floor(xMove); x++)
					{
						//if (bot == floor(newBottom) && x == floor(newX)) continue;

						isMoveAvailX = checkMoveAvail(x, bot); // �̵� �������� üũ

						if (!isMoveAvailX) // �̵��� �Ұ����ϴٸ�
						{
							if (x == floor(newX))
							{
								isMoveAvailY = false;
								newX = x;
							}
							else
							{
								newX = x - 1; // Ȯ�εǾ��� x�� ���ư���
							}
							_angle = PI - _angle;
							_power *= powerChange;
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
						xMove -= ratio; // y �̵����� ���� x �̵���
					}
					for (int x = floor(newX); x >= floor(xMove); x--)
					{
						//if (bot == floor(newBottom) && x == floor(newX)) continue;
						isMoveAvailX = checkMoveAvail(x, bot);

						if (!isMoveAvailX) // �̵��� �Ұ����ϴٸ�
						{
							if (x == floor(newX))
							{
								isMoveAvailY = false;
								newX = x;
							}
							else
							{
								newX = x + 1; // Ȯ�εǾ��� x�� ���ư���
							}
							_angle = PI - _angle;
							_power *= powerChange;
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

				if (!isMoveAvailY)
				{
					newBottom = bot + 1;
					break;
				}
				if (isPixelAvail) // x�� �̵� ��� �� ������ ���
				{
					_x = newX;
					_y = bot - (_height / 2);
					_rc = RectMakeCenter(_x, _y, _width, _height);
					_power *= powerChange;
					return true;
				}

				if (bot == floor(yMove)) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
				{
					newBottom = yMove;
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
				for (int x = floor(newX) ; x <= floor(xMove); x++)
				{
					isMoveAvail = checkMoveAvail(x, bot); // �̵� �������� üũ

					if (!isMoveAvail) // �̵��� �Ұ����ϴٸ�
					{
						if (x == floor(newX))
						{
							newBottom -= 1;
							newX = x;
						}
						else
						{
							newX = x - 1; // Ȯ�εǾ��� x�� ���ư���
						}
						_angle = PI - _angle;
						_power *= powerChange;
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
				for (int x = floor(newX) ; x >= floor(xMove); x--)
				{
					isMoveAvail = checkMoveAvail(x, bot);

					if (!isMoveAvail) // �̵��� �Ұ����ϴٸ�
					{
						if (x == floor(newX))
						{
							newBottom -= 1;
							newX = x;
						}
						else
						{
							newX = x + 1; // Ȯ�εǾ��� x�� ���ư���
						}
						_angle = PI - _angle;
						_power *= powerChange;
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

			if (isPixelAvail) // x�� �̵� ��� �� ������ ���
			{
				_x = newX;
				_y = newBottom - (_height / 2);
				_rc = RectMakeCenter(_x, _y, _width, _height);
				_power *= powerChange;
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
		if (abs(deltaY) >= 1) // y�� �̵��� �־� ratio ������� ����� ��
		{
			for (int bot = floor(newBottom); bot >= floor(yMove); bot--)
			{
				bool isMoveAvailY = true; // Y �� �̵��� �������� �˻��ϴ� �� ����
				bool isMoveAvailX = false; // X �� �̵��� �������� �˻��ϴ� �� ����
				
				float firstDeltaX = (newBottom - floor(newBottom));
				float xMove = newX;

				if (deltaX >= 0) // deltaX�� ����� �� (���������� �̵��� ��, �̵����� ���� ��)
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
						xMove += ratio; // y �̵����� ���� x �̵���
					}

					for (int x = floor(newX); x <= floor(xMove); x++)
					{
						isMoveAvailX = checkMoveAvail(x, bot); // x�� �̵��� �������� �˻�

						if (!isMoveAvailX) // x�� �̵��� �Ұ����� ��
						{
							if (x == floor(newX))
							{
								newX = x;
								isMoveAvailY = false; // y �� �̵� �� �� �� ����
							}
							else
							{
								newX = x - 1;
							}
							_angle = PI - _angle;
							_power *= powerChange;
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
						xMove -= ratio; // y �̵����� ���� x �̵���
					}

					for (int x = floor(newX); x >= floor(xMove); x--)
					{
						isMoveAvailX = checkMoveAvail(x, bot);

						if (!isMoveAvailX) // X �� �̵��� �ȵ� ��
						{
							if (x == floor(newX))
							{
								isMoveAvailY = false; // Y �� �̵� �� �� �� ����
								newX = x;
							}
							else
							{
								newX = x + 1;
							}
							_angle = PI - _angle;
							_power *= powerChange;
							break;
						}

						if (x == floor(xMove)) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
						{
							newX = xMove;
						}
					}
				}

				if (!isMoveAvailY) // y������ �� �̻� �̵��� �� ���� ���
				{
					newBottom = bot - 1;
					break;
				}

				if (bot == floor(yMove)) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
				{
					newBottom = yMove;
				}
			}
		}
		else // x�ุ ����� �ʿ��� �� 
		{
			float xMove = newX + deltaX;
			int bot = floor(newBottom);
			bool isMoveAvailX = true;

			if (deltaX >= 0) // deltaX�� ����� �� (���������� �̵��� ��, �̵����� ���� ��)
			{
				for (int x = floor(newX); x <= floor(xMove); x++)
				{
					isMoveAvailX = checkMoveAvail(x, bot); // x�� �̵��� �������� �˻�

					if (!isMoveAvailX) // x�� �̵��� �Ұ����� ��
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
						_power *= powerChange;
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
				for (int x = floor(newX); x >= floor(xMove); x--)
				{
					isMoveAvailX = checkMoveAvail(x, bot);

					if (!isMoveAvailX) // X �� �̵��� �ȵ� ��
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
						_power *= powerChange;
						break;
					}

					if (x == floor(xMove)) // �̵��� �� �ִ� ��ŭ �� �̵��� ���
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
	return (_wormManager->getCurrentTurnIndex() == _index && getStageManager()->isTurn());
}

void worm::hit(float angle, float power)
{
	_angle = angle;
	_power = power;

	if (!isDead())
	{
		_state->exit(*this);
		delete _state;
		_state = new flyState;
		_state->enter(*this);
	}
}

void worm::setDamage(int damage)
{
	_wormManager->wormDamage(_index, damage);
}

void worm::renderUI()
{
	_wormManager->renderWormUI(_x, _y, _currHp, _index, _name);
}

void worm::setWaiting()
{
	getStageManager()->setWaiting();
}

void worm::removeWeapon()
{
	_weapon->exit(*this);
	delete _weapon;
	_weapon = nullptr;
}

void worm::setDead()
{
	_state->exit(*this);
	delete _state;
	_state = new deadState;
	_state->enter(*this);
}

bool worm::isFreshDead()
{
	if (_currHp == 0 && !dynamic_cast<deadState*>(_state))
	{
		return true;
	}
	return false;
}

bool worm::isDead()
{
	if (dynamic_cast<deadState*>(_state))
	{
		return true;
	}
	return false;
}

bool worm::isStop()
{
	return _state->isStop();
}

stageManager * worm::getStageManager()
{
	return _wormManager->getStageManager();
}

wormManager * worm::getWormManager()
{
	return _wormManager;
}

objectManager* worm::getObjectManager()
{
	return _wormManager->getObjectManager();
}
