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
