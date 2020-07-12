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
		delete _projectiles[i];
	}
}

void projectileManager::update()
{
	for (int i = 0; i < _projectiles.size();)
	{
		_projectiles[i]->update();
		if (_projectiles[i]->isFinish())
		{
			_projectiles[i]->release();
			delete _projectiles[i];
			_projectiles.erase(_projectiles.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

void projectileManager::render()
{
	for (int i = 0; i < _projectiles.size(); i++)
	{
		_projectiles[i]->render();
	}
}

void projectileManager::addProjectile(projectile* projectile)
{
	_projectiles.push_back(projectile);
}
