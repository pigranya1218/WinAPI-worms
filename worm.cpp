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
	for (int checkY = bottom - _height; checkY < bottom; checkY++) // 앞이 막혀있는지 체크
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

int worm::checkGroundAvail(int x, int bottom)
{
	for (int bot = bottom - _offsetClimb; bot <= bottom + _offsetClimb; bot++) // 위에서부터 땅이 있는지 체크
	{
		if (checkPixelAvail(x, bot))
		{
			return bot;
		}
	}

	return -1; // 밟을 발판이 없음
}

bool worm::checkPixelAvail(int x, int bottom)
{
	COLORREF rgb = _wormManager->getPixel(x, bottom);

	int r = GetRValue(rgb);
	int g = GetGValue(rgb);
	int b = GetBValue(rgb);

	if (!(r == 255 && g == 0 && b == 255)) // 밟을 수 있음
	{
		return true;
	}

	return false;
}

HRESULT worm::init(wormManager* wormManager, int index, string name, float x, float y) // x 좌표와 바닥 높이
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
	_weaponCount.insert(make_pair(WEAPON_CODE::TELEPORT, 1));
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

	if (_wormManager->getCurrentTurnIndex() != _index && !isDead()) // 체력바랑 이름 표시하기
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

// 중심점을 기준으로 보고 있는 방향 봐서 각도 변화가 있는지 파악함
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
	float deltaX = _speed * ((_dir == DIRECTION::LEFT) ? -1 : 1);
	float newX = _x;
	float newBottom = _rc.bottom;

	if (floor(_x) == floor(_x + deltaX)) // 움직임 변화 없는 경우
	{
		_x += deltaX;
		return true;
	}

	int offset = (_dir == DIRECTION::LEFT) ? -1 : 1;

	for (int x = floor(_x) + offset; ((_dir == DIRECTION::LEFT) ? (x >= floor(_x + deltaX)) : (x <= floor(_x + deltaX))); x += offset) // 움직임 변화 있는 경우
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
			newX = x - offset;
			break;
		}

		if (x == floor(_x + deltaX)) // 이동 가능 범위 끝까지 이동한 경우
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

// false : 아직 떨어지는 중 , true : 착지함
bool worm::gravityMove(float xPower, float powerChange) // xPower = x축 움직임 더하기
{
	_gravity += 0.08; // 중력 업데이트
	float deltaX = cosf(_angle) * _power + xPower; // X축 이동값
	float deltaY = -sinf(_angle) * _power + _gravity; // Y축 이동값
	_displayAngle = atan2f(-deltaY, deltaX);
	if (_displayAngle < 0) _displayAngle += PI2;

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
							_power *= powerChange;
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
							_power *= powerChange;
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
					_rc = RectMakeCenter(_x, _y, _width, _height);
					_power *= powerChange;
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
				for (int x = floor(newX) ; x <= floor(xMove); x++)
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
						_power *= powerChange;
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
				for (int x = floor(newX) ; x >= floor(xMove); x--)
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
						_power *= powerChange;
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
				_rc = RectMakeCenter(_x, _y, _width, _height);
				_power *= powerChange;
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
							_power *= powerChange;
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
							_power *= powerChange;
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
						_power *= powerChange;
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
						_power *= powerChange;
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
	if (_index == getStageManager()->getCurrentTurnIndex())
	{
		getStageManager()->setWaiting();
	}
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
