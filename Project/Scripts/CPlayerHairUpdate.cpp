#include "pch.h"
#include "CPlayerHairUpdate.h"
#include "CPlayerSprite.h"
#include <Engine\CHair.h>
#include "CPlayerScript.h"



CPlayerHairUpdate::CPlayerHairUpdate(CPlayerSprite* _Sprite, CHair* _Hair)
{
	Sprite = _Sprite;
	Hair = _Hair;
	auto& RenderInfoRef = Hair->GetRenderInfoRef();
	auto& Nodes = RenderInfoRef.vecHairNodes;

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
	auto worldpos = Hair->GetOwner()->Transform()->GetWorldPos();
	Vec2 at = Vec2(worldpos.x, worldpos.y) + Vec2((float)(-facing * (Facings)200), 200.f);
	auto& RenderInfo = Hair->GetRenderInfoRef();
	auto& Nodes = RenderInfo.vecHairNodes;

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
	auto SpriteScale = player->Animator2D()->GetMulScale();
	auto playerworldscale = player->Transform()->GetWorldScale();
	
	auto& RenderInfoRef = Hair->GetRenderInfoRef();
	auto& Nodes = RenderInfoRef.vecHairNodes;
	
	auto Info = Hair->GettHairInfo();


	Vec2 offset = Info.HairOffset * Vec2((float)facing, 1.f);
	Nodes[0].vOffset = Vec2(playerworldscale.x, playerworldscale.y) + Vec2(0.f, -9.f * SpriteScale.y) + offset;
	Vec2 target = Nodes[0].vOffset + Vec2((float)(-(float)facing) * StepInFacingPerSegment * 2.f, (float)sinf((double)wave) * StepYSinePerSegment) + StepPerSegment;
	Vec2 prev = Nodes[0].vOffset;
	float maxdist = 3.f;
	for (int i = 1; i < Sprite->HairCount; i++)
	{
		// ������ 
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
		target = Nodes[i].vOffset + Vec2((float)(-(float)facing) * StepInFacingPerSegment, (float)sin((double)(wave + (float)i * 0.8f)) * StepYSinePerSegment) + StepPerSegment;
		prev = Nodes[i].vOffset;

		// Scale
		Nodes[i].vScale = GetHairScale(i);
	}

	// Bang
	RenderInfoRef.ThisFrameBangIdx = Info.BangFrame;
	// HasHair
	RenderInfoRef.HasHair = Info.HasHair;

	// ���ī��Ʈ ���
	RenderInfoRef.HairCnt =  Sprite->HairCount;
}

Vec2 CPlayerHairUpdate::GetHairScale(int index)
{
	auto player = Sprite->m_Owner->GetOwner();
	auto Facing = player->m_facing;
	auto SpriteScale = player->Animator2D()->GetMulScale();

	float scale = 0.25f + (1.f - (float)index / (float)Sprite->HairCount) * 0.75f;
	return Vec2(((index == 0) ? ((float)Facing) : scale) * abs(SpriteScale.x), scale);
}


