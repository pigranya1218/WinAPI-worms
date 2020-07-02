#include "stdafx.h"
#include "cameraManager.h"

cameraManager::cameraManager()
{
}

cameraManager::~cameraManager()
{
}

HRESULT cameraManager::init()
{
	return S_OK;
}

void cameraManager::release()
{
}

// �ʱ� ����, _L, _T�� ī�޶��� LEFT, TOP
void cameraManager::setConfig(float L, float T, float width, float height, float minL, float minT, float maxL, float maxT)
{
	_L = L;
	_T = T;
	_width = width;
	_height = height;
	_minL = minL;
	_minT = minT;
	_maxL = maxL;
	_maxT = maxT;

	_x = _L + (_width / 2);
	_y = _T + (_height / 2);
	_minX = _minL + (_width / 2);
	_minY = _minT + (_height / 2);
	_maxX = _maxL + (_width / 2);
	_maxY = _maxT + (_height / 2);
}

void cameraManager::setConfigCenter(float x, float y, float width, float height, float minX, float minY, float maxX, float maxY)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	_minX = minX;
	_minY = minY;
	_maxX = maxX;
	_maxY = maxY;

	_L = _x - (width / 2);
	_T = _y - (height / 2);
	_minL = _minX - (width / 2);
	_minT = _minY - (height / 2);
	_maxL = _maxX - (width / 2);
	_maxT = _minY - (height / 2);
}

float cameraManager::getL()
{
	return _L;
}

float cameraManager::getT()
{
	return _T;
}

float cameraManager::getX()
{
	return _x;
}

float cameraManager::getY()
{
	return _y;
}

void cameraManager::setL(float newL)
{
	newL = min(_maxL, newL);
	newL = max(_minL, newL);
	_L = floor(newL);
	_x = _L + (_width / 2);
}

void cameraManager::setT(float newT)
{
	newT = min(_maxT, newT);
	newT = max(_minT, newT);
	_T = floor(newT);
	_y = _T + (_height / 2);
}

// ī�޶��� ������ X�� ����
void cameraManager::setX(float newX)
{
	newX = min(_maxX, newX);
	newX = max(_minX, newX);
	_x = floor(newX);
	_L = _x - (_width / 2);
}

// ī�޶��� ������ Y�� ����
void cameraManager::setY(float newY)
{
	newY = min(_maxY, newY);
	newY = max(_minY, newY);
	_y = floor(newY);
	_T = _y - (_height / 2);
}

// ī�޶��� �������� �̵���Ŵ
void cameraManager::movePivot(float offsetX, float offsetY)
{
	setL(_L + offsetX);
	setT(_T + offsetY);
	setX(_x + offsetX);
	setY(_y + offsetY);
}

// ī�޶��� LEFT�� �������� �����ǥ left���� ����
float cameraManager::getRelativeL(float left)
{
	float newL = left - _L;
	return newL;
}

// ī�޶��� TOP�� �������� �����ǥ top���� ����
float cameraManager::getRelativeT(float top)
{
	float newT = top - _T;
	return  newT;
}

// �簢�� �׸��� MYRECT ���
void cameraManager::rectangle(HDC hdc, TTYONE_UTIL::MYRECT mrec)
{
	RECT rect = { getRelativeL(mrec.left), getRelativeT(mrec.top), getRelativeL(mrec.right), getRelativeT(mrec.bottom) };
	Rectangle(hdc, rect);
}

// ���׶�� �׸��� MYRECT ���
void cameraManager::ellipse(HDC hdc, TTYONE_UTIL::MYRECT mrec)
{
	RECT rect = { getRelativeL(mrec.left), getRelativeT(mrec.top), getRelativeL(mrec.right), getRelativeT(mrec.bottom) };
	Ellipse(hdc, rect);
}

void cameraManager::render(HDC hdc, image * img)
{
	float newL = getRelativeL(0);
	float newT = getRelativeT(0);
	if (img) img->render(hdc, newL, newT);
}

void cameraManager::render(HDC hdc, image * img, float destX, float destY)
{
	float newL = getRelativeL(destX);
	float newT = getRelativeT(destY);
	if (img) img->render(hdc, newL, newT);
}

void cameraManager::render(HDC hdc, image * img, float destX, float destY, float sourX, float sourY, float sourWidth, float sourHeight)
{
	float newL = getRelativeL(destX);
	float newT = getRelativeT(destY);
	if (img) img->render(hdc, newL, newT, sourX, sourY, sourWidth, sourHeight);
}

void cameraManager::render(HDC hdc, HDC originDC, float destX, float destY, float width, float height)
{
	float newL = getRelativeL(destX);
	float newT = getRelativeT(destY);
	BitBlt(hdc, newL, newT, width, height, originDC, 0, 0, SRCCOPY);
}

void cameraManager::frameRender(HDC hdc, image * img, float destX, float destY)
{
	float newL = getRelativeL(destX);
	float newT = getRelativeT(destY);
	if (img) img->frameRender(hdc, newL, newT);
}

void cameraManager::frameRender(HDC hdc, image * img, float destX, float destY, int frameX, int frameY)
{
	float newL = getRelativeL(destX);
	float newT = getRelativeT(destY);
	if (img) img->frameRender(hdc, newL, newT, frameX, frameY);
}

void cameraManager::zoom(HDC hdc, float ratio)
{
	if (ratio == 1) return;

	float newWidth = (float)_width / ratio;
	float newHeight = (float)_height / ratio;

	float zoomL = WINSIZEX / 2 - (newWidth / 2);
	float zoomT = WINSIZEY / 2 - (newHeight / 2);

	StretchBlt(hdc, 0, 0, _width, _height,
		hdc, zoomL, zoomT, newWidth, newHeight,
		SRCCOPY);

	// �� ������ ���� Zoom ����
}
