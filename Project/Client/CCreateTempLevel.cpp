#include "pch.h"
#include "CCreateTempLevel.h"


#include <Engine/CCollisionMgr.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Scripts/CPlayerScript.h>
#include <Scripts/CBackgroundScript.h>

#include <Engine/CMesh.h>
#include <Engine/CGraphicsShader.h>
#include <Engine/CTexture.h>
#include <Engine/CSetColorShader.h>

#include "CLevelSaveLoad.h"
#include <Scripts/CCameraMoveScript.h>
#include <Scripts/CMissileScript.h>
#include <Scripts/CMonsterScript.h>

#include <Engine/CAssetMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CFSM.h>

#include "CIdleState.h"
#include "CTraceState.h"
#include <Scripts/CPlatFormScript.h>
#include <Engine\CDreamBlockParticleSystem.h>
#include <Scripts\CDreamBlockScript.h>
#include "Scripts\CMainCameraScript.h"
#include <Scripts\CTileMapScript.h>

void CCreateTempLevel::Init()
{
	// Missile Prefab 생성
	/*CGameObject* pObj = nullptr;

	pObj = new CGameObject;
	pObj->SetName(L"Missile");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CMissileScript);

	pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_Std2dMeterial));

	Ptr<CPrefab> pMissilePrefab = new CPrefab(pObj);
	CAssetMgr::GetInst()->AddAsset<CPrefab>(L"MissilePrefab", pMissilePrefab.Get());
	

	//pMissilePrefab->Save(L"prefab\\missile.pref");
	*/

	// 임시 FSM 객체 에셋 하나 생성하기
	Ptr<CFSM>	pFSM = new CFSM(true);

	pFSM->AddState(L"IdleState", new CIdleState);
	pFSM->AddState(L"TraceState", new CTraceState);

	CAssetMgr::GetInst()->AddAsset<CFSM>(L"NormalMonsterFSM", pFSM.Get());
}

void CCreateTempLevel::CreateTempLevel()
{		

	CLevel* pTempLevel = new CLevel;

	for (int i = 0; i < LAYER_END; ++i)
	{
		auto s = magic_enum::enum_name((LAYER)i);
		pTempLevel->GetLayer(i)->SetName(ToWString({ s.data(),s.size() }));
	}

	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::PLAY);

#pragma region Main Camera
	// Main Camera Object 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);
	pCamObj->AddComponent(new CMainCameraScript);

	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pCamObj->Camera()->SetCameraPriority(0);
	pCamObj->Camera()->LayerCheckAll();
	pCamObj->Camera()->LayerCheck(LAYER_SPCOL, false);
	pTempLevel->AddObject(pCamObj, LAYER_DEFAULT);

	CGameObject* pChidObj;
	pChidObj = nullptr;
#pragma endregion

#pragma region UI CAMERA
	// UI 카메라 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"UICamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(1);
	pCamObj->Camera()->LayerCheck(LAYER_UI, true);

	pTempLevel->AddObject(pCamObj, LAYER_DEFAULT);
#pragma endregion

#pragma region Directional Light
	// 전역 광원 추가
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"Directional Light");
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetAmbient(Vec3(1.f, 1.f, 1.f));
	pTempLevel->AddObject(pLight, LAYER_LIGHT);

	CGameObject* pObj = nullptr;
#pragma endregion

#pragma region DreamBlock
	//드림블럭
	pObj = new CGameObject;
	pObj->SetName(L"DreamBlock");
	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePos(Vec3(100.f, 0.f, 91.f));
	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
	pObj->AddComponent(new CCollider2D);
	pObj->Collider2D()->SetOffsetScale(Vec2(1, 1));
	pObj->AddComponent(new CDreamBlockParticleSystem);
	pObj->AddComponent(new CDreamBlockScript);
	pTempLevel->AddObject(pObj, LAYER_SPCOL);
	pObj = nullptr;
	pChidObj = nullptr;
#pragma endregion

#pragma region Background
	// Backgruond Object 생성
//pObj = new CGameObject;
//pObj->SetName(L"Background");
//pObj->AddComponent(new CTransform);
//pObj->AddComponent(new CMeshRender);
//pObj->AddComponent(new CBackgroundScript);
//pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 300.f));
//pObj->Transform()->SetRelativeScale(Vec3(1600.f, 800.f, 1.f));
//pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
//pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_BackGroundMeterial));
//Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Background.jpg", L"texture\\Background.png");
//pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
//pTempLevel->AddObject(pObj, LAYER_BACKGROUND, false);
#pragma endregion

#pragma region TileMap

	int TileNum = 0;
	pObj = new CGameObject;
	pObj->SetName(STR_KEY_TILEFAP);
	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 91.f));
	pObj->AddComponent(new CTileMapScript);
	pTempLevel->AddObject(pObj, LAYER_TILEMAP, false);

	pObj = nullptr;
	pChidObj = nullptr;
#pragma endregion

#pragma region Player Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Player");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CAnimator2D);
	pObj->AddComponent(new CPlayerScript);

	Vec3 playerscale = Vec3(32.f, 32.f, 1.f);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 90.f));
	pObj->Transform()->SetRelativeScale(playerscale);

	pObj->Collider2D()->SetAbsolute(false);
	pObj->Collider2D()->SetOffsetScale(Vec2(0.25f, 0.25f));
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(STR_KEY_RectMesh));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_Std2dMeterial));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Strawberry.png", L"texture\\Strawberry.png"));
	pTempLevel->AddObject(pObj, LAYER_PLAYER, false);
#pragma endregion

#pragma region Platform Object생성
//pObj = new CGameObject;
//pObj->SetName(L"Platform");
//
//pObj->AddComponent(new CTransform);
//pObj->AddComponent(new CMeshRender);
//pObj->AddComponent(new CCollider2D);
//pObj->AddComponent(new CAnimator2D);
//pObj->AddComponent(new CPlatFormScript);
//
//Vec3 scale = Vec3(324.f, 30.f, 1.f);
//pObj->Transform()->SetRelativePos(Vec3(0.f, -90.f, 90.f));
//pObj->Transform()->SetRelativeScale(scale);
//
//pObj->Collider2D()->SetAbsolute(false);
//pObj->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
//pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
//pObj->Collider2D()->SetColliderType(COLLIDER2D_TYPE::COL_TYPE_RECT);
//
//pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(STR_KEY_RectMesh));
//pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_DebugMeterial));
//pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Strawberry.png", L"texture\\Strawberry.png"));

//pTempLevel->AddObject(pObj, LAYER_TILECOL, false);
#pragma endregion

#pragma region 충돌설정
	// 충돌 설정
	CCollisionMgr::GetInst()->LayerCheck(LAYER_SPCOL, LAYER_PLAYER);
	CCollisionMgr::GetInst()->LayerCheck(LAYER_TILECOL, LAYER_PLAYER);
#pragma endregion


	//CLevelSaveLoad::SaveLevel(pTempLevel, L"level\\temp.lv");	
}