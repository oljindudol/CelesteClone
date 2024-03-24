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
#include <Scripts/CMissileScript.h>
#include <Scripts/CMonsterScript.h>

#include <Engine/CAssetMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CFSM.h>

#include "CIdleState.h"
#include "CTraceState.h"


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
	/*Ptr<CMaterial> pBackgroudMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_BackGroundMeterial);
	Ptr<CMaterial> pStd2DMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_Std2dMeterial);

	pBackgroudMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Background.jpg", L"texture\\Background.jpg"));
	pStd2DMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Fighter.bmp", L"texture\\Fighter.bmp"));*/

	/*CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\temp.lv");
	CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::STOP);
	return;*/

	CLevel* pTempLevel = new CLevel;

	for (int i = 0; i < LAYER_END; ++i)
	{
		auto s = magic_enum::enum_name((LAYER)i);
		pTempLevel->GetLayer(i)->SetName(ToWString({ s.data(),s.size() }));
	}

	//pTempLevel->GetLayer(LAYER_DEFAULT)->SetName(LAYER_DEFAULT);
	//pTempLevel->GetLayer(LAYER_BACKGROUND)->SetName(LAYER_BACKGROUND);
	//pTempLevel->GetLayer(LAYER_BACKTILE)->SetName(LAYER_BACKTILE);
	//pTempLevel->GetLayer(LAYER_TILECOL)->SetName(LAYER_TILECOL);
	//pTempLevel->GetLayer(LAYER_FORETILE)->SetName(LAYER_FORETILE);
	//pTempLevel->GetLayer(LAYER_PLAYER)->SetName(LAYER_PLAYER);
	//pTempLevel->GetLayer(LAYER_MONSTER)->SetName(LAYER_MONSTER);
	//pTempLevel->GetLayer(LAYER_LIGHT)->SetName(LAYER_LIGHT);
	//pTempLevel->GetLayer(LAYER_UI)->SetName(LAYER_UI);




	// ComputeShader 테스트
	// 사용할 텍스쳐 생성
	Ptr<CTexture> pTestTex = CAssetMgr::GetInst()->CreateTexture(L"TestTex"
		, 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);

	Ptr<CSetColorShader> pCS = (CSetColorShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(STR_KEY_SetColorShader).Get();
	pCS->SetColor(Vec3(1.f, 0.f, 0.f));
	pCS->SetTargetTexture(pTestTex);
	pCS->Execute();

	tPixel* pPixel = pTestTex->GetPixels();
	tPixel pixel = pPixel[pTestTex->GetWidth() * 1 + 5];

	// Main Camera Object 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pCamObj->Camera()->SetCameraPriority(0);
	pCamObj->Camera()->LayerCheckAll();
	pCamObj->Camera()->LayerCheck(31, false);

	pTempLevel->AddObject(pCamObj, LAYER_DEFAULT);

	// UI 카메라 생성
	pCamObj = new CGameObject;
	pCamObj->SetName(L"UICamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(1);
	pCamObj->Camera()->LayerCheck(31, true);

	pTempLevel->AddObject(pCamObj, LAYER_DEFAULT);

	// 전역 광원 추가
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"Directional Light");
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetAmbient(Vec3(0.8f, 0.8f, 0.8f));
	pTempLevel->AddObject(pLight, LAYER_LIGHT);


	CGameObject* pObj = nullptr;

	// Backgruond Object 생성
	//pObj = new CGameObject;
	//pObj->SetName(L"Background");

	//pObj->AddComponent(new CTransform);
	//pObj->AddComponent(new CMeshRender);
	//pObj->AddComponent(new CBackgroundScript);

	//pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 600.f));
	//pObj->Transform()->SetRelativeScale(Vec3(1600.f, 800.f, 1.f));

	//pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_BackGroundMeterial));

	//Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Background.jpg", L"texture\\Background.jpg");
	//pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);

	//pTempLevel->AddObject(pObj, L"Background", false);


	// TileMap Object
	pObj = new CGameObject;
	pObj->SetName(L"TileMap");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CTileMap);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 91.f));

	//Ptr<CTexture> pTileAtlas = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\snow.png", L"texture\\snow.png");
	//pObj->TileMap()->SetTileAtlas(pTileAtlas);

	//pTileAtlas = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\girder.png", L"texture\\girder.png");
	//pObj->TileMap()->SetTileAtlas(pTileAtlas);

	string ForeTilePath = "C:\\CelesteClone\\OutputFile\\content\\texture\\Gameplay\\tilemap\\backtile";
	auto files = getImagesFromDirectory(ForeTilePath);

	static const std::filesystem::path base_path = CPathMgr::GetContentPath();

	for (auto& p : files)
	{
		std::filesystem::path relative_path = std::filesystem::relative(p, base_path);
		Ptr<CTexture> pAtlasTex = CAssetMgr::GetInst()->Load<CTexture>(relative_path, relative_path);
		pObj->TileMap()->SetTileAtlas(pAtlasTex);
	}


	Ptr<CTexture> pTileAtlas = CAssetMgr::GetInst()->CreateArrayTexture(L"TileMapTextureArray", pObj->TileMap()->GetTileAtlases() , 1);
	pObj->TileMap()->SetArrAtlas(pTileAtlas);


	pTempLevel->AddObject(pObj, LAYER_FORETILE, false);
	pObj->TileMap()->SetRowCol(40, 23);
	//for (int i = 0; i < 23; ++i)
	//{
	//	for (int j = 0; j < 40; ++j)
	//	{
	//		pObj->TileMap()->SetTileIndex(i, j, i * 40 + j);
	//	}
	//}

	// Player Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Player");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CAnimator2D);
	pObj->AddComponent(new CPlayerScript);
	pObj->AddComponent(new CMissileScript);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 90.f));
	pObj->Transform()->SetRelativeScale(Vec3(32.f, 32.f, 1.f));

	pObj->Collider2D()->SetAbsolute(true);
	pObj->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(STR_KEY_RectMesh));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_Std2dMeterial));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Strawberry.png", L"texture\\Strawberry.png"));

	pTempLevel->AddObject(pObj, LAYER_PLAYER, false);


	// Monster Object 생성
	//pObj = new CGameObject;
	//pObj->SetName(L"Monster");

	//pObj->AddComponent(new CTransform);
	//pObj->AddComponent(new CMeshRender);
	//pObj->AddComponent(new CCollider2D);
	//pObj->AddComponent(new CStateMachine);
	//pObj->AddComponent(new CMonsterScript);

	//pObj->Transform()->SetRelativePos(Vec3(500.f, 0.f, 500.f));
	//pObj->Transform()->SetRelativeScale(Vec3(32.f, 32.f, 1.f));

	//pObj->Collider2D()->SetAbsolute(true);
	//pObj->Collider2D()->SetOffsetScale(Vec2(120.f, 120.f));
	//pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));

	//pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(STR_KEY_RectMesh));
	//pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_Std2dMeterial));
	//pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\badline.png", L"texture\\badline.png"));

	//pObj->StateMachine()->SetFSM(CAssetMgr::GetInst()->FindAsset<CFSM>(L"NormalMonsterFSM"));

	//pTempLevel->AddObject(pObj,LAYER_MONSTER, false);


	// Platform Object 생성
	pObj = new CGameObject;
	pObj->SetName(L"Platform");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CAnimator2D);

	Vec3 scale = Vec3(32.f, 32.f, 1.f);
	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 90.f));
	pObj->Transform()->SetRelativeScale(Vec3(32.f, 32.f, 1.f));

	pObj->Collider2D()->SetAbsolute(true);
	pObj->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(STR_KEY_RectMesh));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_DebugMeterial));
	//pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Strawberry.png", L"texture\\Strawberry.png"));

	pTempLevel->AddObject(pObj, LAYER_TILECOL, false);


	// 충돌 설정
	CCollisionMgr::GetInst()->LayerCheck(3, 4);
	CCollisionMgr::GetInst()->LayerCheck(4, 4);

	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::STOP);

	CLevelSaveLoad::SaveLevel(pTempLevel, L"level\\temp.lv");	
}