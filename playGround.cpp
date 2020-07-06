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
	// * 스테이지
	// ** 스테이지 이미지 관련
	IMAGE_MANAGER->addImage("BACKGROUND", "resources/images/background.bmp", 2000, 1125, false, NULL);
	IMAGE_MANAGER->addImage("STAGE", "resources/images/bigMap.bmp", 2000, 1125, true, RGB(255, 0, 255));
	IMAGE_MANAGER->addImage("SEA", "resources/images/sea.bmp", 2000, 170, true, RGB(255, 0, 255));
	
	
	// * 웜즈 애니메이션 관련
	// ** IDLE
	// *** BIG HEAD
	IMAGE_MANAGER->addFrameImage("IDLE_BIGHEAD_NORMAL", "resources/images/wbghead.bmp", 60, 600, 1, 10, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("IDLE_BIGHEAD_DOWN", "resources/images/wbgheadd.bmp", 60, 600, 1, 10, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("IDLE_BIGHEAD_UP", "resources/images/wbgheadu.bmp", 60, 600, 1, 10, true, RGB(128, 128, 192));
	
	// ** MOVE
	IMAGE_MANAGER->addFrameImage("MOVE_NORMAL", "resources/images/wwalk.bmp", 60, 900, 1, 15, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("MOVE_DOWN", "resources/images/wwalkd.bmp", 60, 900, 1, 15, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("MOVE_UP", "resources/images/wwalku.bmp", 60, 900, 1, 15, true, RGB(128, 128, 192));

	// ** JUMP
	IMAGE_MANAGER->addFrameImage("JUMP_NORMAL", "resources/images/wjump.bmp", 60, 600, 1, 10, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("JUMP_DOWN", "resources/images/wjumpd.bmp", 60, 600, 1, 10, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("JUMP_UP", "resources/images/wjumpu.bmp", 60, 600, 1, 10, true, RGB(128, 128, 192));

	// ** ATTACK

	// ** FALLEN
	IMAGE_MANAGER->addFrameImage("FALLEN", "resources/images/wfall.bmp", 60, 120, 1, 2, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("FALLEN_TWANG_NORMAL", "resources/images/wtwang.bmp", 60, 2940, 1, 49, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("FALLEN_TWANG_DOWN", "resources/images/wtwangd.bmp", 60, 2940, 1, 49, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("FALLEN_TWANG_UP", "resources/images/wtwangu.bmp", 60, 2940, 1, 49, true, RGB(128, 128, 192));

	// ** etc
	IMAGE_MANAGER->addImage("redDC", "resources/images/redDC.bmp", 500, 500, false, NULL);
	IMAGE_MANAGER->addImage("redTest", "resources/images/redtest.bmp", 27, 24, true, RGB(255, 0, 255));


	// 씬 추가
	SCENE_MANAGER->addScene("StageScene", new stageScene);
	dynamic_cast<stageScene*>(SCENE_MANAGER->findScene("StageScene"))->init(1, 1, 60); // 개발용...
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
