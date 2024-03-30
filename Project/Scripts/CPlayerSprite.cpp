#include "pch.h"
#include "CPlayerSprite.h"
#include "CPlayerScript.h"



CPlayerSprite::CPlayerSprite()
	:Hair(nullptr)
{
	Mode = PlayerSpriteMode::MadelineNoBackpack;

	spriteName = "player_no_backpack";
}

CPlayerSprite::CPlayerSprite(CPlayerScript* _Owner,PlayerSpriteMode mode)
	:CPlayerSprite()
{
	m_Owner = _Owner;
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

void CPlayerSprite::Play(string _animid)
{
	if (LastAnimationID == _animid)
	{
		return;
	}
	LastAnimationID = CurrentAnimationID;
	CurrentAnimationID = _animid;
	
	m_Owner->Animator2D()->Play(ToWString(_animid));
}
