#include "pch.h"
#include "CPlatFormScript.h"

#include "CPlayerScript.h"

CPlatFormScript::CPlatFormScript()
	: CScript((UINT)SCRIPT_TYPE::DREAMBLOCKSCRIPT)
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
	//Vec2 ThisPos = _Collider->GetOffsetPos();
	//Vec2 ThisScale = _Collider->GetOffsetScale();
	//auto ThisAbs = _Collider->IsAbsolute();
	//if (false == ThisAbs)
	//{
	//	auto ownerpos = _Collider->GetOwner()->Transform()->GetWorldPos();
	//	auto ownerscale = _Collider->GetOwner()->Transform()->GetWorldScale();
	//	ThisPos = ownerpos * ThisPos;
	//	ThisScale = ownerscale * ThisScale;
	//}

	//Vec2 OtherPos = _OtherCollider->GetOffsetPos();
	//Vec2 OtherScale = _OtherCollider->GetOffsetScale();
	//auto OtherAbs = _OtherCollider->IsAbsolute();
	//if (false == OtherAbs)
	//{
	//	auto ownerpos = _OtherObj->Transform()->GetWorldPos();
	//	auto ownerscale = _OtherObj->Transform()->GetWorldScale();
	//	OtherPos = ownerpos * ThisPos;
	//	OtherScale = ownerscale * ThisScale;
	//}

	Vec2 ThisPos = _Collider->GetWorldPos();
	Vec2 ThisPrevPos = _Collider->GetPrevWorldPos();
	Vec2 ThisScale = _Collider->GetWorldScale();

	Vec2 OtherPos = _OtherCollider->GetWorldPos();
	Vec2 OtherPrevPos = _OtherCollider->GetPrevWorldPos();
	Vec2 OtherScale = _OtherCollider->GetWorldScale();

	// TODO: if (레이어 판정로직 or 오브젝트 타입 판정로직?)

	float plattop = (ThisPos.y + ThisScale.y / 2.f);
	float otherprevbottom = (OtherPrevPos.y - OtherScale.y / 2.f);

	float yfix = 0.99f;//((UINT)LAYER::PLAYER == _OtherObj->GetLayerIdx()) ? 0.99f : 0.97f;

	//위에서 올라왔으면, y스피드를 0으로 고정
	if (plattop > otherprevbottom)// *yfix)
	{
		auto vs = _OtherObj->GetScripts();
		CPlayerScript* ps = nullptr;
		for (auto& s : vs)
		{
			ps = dynamic_cast<CPlayerScript*>(s);
			if (nullptr != ps)
			{
				break;
			}
		}

		if (nullptr != ps)
		{
			ps->SetGround();
		}
	}
}

void CPlatFormScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	Vec2 ThisPos = _Collider->GetWorldPos();
	Vec2 ThisPrevPos = _Collider->GetPrevWorldPos();
	Vec2 ThisScale = _Collider->GetWorldScale();

	Vec2 OtherPos = _OtherCollider->GetWorldPos();
	Vec2 OtherPrevPos = _OtherCollider->GetPrevWorldPos();
	Vec2 OtherScale = _OtherCollider->GetWorldScale();

	float plattop = (ThisPos.y + ThisScale.y / 2.f);
	float otherprevbottom = (OtherPrevPos.y - OtherScale.y / 2.f);
	//float otherbottom =( _OtherCol->GetPos().y +_OtherCol->GetScale().y / 2.f);

	//if (_OwnCol->GetName() == L"PlatformCollider2")
	//	LOG(LOG_LEVEL::LOG, std::to_wstring(otherbottom).c_str());
	//if ((UINT)LAYER::MONSTER == _OtherObj->GetLayerIdx())
	//	int a = 0;

	float yfix = 1.f;// 0.99f; // ((UINT)LAYER::PLAYER == _OtherObj->GetLayerIdx()) ? 0.99f : 0.95f;


	if (plattop >= otherprevbottom * yfix)// && plattop <= otherbottom)
	{

		float up = (ThisScale.y / 2.f
			+ OtherScale.y / 2.f
			- abs(ThisPos.y
				- OtherPos.y)
			) / 2.f;

		auto OtherOffset = _OtherCollider->GetOffsetPos();
		auto OtherObTrans = _OtherObj->Transform();

		if(true == _OtherCollider->IsAbsolute())
		{
			up -= _OtherCollider->GetOffsetPos().y;

		}
		else
		{
			auto ws = OtherObTrans->GetWorldScale().y;
			auto op = _OtherCollider->GetOffsetPos().y;

			up -= (ws * op);
		}

		Vec3 OtherObPos = OtherObTrans->GetRelativePos();
		_OtherObj->Transform()->SetRelativePos(Vec3(OtherObPos.x, OtherObPos.y + up, OtherObPos.z));
	}
}

void CPlatFormScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	auto vs = _OtherObj->GetScripts();
	CPlayerScript* ps = nullptr;
	for (auto& s : vs)
	{
		ps = dynamic_cast<CPlayerScript*>(s);
		if (nullptr != ps)
		{
			break;
		}
	}

	if (nullptr != ps)
	{
		ps->UnSetGround();
	}
}

void CPlatFormScript::SaveToFile(FILE* _File)
{
}

void CPlatFormScript::LoadFromFile(FILE* _File)
{
}

