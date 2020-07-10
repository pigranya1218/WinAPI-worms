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
	IMAGE_MANAGER->addImage("STAGE_BACK", "resources/images/bigMapBack.bmp", 2000, 1125, true, RGB(255, 0, 255));
	IMAGE_MANAGER->addImage("STAGE_SHADOW", "resources/images/bigMapShadow.bmp", 2000, 1125, true, RGB(255, 0, 255));
	IMAGE_MANAGER->addImage("SEA", "resources/images/sea.bmp", 2000, 170, true, RGB(255, 0, 255));
	IMAGE_MANAGER->addImage("WIND_LEFT", "resources/images/windl.bmp", 222, 30, false, NULL);
	IMAGE_MANAGER->addImage("WIND_RIGHT", "resources/images/windr.bmp", 222, 30, false, NULL);
	
	
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

	// ** LONG JUMP
	IMAGE_MANAGER->addFrameImage("LONGJUMP", "resources/images/wbackflp.bmp", 60, 1320, 1, 22, true, RGB(128, 128, 192));

	// ** FLY
	IMAGE_MANAGER->addFrameImage("FLY1", "resources/images/wfly1.bmp", 60, 1920, 1, 32, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("FLY2", "resources/images/wfly2.bmp", 60, 1920, 1, 32, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("FLY3", "resources/images/wfly3.bmp", 60, 1920, 1, 32, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("SLIDE_NORMAL", "resources/images/wslide.bmp", 60, 180, 1, 3, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("SLIDE_DOWN", "resources/images/wslided.bmp", 60, 180, 1, 3, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("SLIDE_UP", "resources/images/wslideu.bmp", 60, 180, 1, 3, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("SLIDE_LINK_NORMAL", "resources/images/wsldlk2.bmp", 60, 2160, 1, 36, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("SLIDE_LINK_DOWN", "resources/images/wsldlk2d.bmp", 60, 2160, 1, 36, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("SLIDE_LINK_UP", "resources/images/wsldlk2u.bmp", 60, 2160, 1, 36, true, RGB(128, 128, 192));

	// ** ATTACK
	// *** AIM
	IMAGE_MANAGER->addFrameImage("WEAPON_AIM", "resources/images/weapon/aim/aim.bmp", 60, 1920, 1, 32, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("WEAPON_BLOB", "resources/images/weapon/aim/blob.bmp", 64, 1024, 1, 16, true, RGB(128, 128, 192));

	// *** PROJECTILE
	IMAGE_MANAGER->addFrameImage("PROJECTILE_MISSILE", "resources/images/weapon/projectile/missile.bmp", 60, 1920, 1, 32, true, RGB(128, 128, 192));

	// *** EFFECT
	IMAGE_MANAGER->addFrameImage("EFFECT_CIRCLE", "resources/images/weapon/effect/circle50.bmp", 100, 800, 1, 8, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("EFFECT_ELIPSE", "resources/images/weapon/effect/elipse50.bmp", 150, 3000, 1, 20, true, RGB(32, 32, 248));
	IMAGE_MANAGER->addFrameImage("EFFECT_SMOKE_WHITE", "resources/images/weapon/effect/smklt50.bmp", 60, 1680, 1, 28, true, RGB(192, 192, 128));
	IMAGE_MANAGER->addFrameImage("EFFECT_SMOKE_DARK", "resources/images/weapon/effect/smkdrk30.bmp", 30, 840, 1, 28, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("EFFECT_FLAME", "resources/images/weapon/effect/flame1.bmp", 60, 1920, 1, 32, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("EFFECT_EX_POW", "resources/images/weapon/effect/expow.bmp", 60, 720, 1, 12, true, RGB(128, 128, 192));

	EFFECT_MANAGER->addEffect("EFFECT_CIRCLE", "EFFECT_CIRCLE", IMAGE_MANAGER->findImage("EFFECT_CIRCLE")->getWidth(), IMAGE_MANAGER->findImage("EFFECT_CIRCLE")->getHeight(),
		IMAGE_MANAGER->findImage("EFFECT_CIRCLE")->getFrameWidth(), IMAGE_MANAGER->findImage("EFFECT_CIRCLE")->getFrameHeight(), 15, 10);
	EFFECT_MANAGER->addEffect("EFFECT_ELIPSE", "EFFECT_ELIPSE", IMAGE_MANAGER->findImage("EFFECT_ELIPSE")->getWidth(), IMAGE_MANAGER->findImage("EFFECT_ELIPSE")->getHeight(),
		IMAGE_MANAGER->findImage("EFFECT_ELIPSE")->getFrameWidth(), IMAGE_MANAGER->findImage("EFFECT_ELIPSE")->getFrameHeight(), 30, 10);
	EFFECT_MANAGER->addEffect("EFFECT_SMOKE_WHITE", "EFFECT_SMOKE_WHITE", IMAGE_MANAGER->findImage("EFFECT_SMOKE_WHITE")->getWidth(), IMAGE_MANAGER->findImage("EFFECT_SMOKE_WHITE")->getHeight(),
		IMAGE_MANAGER->findImage("EFFECT_SMOKE_WHITE")->getFrameWidth(), IMAGE_MANAGER->findImage("EFFECT_SMOKE_WHITE")->getFrameHeight(), 30, 10);
	EFFECT_MANAGER->addEffect("EFFECT_SMOKE_DARK", "EFFECT_SMOKE_DARK", IMAGE_MANAGER->findImage("EFFECT_SMOKE_DARK")->getWidth(), IMAGE_MANAGER->findImage("EFFECT_SMOKE_DARK")->getHeight(),
		IMAGE_MANAGER->findImage("EFFECT_SMOKE_DARK")->getFrameWidth(), IMAGE_MANAGER->findImage("EFFECT_SMOKE_DARK")->getFrameHeight(), 30, 10);
	EFFECT_MANAGER->addEffect("EFFECT_FLAME", "EFFECT_FLAME", IMAGE_MANAGER->findImage("EFFECT_FLAME")->getWidth(), IMAGE_MANAGER->findImage("EFFECT_FLAME")->getHeight(),
		IMAGE_MANAGER->findImage("EFFECT_FLAME")->getFrameWidth(), IMAGE_MANAGER->findImage("EFFECT_FLAME")->getFrameHeight(), 30, 10);
	EFFECT_MANAGER->addEffect("EFFECT_EX_POW", "EFFECT_EX_POW", IMAGE_MANAGER->findImage("EFFECT_EX_POW")->getWidth(), IMAGE_MANAGER->findImage("EFFECT_EX_POW")->getHeight(),
		IMAGE_MANAGER->findImage("EFFECT_EX_POW")->getFrameWidth(), IMAGE_MANAGER->findImage("EFFECT_EX_POW")->getFrameHeight(), 25, 10);


	// *** BAZUKA
	IMAGE_MANAGER->addFrameImage("WEAPON_BAZUKA_NORMAL", "resources/images/weapon/bazuka/wbaz.bmp", 60, 1920, 1, 32, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("WEAPON_BAZUKA_DOWN", "resources/images/weapon/bazuka/wbazd.bmp", 60, 1920, 1, 32, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("WEAPON_BAZUKA_UP", "resources/images/weapon/bazuka/wbazu.bmp", 60, 1920, 1, 32, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("WEAPON_BAZUKA_BACK_NORMAL", "resources/images/weapon/bazuka/wbazbak.bmp", 60, 420, 1, 7, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("WEAPON_BAZUKA_BACK_DOWN", "resources/images/weapon/bazuka/wbazbak.bmp", 60, 420, 1, 7, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("WEAPON_BAZUKA_BACK_UP", "resources/images/weapon/bazuka/wbazbak.bmp", 60, 420, 1, 7, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("WEAPON_BAZUKA_LINK_NORMAL", "resources/images/weapon/bazuka/wbazbak.bmp", 60, 420, 1, 7, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("WEAPON_BAZUKA_LINK_DOWN", "resources/images/weapon/bazuka/wbazbak.bmp", 60, 420, 1, 7, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("WEAPON_BAZUKA_LINK_UP", "resources/images/weapon/bazuka/wbazbak.bmp", 60, 420, 1, 7, true, RGB(128, 128, 192));

	// ** FALLEN
	IMAGE_MANAGER->addFrameImage("FALLEN", "resources/images/wfall.bmp", 60, 120, 1, 2, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("FALLEN_TWANG_NORMAL", "resources/images/wtwang.bmp", 60, 2940, 1, 49, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("FALLEN_TWANG_DOWN", "resources/images/wtwangd.bmp", 60, 2940, 1, 49, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("FALLEN_TWANG_UP", "resources/images/wtwangu.bmp", 60, 2940, 1, 49, true, RGB(128, 128, 192));

	// ** DEAD
	IMAGE_MANAGER->addFrameImage("DEAD", "resources/images/wdie.bmp", 60, 3600, 1, 60, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("GRAVE1", "resources/images/grave1.bmp", 60, 1200, 1, 20, true, RGB(192, 192, 128));
	IMAGE_MANAGER->addFrameImage("GRAVE2", "resources/images/grave2.bmp", 60, 1200, 1, 20, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("GRAVE3", "resources/images/grave3.bmp", 60, 1200, 1, 20, true, RGB(128, 128, 192));
	IMAGE_MANAGER->addFrameImage("GRAVE4", "resources/images/grave4.bmp", 60, 1200, 1, 20, true, RGB(192, 192, 128));
	IMAGE_MANAGER->addFrameImage("GRAVE5", "resources/images/grave5.bmp", 60, 1200, 1, 20, true, RGB(192, 192, 128));
	IMAGE_MANAGER->addFrameImage("GRAVE6", "resources/images/grave6.bmp", 60, 1200, 1, 20, true, RGB(192, 192, 128));

	// ** etc
	IMAGE_MANAGER->addImage("redDC", "resources/images/redDC.bmp", 500, 500, false, NULL);
	IMAGE_MANAGER->addImage("redTest", "resources/images/redtest.bmp", 27, 24, true, RGB(255, 0, 255));


	// 씬 추가
	SCENE_MANAGER->addScene("StageScene", new stageScene);
	dynamic_cast<stageScene*>(SCENE_MANAGER->findScene("StageScene"))->init(4, 1, 30); // 개발용...
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
