#include "stdafx.h"
#include "banana.h"
#include "stageManager.h"
#include "wormManager.h"
#include "objectManager.h"


bool banana::checkMoveAvail(int x, int bot)
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

bool banana::checkPixelAvail(int x, int bot)
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

void banana::makeBanana(float angle)
{
	banana* _projectile = new banana;

	float initGravity = 0;
	float initAngle = angle;
	float initPower = 6;

	float initX = _x;
	float initY = _y;
	float widthX = _width * 0.5;
	float widthY = _width * 0.5;
	float initBombWidth = _bombWidth * 0.6;
	float initDamage = _damage * 0.6;

	_projectile->init(_stageManager, _wormManager, _objectManager, initX, initY, widthX, widthY, initAngle, initPower, initDamage, initBombWidth, true, true, true, true, true);
	_projectile->setImage(IMAGE_MANAGER->findImage("PROJECTILE_BANANA_SMALL"));
	_projectile->setAnimation();
	_projectile->setLevel(1);

	_wormManager->shoot(_projectile);
}

bool banana::gravityMove(float xPower)
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

void banana::update()
{
	if (!_isFinish)
	{
		EFFECT_MANAGER->play("EFFECT_HEXHAUST", _x, _y, 20 * _level , 20 * _level);
		_isGround = gravityMove(_stageManager->getWind() * 0.2);

		if (_isGround) // ���Ľ�Ű��
		{
			if (++_bombCount >= _maxBombCount)
			{
				EFFECT_MANAGER->play("EFFECT_CIRCLE", _x, _y, _bombWidth, _bombWidth);
				EFFECT_MANAGER->play("EFFECT_ELIPSE", _x, _y, _bombWidth + 30, _bombWidth + 30);
				EFFECT_MANAGER->play("EFFECT_EX_POW", _x, _y - 50, 50, 50);

				_stageManager->pixelBomb(_x, _y, _damage, _bombWidth); // �ȼ� ���Ľ�Ű��
				_wormManager->wormBomb(_x, _y, _damage, _bombWidth); // ���� ���� ���󰡰� �ϱ�
				_objectManager->objectBomb(_x, _y, _damage, _bombWidth);
				_objectManager->makeBombEffect(_x, _y, _bombWidth);

				_isFinish = true;
				if (_level > 1)
				{
					float angle = PI * 0.85;
					for (int i = 0; i < 5; i++)
					{
						makeBanana(angle);
						angle -= PI * 0.15;
					}
				}
			}
			else
			{
				_gravity = 0;
			}
		} 
		else
		{
			_ani->frameUpdate(TIME_MANAGER->getElapsedTime());
		}
	}
}

void banana::render()
{
	CAMERA_MANAGER->aniRender(getMemDC(), _img, _x - (_img->getFrameWidth() / 2), _y - (_img->getFrameHeight() / 2), _ani, false);
}
