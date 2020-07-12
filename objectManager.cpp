#include "stdafx.h"
#include "objectManager.h"
#include "effectObject.h"

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

void objectManager::makeBombEffect(int x, int y, int width)
{
	// �Ҳ� �����
	for (int i = 0; i < 8; i++)
	{
		int newX = RND->getFromIntTo(x - width / 2, x + width / 2);
		int newY = RND->getFromIntTo(y - width / 2, y + width / 2);
		float newPI = RND->getFromFloatTo(PI * 0.3, PI * 0.7);
		float newPower = RND->getFromFloatTo(2, 4);

		effectObject* newEffect = new effectObject;
		newEffect->init(IMAGE_MANAGER->findImage("EFFECT_FLAME"), 20, newX, newY, 60, 60, newPI, newPower, true, false);
		_objects.push_back(newEffect);
	}

	// ���� ȸ�� ���Ա׸� �����
	for (int i = 0; i < 16; i++)
	{
		int newX = RND->getFromIntTo(x - width / 2, x + width / 2);
		int newY = RND->getFromIntTo(y - width / 2, y + width / 2);

		effectObject* newEffect = new effectObject;
		newEffect->init(IMAGE_MANAGER->findImage("EFFECT_SMOKE_DARK"), 30, newX, newY, 30, 30, PI * 0.5, 1, false, false);
		_objects.push_back(newEffect);
	}
	
	// ũ�� �� ���Ա��� �����
	for (int i = 0; i < 10; i++)
	{
		int newX = RND->getFromIntTo(x - width / 2, x + width / 2);
		int newY = RND->getFromIntTo(y - width / 2, y + width / 2);
		float newPI = RND->getFromFloatTo(PI * 0.3, PI * 0.7);

		effectObject* newEffect = new effectObject;
		newEffect->init(IMAGE_MANAGER->findImage("EFFECT_SMOKE_WHITE"), 30, newX, newY, 60, 60, newPI, 1, false, false);
		_objects.push_back(newEffect);
	}


}
