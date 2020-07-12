#include "stdafx.h"
#include "objectManager.h"

HRESULT objectManager::init()
{
	return S_OK;
}

void objectManager::release()
{
	for (int i = 0; i < _objects.size(); i++)
	{
		_objects[i]->release();
		delete _objects[i];
	}
}

void objectManager::update()
{
	for (int i = 0; i < _objects.size();)
	{
		_objects[i]->update();
		if (_objects[i]->isFinish()) // ���� object�� ���ش�
		{
			_objects[i]->release();
			delete _objects[i];
			_objects.erase(_objects.begin() + i);
		} 
		else
		{
			i++;
		}
	}
}

void objectManager::render()
{
	for (int i = 0; i < _objects.size(); i++)
	{
		_objects[i]->render();
	}
}

void objectManager::objectBomb(float x, float y, float power, float width)
{
	MYCIRCLE bombCircle = { x, y, width / 2 };

	for (int i = 0; i < _objects.size(); i++)
	{
		RECT wormRC = _objects[i]->getRect();
		if (TTYONE_UTIL::isCollision(bombCircle, wormRC)) // ���Ĺݰ�� ������ �浹���� �˻�
		{
			float distance = TTYONE_UTIL::getDistance(x, y, _objects[i]->getX(), _objects[i]->getY());
			float ratio = 1.4 - (distance / (width * 0.5)); // �Ÿ�����
			ratio = min(1, ratio); // �ּ� ���� ����

			float angle = TTYONE_UTIL::getAngle(x, y, _objects[i]->getX(), _objects[i]->getY());
			float degree = angle * (180 / PI);
			_objects[i]->hit(angle, power * 0.2 * ratio);
		}
	}
}

bool objectManager::checkNoUpdate()
{
	for (int i = 0; i < _objects.size(); i++)
	{
		if (_objects[i]->isUpdate())
		{
			return false;
		}
	}
	return true;
}
