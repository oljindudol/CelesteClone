#include "pch.h"
#include "CMainCameraScript.h"
#include "Engine\CLevelMgr.h"
#include <Engine\CTransitionParticleSystem.h>


CMainCameraScript::CMainCameraScript()
	: CScript((UINT)SCRIPT_TYPE::MAINCAMERASCRIPT)
{
}

CMainCameraScript::~CMainCameraScript()
{
}



void CMainCameraScript::begin()
{
	CGameObject* pChidObj = new CGameObject;
	pChidObj->SetName(L"Rain");
	pChidObj->AddComponent(new CTransform);
	pChidObj->Transform()->SetRelativePos(Vec3(1.f, 160.f, 130.f));
	pChidObj->AddComponent(new CAnimatedParticleSystem);
	GetOwner()->AddChild(pChidObj);
	GamePlayStatic::SpawnGameObject(pChidObj, LAYER_BACKEFFECT);

	pChidObj = new CGameObject;
	pChidObj->SetName(L"Transition");
	pChidObj->AddComponent(new CTransform);
	pChidObj->Transform()->SetRelativePos(Vec3(1.f, 0.f, 40.f));
	pChidObj->AddComponent(new CTransitionParticleSystem);
	GetOwner()->AddChild(pChidObj);
	GamePlayStatic::SpawnGameObject(pChidObj, LAYER_BACKEFFECT);

}

void CMainCameraScript::tick()
{
}

void CMainCameraScript::SaveToFile(FILE* _File)
{
}

void CMainCameraScript::LoadFromFile(FILE* _File)
{
}

