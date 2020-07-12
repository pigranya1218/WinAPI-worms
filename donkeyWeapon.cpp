#include "stdafx.h"
#include "allWeapon.h"
#include "worm.h"
#include "state.h"
#include "stageManager.h"
#include "wormManager.h"

void donkeyWeapon::shot(worm& shooter)
{
    donkey* _donkey = new donkey;

    float initGravity = 0;
    float initAngle = 0;
    float initPower = _power;

    float initX = _x;
    float initY = 100;
    float widthX = 100;
    float widthY = 2;
    float initBombWidth = _bombWidth;
    float initDamage = _damage;

    _donkey->init(shooter.getStageManager(), shooter.getWormManager(), shooter.getObjectManager(), initX, initY, widthX, widthY, initAngle, initPower, initDamage, initBombWidth, false, false, true, false, true);
    _donkey->setImage(IMAGE_MANAGER->findImage("WEAPON_DONKEY"));

    shooter.getWormManager()->shoot(_donkey);
    shooter.adjustWeapon(WEAPON_CODE::DONKEY, -1);
}

void donkeyWeapon::setWaiting(worm& player)
{
    _wormImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_AIR_BACK", player.getSlope()));
    _wormAni->init(_wormImg->getWidth(), _wormImg->getHeight(), _wormImg->getFrameWidth(), _wormImg->getFrameHeight());
    _wormAni->setDefPlayFrame(false, false);
    _wormAni->setFPS(30);
    _wormAni->start();
    _state = WEAPON_STATE::WAITING;
    player.setWaiting();
}

void donkeyWeapon::enter(worm& player)
{
    _power = 0;
    _damage = 50;
    _bombWidth = 240;
    _state = WEAPON_STATE::BEGIN;
    _x = player.getX();
    _y = player.getY();

    _wormImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_AIR_LINK", player.getSlope()));
    _wormAni = new animation;
    _wormAni->init(_wormImg->getWidth(), _wormImg->getHeight(), _wormImg->getFrameWidth(), _wormImg->getFrameHeight());
    _wormAni->setDefPlayFrame(false, false);
    _wormAni->setFPS(30);
    _wormAni->start();

    _markerImg = IMAGE_MANAGER->findImage("WEAPON_MARKER");
    _markerAni = new animation;
    _markerAni->init(_markerImg->getWidth(), _markerImg->getHeight(), _markerImg->getFrameWidth(), _markerImg->getFrameHeight());
    _markerAni->setDefPlayFrame(true, true);
    _markerAni->setFPS(40);
    _markerAni->start();
}

void donkeyWeapon::exit(worm& player)
{
    SAFE_DELETE(_wormAni);
    SAFE_DELETE(_markerAni);
}

WEAPON_FINISH_TYPE donkeyWeapon::update(worm& player)
{
    switch (_state)
    {
    case WEAPON_STATE::BEGIN:
    {
        if (player.isTurn())
        {
            if (KEY_MANAGER->isOnceKeyDown(VK_LEFT) || KEY_MANAGER->isOnceKeyDown(VK_RIGHT)) // 이동을 시도하는 경우
            {
                return WEAPON_FINISH_TYPE::MOVING;
            }
            else if (KEY_MANAGER->isOnceKeyDown(VK_LCONTROL)) // 점프를 시동하는 경우
            {
                return WEAPON_FINISH_TYPE::JUMPING;
            }

            if (!_wormAni->isPlay())
            {
                _state = WEAPON_STATE::IDLE;
            }
        }
        else
        {
            setWaiting(player);
        }
    }
    break;
    case WEAPON_STATE::IDLE: // 발사 위치 조정
    {
        if (player.isTurn())
        {
            if (KEY_MANAGER->isStayKeyDown(VK_LBUTTON))
            {
                _x = CAMERA_MANAGER->getAbsoluteL(_ptMouse.x);
                _y = CAMERA_MANAGER->getAbsoluteT(_ptMouse.y);
            }
            else if (KEY_MANAGER->isOnceKeyDown(VK_SPACE)) // 발사!!
            {
                _wormImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_AIR", player.getSlope()));
                _wormAni->init(_wormImg->getWidth(), _wormImg->getHeight(), _wormImg->getFrameWidth(), _wormImg->getFrameHeight());
                _wormAni->setDefPlayFrame(false, false);
                _wormAni->setFPS(30);
                _wormAni->start();
                _state = WEAPON_STATE::SHOOTING;
            }
            else if (KEY_MANAGER->isOnceKeyDown(VK_LEFT) || KEY_MANAGER->isOnceKeyDown(VK_RIGHT)) // 이동을 시도하는 경우
            {
                return WEAPON_FINISH_TYPE::MOVING;
            }
            else if (KEY_MANAGER->isOnceKeyDown(VK_LCONTROL)) // 점프를 시동하는 경우
            {
                return WEAPON_FINISH_TYPE::JUMPING;
            }
        }
        else
        {
            setWaiting(player);
        }
    }
    break;
    case WEAPON_STATE::SHOOTING:
    {
        if (!_wormAni->isPlay())
        {
            shot(player);
            setWaiting(player);
        }
    }
    break;
    case WEAPON_STATE::WAITING:
    {
        if (!_wormAni->isPlay())
        {
            _state = WEAPON_STATE::FINISH;
        }
    }
    break;
    case WEAPON_STATE::FINISH:
    {
        player.setWaiting();
        return WEAPON_FINISH_TYPE::FINISH;
    }
    break;
    }

    _markerAni->frameUpdate(TIME_MANAGER->getElapsedTime());
    _wormAni->frameUpdate(TIME_MANAGER->getElapsedTime());

    return WEAPON_FINISH_TYPE::ATTACK;
}

void donkeyWeapon::render(worm& player)
{
    CAMERA_MANAGER->aniRender(getMemDC(), _wormImg, player.getX() - 30, player.getY() - 30, _wormAni, (player.getDirection() == DIRECTION::RIGHT));
 
    switch (_state)
    {
    case WEAPON_STATE::IDLE:
    case WEAPON_STATE::SHOOTING:
    case WEAPON_STATE::WAITING:
    case WEAPON_STATE::FINISH:
    {
        CAMERA_MANAGER->aniRender(getMemDC(), _markerImg, _x - (_markerImg->getFrameWidth() / 2), _y - (_markerImg->getFrameHeight() / 2), _markerAni, false);
    }
    break;
    }
}
