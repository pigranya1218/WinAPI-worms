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
	}
}

void objectManager::update()
{
	for (int i = 0; i < _objects.size(); i++)
	{
		_objects[i]->update();
	}
}

void objectManager::render()
{
	for (int i = 0; i < _objects.size(); i++)
	{
		_objects[i]->render();
	}
}
