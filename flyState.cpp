#include "stdafx.h"
#include "allState.h"
#include "worm.h"

float flyState::getFrameAngle(worm& player)
{
	float frameAngle = player.getDisplayAngle();
	float angleDegree = frameAngle * (180 / PI);
	if (frameAngle >= PI * 0.5 && frameAngle <= PI * 1.5) // ¿ÞÂÊ
	{
		frameAngle -= PI / 2;
	}
	else // ¿À¸¥ÂÊ
	{
		frameAngle += PI / 2;
		frameAngle = PI - frameAngle;
	}
	if (frameAngle > PI2) frameAngle -= PI2;

	return frameAngle;
}

int flyState::getYFrameIndex(worm& player)
{
	float frameAngle = getFrameAngle(player);
	frameAngle = PI2 - frameAngle; // reverse
	frameAngle += (PI / 2);

	if (frameAngle > PI2) frameAngle -= PI2;
	if (frameAngle < 0) frameAngle += PI2;

	int portion = frameAngle / _frameAngleOffset;
	int frameIndex = floor(((frameAngle / _frameAngleOffset) + 1) / 2);

	return frameIndex;
}

void flyState::enter(worm& player)
{
	if (player.getPower() >= 4)
	{
		_state = FLY_STATE::FLY;
		SOUND_MANAGER->stop("WORM_FLY");
		SOUND_MANAGER->play("WORM_FLY", 1);
	}
	else
	{
		_state = FLY_STATE::SLIDING;
		SOUND_MANAGER->stop("WORM_SLIDING");
		SOUND_MANAGER->play("WORM_SLIDING", 1);
	}
	player.setWaiting();
}

void flyState::exit(worm& player)
{
	SAFE_DELETE(_ani);
}

state* flyState::update(worm& player)
{
	

	switch (_state)
	{
	case FLY_STATE::FLY:
	case FLY_STATE::SLIDING:
	{
		float lastX = player.getX();
		float lastY = player.getY();
		_isGround = player.gravityMove(0, 0.8); // ³¯¶ó°¡´Â Áß

		float angle = player.getDisplayAngle();
		if (angle >= PI * 0.5 && angle <= PI * 1.5)
		{
			player.setDirection(DIRECTION::LEFT);
		}
		else
		{
			player.setDirection(DIRECTION::RIGHT);
		}

		if (_isGround) // ¶¥¿¡ ´ê¾ÒÀ» ¶§
		{
			float gravity = player.getGravity(); // Áß·Â °ª
			float power = player.getPower(); // ³¯¶ó´Ù´Ñ Èû

			if (gravity >= 7) // ¶¥¿¡ ³Ê¹« ½ê°Ô ¹Ú¾ÒÀ» ¶§
			{
				player.setAngle(0);
				player.setPower(0);
				return new fallenState;
			}
			else if (power >= 4)// ¶¥¿¡ Á» ½ê°Ô ¹Ú¾ÒÀ» ¶§
			{
				_state = FLY_STATE::FLY;
				_currFrameCount = 0;
			}
			else if (power >=  0.4) // ¶¥¿¡ ´ú ½ê°Ô ¹Ú¾ÒÀ» ¶§ (½½¶óÀÌµù)
			{
				_state = FLY_STATE::SLIDING;
				_currFrameCount = 0;
				_currFrame = 0;
				player.updateSlope();
				SOUND_MANAGER->stop("WORM_SLIDING");
				SOUND_MANAGER->play("WORM_SLIDING", 1);
			}
			else // ÁøÂ¥ ¾àÇÏ°Ô ºÎµúÇûÀ» ¶§ (STOP)
			{
				player.setGravity(0);
				player.setPower(0);
				player.setAngle(0);
				player.updateSlope();
				_state = FLY_STATE::TO_IDLE;
				_img = IMAGE_MANAGER->findImage(getImageKey("SLIDE_LINK", player.getSlope()));
				_ani = new animation;
				_ani->init(_img->getWidth(), _img->getHeight(), _img->getFrameWidth(), _img->getFrameHeight());
				_ani->setDefPlayFrame(false, false);
				_ani->setFPS(20);
				_ani->start();
			}
			player.setGravity(gravity * 0.2); // ´Ù½Ã Æ¨±æ ¼ö ÀÖµµ·Ï ÇÔ
		}
	}
	break;
	case FLY_STATE::TO_IDLE:
	{
		if (!_ani->isPlay())
		{
			return new idleState;
		}
		_ani->frameUpdate(TIME_MANAGER->getElapsedTime());
	}
	break;
	}

	return nullptr;
}

void flyState::render(worm& player)
{
	RECT rc = player.getRect();

	float x = (rc.left + rc.right) / 2, y = (rc.top + rc.bottom) / 2;

	switch (_state)
	{
	case FLY_STATE::FLY:
	{
		sprintf_s(_buffer, "FLY%d", (_currFrame + 1));
		_img = IMAGE_MANAGER->findImage(_buffer);

		_currFrameCount++;
		if (_currFrameCount == 3)
		{
			_currFrameCount = 0;
			_currFrame = (_currFrame + 1) % 3;
			sprintf_s(_buffer, "FLY%d", (_currFrame + 1));
			_img = IMAGE_MANAGER->findImage(_buffer);
		}
		int frameY = getYFrameIndex(player);
		CAMERA_MANAGER->frameRender(getMemDC(), _img, x - (_img->getFrameWidth() / 2), y - (_img->getFrameHeight() / 2), 0, frameY, (player.getDirection() == DIRECTION::RIGHT));
	}
	break;
	case FLY_STATE::SLIDING:
	{
		_img = IMAGE_MANAGER->findImage(getImageKey("SLIDE", player.getSlope()));

		_currFrameCount++;
		if (_currFrameCount == 4)
		{
			_currFrameCount = 0;
			_currFrame = (_currFrame + 1) % 3;
		}
		CAMERA_MANAGER->frameRender(getMemDC(), _img, x - (_img->getFrameWidth() / 2), y - (_img->getFrameHeight() / 2), 0, _currFrame, (player.getDirection() == DIRECTION::RIGHT));
	}
	break;
	case FLY_STATE::TO_IDLE:
	{
		_img = IMAGE_MANAGER->findImage(getImageKey("SLIDE_LINK", player.getSlope()));
		CAMERA_MANAGER->aniRender(getMemDC(), _img, x - (_img->getFrameWidth() / 2), y - (_img->getFrameHeight() / 2), _ani, (player.getDirection() == DIRECTION::RIGHT));
	}
	break;
	}
	
}
