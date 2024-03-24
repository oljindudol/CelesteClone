#include "pch.h"
#include "CEditorObjMgr.h"

#include "CGameObjectEx.h"
#include <Engine/components.h>
#include <Scripts/CCameraMoveScript.h>

#include <Engine/CRenderMgr.h>

CEditorObjMgr::CEditorObjMgr()
{

}

CEditorObjMgr::~CEditorObjMgr()
{
	Delete_Vec(m_vecEditorObj);
}

void CEditorObjMgr::init()
{
	// 에디터용 카메라 오브젝트 생성
	CGameObjectEx* pEditorCam = new CGameObjectEx;
	pEditorCam->AddComponent(new CTransform);
	pEditorCam->AddComponent(new CCamera);
	pEditorCam->AddComponent(new CCameraMoveScript);

	pEditorCam->SetName(STR_Editor_OBJ_NAME_EditorCamera);
	
	pEditorCam->Camera()->LayerCheckAll();
	pEditorCam->Camera()->LayerCheck(31, false);
	pEditorCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	//pEditorCam->Camera()->SetFOV(XM_PI / 2.f);
	pEditorCam->Camera()->SetFar(1000.f);
	pEditorCam->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	//해상도에따라 0.25f or 0.1666666f
	pEditorCam->Camera()->SetScale(.25f);



	m_vecEditorObj.push_back(pEditorCam);

	// Editor 용 카메라로서 렌더매니저에 등록
	CRenderMgr::GetInst()->RegisterEditorCamera(pEditorCam->Camera());
}

void CEditorObjMgr::progress()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->tick();
	}

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->finaltick();
	}
}
