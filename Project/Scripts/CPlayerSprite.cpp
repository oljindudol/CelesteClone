#include "pch.h"
#include "CPlayerSprite.h"




CPlayerSprite::CPlayerSprite()
{
	Mode = PlayerSpriteMode::MadelineNoBackpack;

	spriteName = "player_no_backpack";
}

CPlayerSprite::CPlayerSprite(PlayerSpriteMode mode)
{
	Mode = mode;
	string sprite = "";
	if (mode == PlayerSpriteMode::Madeline)
	{
		sprite = "player";
	}
	else if (mode == PlayerSpriteMode::MadelineNoBackpack)
	{
		sprite = "player_no_backpack";
	}
	else if (mode == PlayerSpriteMode::Badeline)
	{
		sprite = "badeline";
	}
	else if (mode == PlayerSpriteMode::MadelineAsBadeline)
	{
		sprite = "player_badeline";
	}
	else if (mode == PlayerSpriteMode::Playback)
	{
		sprite = "player_playback";
	}
	spriteName = sprite;
	//GFX.SpriteBank.CreateOn(this, sprite);
}

CPlayerSprite::~CPlayerSprite()
{
}

Vec2 CPlayerSprite::HairOffset()
{
	return Vec2();
}

int CPlayerSprite::HairFrame()
{
	return 0;
}

bool CPlayerSprite::HasHair()
{
	return false;
}

bool CPlayerSprite::Running()
{
	return false;
}

bool CPlayerSprite::DreamDashing()
{
	return false;
}
