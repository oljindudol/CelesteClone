#include "pch.h"
#include "CPlayerScript.h"
#include <Engine/CAssetMgr.h>

#include <Engine/CGameObject.h>
#include "CMissileScript.h"

#include <Engine/CMaterial.h>
#include <Engine/CRenderComponent.h>

#include "CPhysics.h"

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
	, m_Speed(100.f)
{
}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::begin()
{
	string OriginPath = "C:\\CelesteClone\\OutputFile\\content\\texture\\Gameplay\\madeline";
	auto folders = getFoldersFromDirectory(OriginPath);
	
	//auto ty = Transform()->GetRelativeScale().y;
	for (auto& f : folders)
	{
		auto rel = std::filesystem::relative(f, OriginPath);

		Animator2D()->CreateFromFolder(rel,f, 12.5 , Vec2(0.f, -0.375f) );
	}


	GetRenderComponent()->GetDynamicMaterial();

	auto a = Animator2D();
	Animator2D()->Play(L"idle");


	SetPhysics(new CPhysics(GetOwner()));

}

void CPlayerScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();


	auto a = Animator2D();
	if (KEY_PRESSED(KEY::UP))
		vPos.y += DT * m_Speed;	
	if (KEY_TAP(KEY::UP))
		Animator2D()->Play(L"MOVE_UP");
	if (KEY_RELEASED(UP))
		Animator2D()->Play(L"IDLE_UP");
	
	if (KEY_PRESSED(KEY::DOWN))
		vPos.y -= DT * m_Speed;
	if (KEY_TAP(KEY::DOWN))
		Animator2D()->Play(L"MOVE_DOWN");
	if (KEY_RELEASED(DOWN))
		Animator2D()->Play(L"IDLE_DOWN");

	if (KEY_PRESSED(KEY::LEFT))
		vPos.x -= DT * m_Speed;
	if (KEY_TAP(KEY::LEFT))
		Animator2D()->Play(L"MOVE_LEFT");
	if (KEY_RELEASED(LEFT))
		Animator2D()->Play(L"IDLE_LEFT");

	if (KEY_PRESSED(KEY::RIGHT))
		vPos.x += DT * m_Speed;
	if (KEY_TAP(KEY::RIGHT))
		Animator2D()->Play(L"MOVE_RIGHT");
	if (KEY_RELEASED(RIGHT))
		Animator2D()->Play(L"IDLE_RIGHT");

	if (KEY_PRESSED(KEY::X))
	{
		vRot.x += DT * XM_PI;
	}

	if (KEY_PRESSED(KEY::Y))
	{
		vRot.y += DT * XM_PI;
	}

	if (KEY_PRESSED(KEY::Z))
	{
		vRot.z += DT * XM_PI;
	}
	
	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRot);

	if (KEY_TAP(KEY::SPACE))
	{
		Instantiate(m_Missile, Transform()->GetWorldPos(), 0);
		//GamePlayStatic::Play2DSound(L"sound\\DM.wav", 1, 0.5f, false);
		GamePlayStatic::Play2DBGM(L"sound\\DM.wav", 0.5f);
	}

	if (KEY_PRESSED(KEY::SPACE))
	{
		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
		if (nullptr != pMtrl)
		{
			pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, 1);
		}
	}
	else if (KEY_RELEASED(KEY::SPACE))
	{
		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
		if (nullptr != pMtrl)
		{
			pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		}
	}

	GetPhysics()->Update();

	//static float f = 0.f;
	//f += DT * 0.3f;
	//GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_1, f);

	//GamePlayStatic::DrawDebugRect(Vec3(0.f, 0.f, 0.f), Vec3(200.f, 200.f, 1.f), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), true, 20);
	//GamePlayStatic::DrawDebugCircle(Vec3(0.f, 0.f, 0.f), 200.f, Vec3(0.f, 1.f, 1.f), true);
}

void CPlayerScript::BeginOverlap(CCollider2D* _Collider
	, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	Ptr<CMaterial> pMtrl = GetRenderComponent()->GetDynamicMaterial();
}

void CPlayerScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	
}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}
