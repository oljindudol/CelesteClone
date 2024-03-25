#include "pch.h"
#include "CPlatFormScript.h"

#include "CPhysics.h"

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

	// TODO: if (���̾� �������� or ������Ʈ Ÿ�� ��������?)

	float plattop = (ThisPos.y + ThisScale.y / 2.f);
	float otherprevbottom = (OtherPrevPos.y - OtherScale.y / 2.f);

	float yfix = 0.99f;//((UINT)LAYER::PLAYER == _OtherObj->GetLayerIdx()) ? 0.99f : 0.97f;

	//������ �ö������, y���ǵ带 0���� ����
	if (plattop > otherprevbottom)// *yfix)
	{
		CPhysics* phy = nullptr;
		auto vs = _OtherObj->GetScripts();
		for (auto& s : vs)
		{
			auto pp = s->GetPhysics();
			if (nullptr != pp)
			{
				phy = pp;
			}
		}

		if (nullptr != phy)
		{
			Vec3 v = phy->GetVelocity();
			phy->SetVelocity(Vec3(v.x, 0.f,0.f));
			phy->UseGravity(false);
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

	float yfix = 0.99f; // ((UINT)LAYER::PLAYER == _OtherObj->GetLayerIdx()) ? 0.99f : 0.95f;


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
}

void CPlatFormScript::SaveToFile(FILE* _File)
{
}

void CPlatFormScript::LoadFromFile(FILE* _File)
{
}

