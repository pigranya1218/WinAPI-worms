#include "stdafx.h"
#include "mineObject.h"
#include "wormManager.h"
#include "stageManager.h"
#include "objectManager.h"

bool mineObject::checkMoveAvail(int x, int bot)
{
	for (int checkY = bot - _height; checkY < bot; checkY++) // ���� �����ִ��� üũ
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

bool mineObject::checkPixelAvail(int x, int bot)
{
	COLORREF rgb = _wormManager->getPixel(x, bot);

	int r = GetRValue(rgb);
	int g = GetGValue(rgb);
	int b = GetBValue(rgb);

	if (!(r == 255 && g == 0 && b == 255)) // ���� �� ����
	{
		return true;
	}

	return false;
}

bool mineObject::gravityMove(float xPower)
{

	_gravity += 0.08; // �߷� ������Ʈ
	float deltaX = cosf(_angle) * _power + xPower; // X�� �̵���
	float deltaY = -sinf(_angle) * _power + _gravity; // Y�� �̵���

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
							_power *= _powerChange;
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
							_power *= _powerChange;
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
					_power *= _powerChange;
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
				for (int x = floor(newX); x <= floor(xMove); x++)
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
						_power *= _powerChange;
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
				for (int x = floor(newX); x >= floor(xMove); x--)
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
						_power *= _powerChange;
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
				_power *= _powerChange;
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
							_power *= _powerChange;
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
							_power *= _powerChange;
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
						_power *= _powerChange;
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
						_power *= _powerChange;
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
	return false;
}

bool mineObject::checkMineRange()
{
	RECT mineRc = {_x - _detectWidth, _y - _detectWidth, _x + _detectWidth, _y + _detectWidth};
	return _wormManager->checkCollisionPixel(mineRc);
}

void mineObject::bomb()
{
	EFFECT_MANAGER->play("EFFECT_CIRCLE", _x, _y, _bombWidth, _bombWidth);
	EFFECT_MANAGER->play("EFFECT_ELIPSE", _x, _y, _bombWidth + 30, _bombWidth + 30);
	EFFECT_MANAGER->play("EFFECT_EX_POW", _x, _y - 50, 50, 50);

	_stageManager->pixelBomb(_x, _y, _damage, _bombWidth); // �ȼ� ���Ľ�Ű��
	_wormManager->wormBomb(_x, _y, _damage, _bombWidth); // ���� ���� ���󰡰� �ϱ�
	_objectManager->objectBomb(_x, _y, _damage, _bombWidth);
	_objectManager->makeBombEffect(_x, _y, _bombWidth);
}

void mineObject::release()
{
	SAFE_DELETE(_ani);
}

void mineObject::update()
{
	_isUpdate = true;
	_isFinish = false;

	bool isGround = gravityMove(0);
	if (isGround)
	{
		if (_power < 0.5) // ��� ���� ������ ���
		{
			if (_state == MINE_STATE::WAITING && _waitingCount++ >= 200)
			{
				_isUpdate = false;
			}
		}
		else
		{
			SOUND_MANAGER->stop("WEAPON_MINE_TOK");
			SOUND_MANAGER->play("WEAPON_MINE_TOK", 1);
			_gravity = 0;
		}
	}
	else
	{
		_ani->frameUpdate(TIME_MANAGER->getElapsedTime());
	}

	switch (_state)
	{
	case MINE_STATE::WAITING:
	{
		if (_waitingCount >= 200) // �۵� ���� ��
		{
			if (!_arm)
			{
				_arm = true;
				SOUND_MANAGER->stop("WEAPON_MINE_ARM");
				SOUND_MANAGER->play("WEAPON_MINE_ARM", 1);
			}
			if (checkMineRange())
			{
				_state = MINE_STATE::ACTIVE;
			}
		}
		else
		{
		}
	}
	break;
	case MINE_STATE::ACTIVE:
	{
		if (++_fireCount >= 150)
		{
			bomb();
			_isFinish = true;
		}
		else
		{
			if (_fireCount % 8 < 4)
			{
				_img = IMAGE_MANAGER->findImage("WEAPON_MINE_ON");
				SOUND_MANAGER->stop("WEAPON_MINE_TICK");
				SOUND_MANAGER->play("WEAPON_MINE_TICK", 1);
			}
			else
			{
				_img = IMAGE_MANAGER->findImage("WEAPON_MINE_OFF");
			}
		}
	}
	break;
	}
}

void mineObject::render()
{
	CAMERA_MANAGER->aniRender(getMemDC(), _img, _x - (_img->getFrameWidth() / 2), _y - (_img->getFrameHeight() / 2), _ani, false);
}
