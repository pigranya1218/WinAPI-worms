#include "stdafx.h"
#include "playGround.h"


playGround::playGround()
{
}


playGround::~playGround()
{
}

//초기화 함수
HRESULT playGround::init()
{
	gameNode::init(true);

	// 모든 이미지, 애니메이션, 음악 추가
	IMAGE_MANAGER->addImage("BACKGROUND", "resources/images/background.bmp", 2000, 1125, false, NULL);
	IMAGE_MANAGER->addImage("STAGE", "resources/images/bigMap.bmp", 2000, 1125, true, RGB(255, 0, 255));

	// 씬 추가
	SCENE_MANAGER->addScene("StageScene", new stageScene);
	dynamic_cast<stageScene*>(SCENE_MANAGER->findScene("StageScene"))->init(2, 1, 60); // 개발용...
	SCENE_MANAGER->changeScene("StageScene");

	return S_OK;
}

//메모리 해제
void playGround::release()
{
	
}

//연산
void playGround::update()
{
	gameNode::update();

	SCENE_MANAGER->update();
}

//그리기 전용
void playGround::render()
{	
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================

	SCENE_MANAGER->render();

	//=============================================
	_backBuffer->render(getHDC(), 0, 0);
}
