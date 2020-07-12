#include "stdafx.h"
#include "allWeapon.h"
#include "worm.h"
#include "stageManager.h"
#include "wormManager.h"

void teleportWeapon::setWaiting(worm& player)
{
    _wormImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_TELEPORT_BACK", player.getSlope()));
    _wormAni->init(_wormImg->getWidth(), _wormImg->getHeight(), _wormImg->getFrameWidth(), _wormImg->getFrameHeight());
    _wormAni->setDefPlayFrame(false, false);
    _wormAni->setFPS(30);
    _wormAni->start();
    _state = WEAPON_STATE::WAITING;
}

bool teleportWeapon::checkAvail(worm& player, float newX, float newY)
{
    int bot = newY + (player.getHeight() / 2);
    return player.checkMoveAvail(newX, bot);
}

void teleportWeapon::enter(worm& player)
{
    _state = WEAPON_STATE::BEGIN;
    _x = player.getX();
    _y = player.getY();
    _isAvail = true;

    _wormImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_TELEPORT_LINK", player.getSlope()));
    _wormAni = new animation;
    _wormAni->init(_wormImg->getWidth(), _wormImg->getHeight(), _wormImg->getFrameWidth(), _wormImg->getFrameHeight());
    _wormAni->setDefPlayFrame(false, false);
    _wormAni->setFPS(30);
    _wormAni->start();

    _markerImg = IMAGE_MANAGER->findImage("WEAPON_MARKER_OKAY");
    _markerAni = new animation;
    _markerAni->init(_markerImg->getWidth(), _markerImg->getHeight(), _markerImg->getFrameWidth(), _markerImg->getFrameHeight());
    _markerAni->setDefPlayFrame(true, true);
    _markerAni->setFPS(40);
    _markerAni->start();
}

void teleportWeapon::exit(worm& player)
{
    SAFE_DELETE(_wormAni);
    SAFE_DELETE(_markerAni);
}

WEAPON_FINISH_TYPE teleportWeapon::update(worm& player)
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
            player.setWaiting();
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
                _isAvail = checkAvail(player, _x, _y);

                SOUND_MANAGER->stop("EFFECT_CURSOR");
                SOUND_MANAGER->play("EFFECT_CURSOR", 1);
            }
            else if (KEY_MANAGER->isOnceKeyDown(VK_SPACE) && _isAvail) // 발사!!
            {
                _wormImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_TELEPORT", player.getSlope()));
                _wormAni->init(_wormImg->getWidth(), _wormImg->getHeight(), _wormImg->getFrameWidth(), _wormImg->getFrameHeight());
                _wormAni->setDefPlayFrame(false, false);
                _wormAni->setFPS(30);
                _wormAni->start();
                _state = WEAPON_STATE::GAUGING;

                SOUND_MANAGER->stop("WEAPON_TELEPORT");
                SOUND_MANAGER->play("WEAPON_TELEPORT", 1);
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
            player.setWaiting();
        }
    }
    break;
    case WEAPON_STATE::GAUGING: // 순간이동 
    {
        if (!_wormAni->isPlay())
        {
            player.adjustWeapon(WEAPON_CODE::TELEPORT, -1);
            player.setPlayerPos(_x, _y);
            _wormImg = IMAGE_MANAGER->findImage(getImageKey("WEAPON_TELEPORT", player.getSlope()));
            _wormAni->setPlayFrame(47, 0, false, false); // 역으로 재생
            _wormAni->start();
            _state = WEAPON_STATE::SHOOTING;
        }
    }
    break;
    case WEAPON_STATE::SHOOTING:
    {
        if (!_wormAni->isPlay())
        {
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
        return WEAPON_FINISH_TYPE::FALLEN_BUT_MOVE;
    }
    break;
    }

    if (_isAvail)
    {
        _markerImg = IMAGE_MANAGER->findImage("WEAPON_MARKER_OKAY");
    }
    else
    {
        _markerImg = IMAGE_MANAGER->findImage("WEAPON_MARKER_NOTOKAY");
    }
    _markerAni->frameUpdate(TIME_MANAGER->getElapsedTime());
    _wormAni->frameUpdate(TIME_MANAGER->getElapsedTime());

    return WEAPON_FINISH_TYPE::ATTACK;
}

void teleportWeapon::render(worm& player)
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
