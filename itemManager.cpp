#include "stdafx.h"
#include "itemManager.h"

HRESULT itemManager::init()
{
	return S_OK;
}

void itemManager::release()
{
	for (int i = 0; i < _items.size(); i++)
	{
		_items[i]->release();
	}
}

void itemManager::update()
{
	for (int i = 0; i < _items.size(); i++)
	{
		_items[i]->update();
	}
}

void itemManager::render()
{
	for (int i = 0; i < _items.size(); i++)
	{
		_items[i]->render();
	}
}
