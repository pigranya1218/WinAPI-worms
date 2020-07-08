#include "stdafx.h"
#include "effectManager.h"
#include "effect.h"


effectManager::effectManager()
{
}


effectManager::~effectManager()
{
}

HRESULT effectManager::init()
{
	return S_OK;
}

void effectManager::release()
{
	for (auto vIter = _vTotalEffect.begin(); vIter != _vTotalEffect.end(); )
	{
		if (vIter->second.size() != NULL)
		{
			while(!vIter->second.empty())
			{
				vIter->second[0]->release();
				delete vIter->second[0];
				vIter->second.erase(vIter->second.begin());
			}
		}
		else ++vIter;
	}
}

void effectManager::update()
{
	for (auto vIter = _vTotalEffect.begin(); vIter != _vTotalEffect.end(); vIter++)
	{
		for (int i = 0; i < vIter->second.size() ; i++)
		{
			vIter->second[i]->update();
		}
	}
}

void effectManager::render()
{
	for (auto vIter = _vTotalEffect.begin(); vIter != _vTotalEffect.end(); vIter++)
	{
		for (int i = 0; i < vIter->second.size(); i++)
		{
			vIter->second[i]->render();
		}
	}
}

void effectManager::addEffect(string effectName, const char * imageName, int imageWidth, int imageHeight, int effectWidth, int effectHeight, int fps, int buffer)
{
	image* img;
	arrEffects vEffectBuffer;

	if (IMAGE_MANAGER->findImage(imageName))
	{
		img = IMAGE_MANAGER->findImage(imageName);
	}
	else
	{
		img = IMAGE_MANAGER->addImage(imageName, imageName, imageWidth, imageHeight, true, RGB(255, 0, 255));
	}

	for (int i = 0; i < buffer; ++i)
	{
		vEffectBuffer.push_back(new effect);
		vEffectBuffer[i]->init(img, effectWidth, effectHeight, fps);
	}

	_vTotalEffect.insert(pair<string, arrEffects>(effectName, vEffectBuffer));
}

void effectManager::play(string effectName, int x, int y)
{
	for (auto vIter = _vTotalEffect.begin(); vIter != _vTotalEffect.end(); vIter++)
	{
		if (!(vIter->first == effectName)) continue;

		for (int i = 0; i < vIter->second.size(); i++)
		{
			if (vIter->second[i]->getIsRunning()) continue;
			vIter->second[i]->startEffect(x, y);
			return;
		}
	}
}

void effectManager::play(string effectName, int x, int y, int width, int height)
{
	for (auto vIter = _vTotalEffect.begin(); vIter != _vTotalEffect.end(); vIter++)
	{
		if (!(vIter->first == effectName)) continue;

		for (int i = 0; i < vIter->second.size() ; i++)
		{
			if (vIter->second[i]->getIsRunning()) continue;
			vIter->second[i]->startEffect(x, y, width, height);
			return;
		}
	}
}
