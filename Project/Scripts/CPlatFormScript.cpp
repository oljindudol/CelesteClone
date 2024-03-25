#include "pch.h"
#include "CPlatFormScript.h"

CPlatFormScript::CPlatFormScript()
	: CScript(PLATFORMSCRIPT)
{
}

CPlatFormScript::~CPlatFormScript()
{
}


void CPlatFormScript::begin()
{
}

void CPlatFormScript::tick()
{
}

void CPlatFormScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	Vec2 ThisPos = _Collider->GetOffsetPos();
	Vec2 ThisScale = _Collider->GetOffsetScale();
	auto ThisAbs = _Collider->IsAbsolute();
	if (false == ThisAbs)
	{
		auto ownerpos = _Collider->GetOwner()->Transform()->GetWorldPos();
		auto ownerscale = _Collider->GetOwner()->Transform()->GetWorldScale();
		ThisPos = ownerpos * ThisPos;
		ThisScale = ownerscale * ThisScale;
	}

	Vec2 OtherPos = _OtherCollider->GetOffsetPos();
	Vec2 OtherScale = _OtherCollider->GetOffsetScale();
	auto OtherAbs = _OtherCollider->IsAbsolute();
	if (false == OtherAbs)
	{
		auto ownerpos = _OtherObj->Transform()->GetWorldPos();
		auto ownerscale = _OtherObj->Transform()->GetWorldScale();
		OtherPos = ownerpos * ThisPos;
		OtherScale = ownerscale * ThisScale;
	}


	if ((UINT)LAYER::PLAYER == _OtherObj->GetLayerIdx() ||
		(UINT)LAYER::MONSTER == _OtherObj->GetLayerIdx())
	{
		float plattop = (_Collider->GetOffsetPos().y - _Collider->GetScale().y / 2.f);
		float otherprevbottom = (_OtherCol->GetPrevPos().y + _OtherCol->GetScale().y / 2.f);

		float yfix = ((UINT)LAYER::PLAYER == _OtherObj->GetLayerIdx()) ? 0.99f : 0.97f;

		//위에서 올라왔으면, y스피드를 0으로 고정
		if (plattop > otherprevbottom)// *yfix)
		{

			CMovement* mov = _OtherObj->GetMovement();

			if (nullptr != mov)
			{
				Vec2 v = mov->GetVelocity();
				mov->SetVelocity(Vec2(v.x, 0.f));
			}
		}
	}
}

void CPlatFormScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	float plattop = (_OwnCol->GetPos().y - _OwnCol->GetScale().y / 2.f);
	float otherprevbottom = (_OtherCol->GetPrevPos().y + _OtherCol->GetScale().y / 2.f);
	//float otherbottom =( _OtherCol->GetPos().y +_OtherCol->GetScale().y / 2.f);

	//if (_OwnCol->GetName() == L"PlatformCollider2")
	//	LOG(LOG_LEVEL::LOG, std::to_wstring(otherbottom).c_str());
	//if ((UINT)LAYER::MONSTER == _OtherObj->GetLayerIdx())
	//	int a = 0;

	float yfix = ((UINT)LAYER::PLAYER == _OtherObj->GetLayerIdx()) ? 0.99f : 0.95f;


	if (plattop >= otherprevbottom * yfix)// && plattop <= otherbottom)
	{

		float up = (_OwnCol->GetScale().y / 2.f
			+ _OtherCol->GetScale().y / 2.f
			- abs(_OwnCol->GetPos().y
				- _OtherCol->GetPos().y)
			) / 2.f;

		_OtherObj->SetPos(Vec2(_OtherObj->GetPos().x, _OtherObj->GetPos().y - up));
	}
}
}

void CPlatFormScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlatFormScript::SaveToFile(FILE* _File)
{
}

void CPlatFormScript::LoadFromFile(FILE* _File)
{
}

