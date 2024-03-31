#include "pch.h"
#include "CPlayerHairUpdate.h"
#include "CPlayerSprite.h"
#include <Engine\CHair.h>
#include "CPlayerScript.h"

CPlayerHairUpdate::CPlayerHairUpdate(CPlayerSprite* _Sprite, CHair* _Hair)
{
	Sprite = _Sprite;
	Hair = _Hair;
	auto& Nodes = Hair->GetHairNodesRef();

	for (int i = 0; i < Sprite->HairCount; i++)
	{
		tHairNode node = {};
		
		Nodes.push_back(node);
	}
}

CPlayerHairUpdate::~CPlayerHairUpdate()
{

}

void CPlayerHairUpdate::Start()
{
	auto facing = Sprite->m_Owner->GetOwner()->m_facing;
	auto worldpos = Hair->GetOwner()->Transform()->GetWorldPos();
	Vec2 at = Vec2(worldpos.x, worldpos.y) + Vec2((float)(-facing * (Facings)200), 200.f);
	auto& Nodes = Hair->GetHairNodesRef();

	for (size_t i = 0; i < Nodes.size(); i++)
	{
		Nodes[i].vOffset = Vec2(at.x,at.y);
	}
}

void CPlayerHairUpdate::Update()
{
	wave += DT * 4.f;
}

void CPlayerHairUpdate::AfterUpdate()
{
	auto player = Sprite->m_Owner->GetOwner();
	auto Facing = player->m_facing;
	auto SpriteScale = player->Animator2D()->GetMulScale();
	auto& Nodes = Hair->GetHairNodesRef();
	auto playerworldscale = player->Transform()->GetWorldScale();

	Vec2 offset = Sprite->HairOffset() * Vec2((float)Facing, 1.f);
	Nodes[0].vOffset = Vec2(playerworldscale.x, playerworldscale.y) + Vec2(0.f, -9.f * SpriteScale.y) + offset;
	Vec2 target = Nodes[0].vOffset + Vec2((float)(-(float)Facing) * StepInFacingPerSegment * 2.f, (float)sinf((double)wave) * StepYSinePerSegment) + StepPerSegment;
	Vec2 prev = Nodes[0].vOffset;
	float maxdist = 3.f;
	for (int i = 1; i < Sprite->HairCount; i++)
	{
		if (i >= Nodes.size())
		{
			Nodes.push_back(Nodes[i - 1]);
		}
		if (SimulateMotion)
		{
			float approach = (1.f - (float)i / (float)Sprite->HairCount * 0.5f) * StepApproach;
			Nodes[i].vOffset = Approach(Nodes[i].vOffset, target, approach * DT);
		}
		if ((Nodes[i].vOffset - prev).Length() > maxdist)
		{
			auto tmp = (Nodes[i].vOffset - prev);
			tmp.Normalize();
			Nodes[i].vOffset = prev + tmp * maxdist;
		}
		target = Nodes[i].vOffset + Vec2((float)(-(float)Facing) * StepInFacingPerSegment, (float)sin((double)(wave + (float)i * 0.8f)) * StepYSinePerSegment) + StepPerSegment;
		prev = Nodes[i].vOffset;
	}
}

Vec2 CPlayerHairUpdate::GetHairScale()
{
	return Vec2();
}


