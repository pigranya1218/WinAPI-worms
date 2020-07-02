#include "stdafx.h"
#include "playGround.h"


playGround::playGround()
{
}


playGround::~playGround()
{
}

//�ʱ�ȭ �Լ�
HRESULT playGround::init()
{
	gameNode::init(true);

	// ��� �̹���, �ִϸ��̼�, ���� �߰�
	IMAGE_MANAGER->addImage("BACKGROUND", "resources/images/background.bmp", 2000, 1125, false, NULL);
	IMAGE_MANAGER->addImage("STAGE", "resources/images/bigMap.bmp", 2000, 1125, true, RGB(255, 0, 255));

	// �� �߰�
	SCENE_MANAGER->addScene("StageScene", new stageScene);
	dynamic_cast<stageScene*>(SCENE_MANAGER->findScene("StageScene"))->init(2, 1, 60); // ���߿�...
	SCENE_MANAGER->changeScene("StageScene");

	return S_OK;
}

//�޸� ����
void playGround::release()
{
	
}

//����
void playGround::update()
{
	gameNode::update();

	SCENE_MANAGER->update();
}

//�׸��� ����
void playGround::render()
{	
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================

	SCENE_MANAGER->render();

	//=============================================
	_backBuffer->render(getHDC(), 0, 0);
}
