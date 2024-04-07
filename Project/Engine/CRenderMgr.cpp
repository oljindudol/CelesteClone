#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CStructuredBuffer.h"

#include "CTimeMgr.h"
#include "CDevice.h"
#include "CAssetMgr.h"
#include "components.h"


CRenderMgr::CRenderMgr()
	: m_Light2DBuffer(nullptr)
	, m_pDebugObj(nullptr)
	, m_DebugPosition(true)
	, m_EditorCam(nullptr)
	, m_RenderFunc(nullptr)
{	
	m_RenderFunc = &CRenderMgr::render_play;
}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_pDebugObj)
		delete m_pDebugObj;

	if (nullptr != m_Light2DBuffer)
		delete m_Light2DBuffer;
}

void CRenderMgr::tick()
{
	// 렌더타겟 및 깊이 타겟 설정
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());
	
	static const Vec4 White = Vec4(1.f, 1.f, 1.f, 1.f);
	static const Vec4 Black = Vec4(0.f, 0.f, 0.f, 1.f);
	static const Vec4 Red = Vec4(1.f, 1.f, 1.f, 1.f);
	static const Vec4 Green = Vec4(0.f, 1.f, 0.f, 1.f);
	static const Vec4 Blue = Vec4(0.f, 0.f, 1.f, 1.f);
	static const Vec4 Yellow = Vec4(1.f, 1.f, 0.f, 1.f);
	static const Vec4 Mazenta = Vec4(1.f, 0.f, 1.f, 1.f);

	//Vec4 vClearColor = Vec4(0.3f, 0.3f, 0.3f, 1.f);
	CDevice::GetInst()->ClearRenderTarget(Black);

	UpdateData();

	(this->*m_RenderFunc)();
	render_debug();

	Clear();
}

void CRenderMgr::DirectionalShake(Vec2 _dir, float _time)
{
	if (0 != m_vecCam.size())
	{
		_dir.Normalize();
		m_vecCam[0]->m_ShakeDirection = _dir;
		m_vecCam[0]->m_LastDirectionalShake = 0;
		m_vecCam[0]->m_ShakeTimer = max(m_vecCam[0]->m_ShakeTimer, _time);
	}
}

void CRenderMgr::render_play()
{
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->SortObject();
		m_vecCam[i]->render();
	}
}

void CRenderMgr::render_editor()
{
	if (nullptr == m_EditorCam)
		return;

	m_EditorCam->SortObject();
	m_EditorCam->render();
}

void CRenderMgr::render_debug()
{
	if (m_vecCam.empty())
		return;

	if (&CRenderMgr::render_play == m_RenderFunc)
	{
		g_Transform.matView = m_vecCam[0]->GetViewMat();
		g_Transform.matProj = m_vecCam[0]->GetProjMat();
	}
	else
	{
		g_Transform.matView = m_EditorCam->GetViewMat();
		g_Transform.matProj = m_EditorCam->GetProjMat();
	}

	list<tDebugShapeInfo>::iterator iter = m_DbgShapeInfo.begin();
	for (; iter != m_DbgShapeInfo.end(); )
	{
		switch ((*iter).eShape)
		{
		case DEBUG_SHAPE::RECT:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(STR_KEY_RectMesh_Debug));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(STR_KEY_CircleMesh_Debug));
			break;
		case DEBUG_SHAPE::CROSS:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(STR_KEY_CrossMesh));
			break;

		case DEBUG_SHAPE::CUBE:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));
			break;
		case DEBUG_SHAPE::SPHERE:
			m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
			break;
		default:
			break;
		}

		m_pDebugObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_DebugMeterial));
		m_pDebugObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, (*iter).vColor);

		D3D11_PRIMITIVE_TOPOLOGY PrevTopology = m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->GetTopology();
		if (DEBUG_SHAPE::CROSS == (*iter).eShape)
		{
			m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		}

		m_pDebugObj->Transform()->SetWorldMat((*iter).matWorld);
		m_pDebugObj->Transform()->UpdateData();

		m_pDebugObj->render();

		m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(PrevTopology);

		(*iter).fLifeTime += DT;
		if ((*iter).fDuration <= (*iter).fLifeTime)
		{
			iter = m_DbgShapeInfo.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CRenderMgr::UpdateData()
{
	g_global.g_Light2DCount = (int)m_vecLight2D.size();
	//g_global.g_Light3DCount = (int)m_vecLight3D.size();

	// 전역 데이터 업데이트
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL_DATA);
	pCB->SetData(&g_global);

	pCB->UpdateData();
	pCB->UpdateData_CS();

	// 2D 광원정보 업데이트
	static vector<tLightInfo> vecLight2DInfo;

	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		const tLightInfo& info = m_vecLight2D[i]->GetLightInfo();
		vecLight2DInfo.push_back(info);
	}

	if (!vecLight2DInfo.empty())
	{
		m_Light2DBuffer->SetData(vecLight2DInfo.data(), (UINT)vecLight2DInfo.size());
	}	
	m_Light2DBuffer->UpdateData(11);

	vecLight2DInfo.clear();

	// 3D 광원정보 업데이트
}

void CRenderMgr::Clear()
{
	m_vecLight2D.clear();
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _Idx)
{
	if (m_vecCam.size() <= _Idx + 1)
	{
		m_vecCam.resize(_Idx + 1);
	}

	// 동일한 우선순위의 카메라가 이미 존재하면 assert
	assert(nullptr == m_vecCam[_Idx]);

	m_vecCam[_Idx] = _Cam;
}


