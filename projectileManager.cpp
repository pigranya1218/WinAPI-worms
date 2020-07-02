#include "stdafx.h"
#include "projectileManager.h"

HRESULT projectileManager::init()
{
	return S_OK;
}

void projectileManager::release()
{
	for (int i = 0; i < _projectiles.size(); i++)
	{
		_projectiles[i]->release();
	}
}

void projectileManager::update()
{
	for (int i = 0; i < _projectiles.size(); i++)
	{
		_projectiles[i]->update();
	}
}

void projectileManager::render()
{
	for (int i = 0; i < _projectiles.size(); i++)
	{
		_projectiles[i]->render();
	}
}
