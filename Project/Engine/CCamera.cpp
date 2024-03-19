#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CTransform.h"

#include "CRenderMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CRenderComponent.h"

#include "CAssetMgr.h"


CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_FOV(XM_PI / 2.f)
	, m_Width(0.f)
	, m_Scale(1.f)
	, m_AspectRatio(1.f)
	, m_Far(10000.f)
	, m_LayerCheck(0)
	, m_CameraPriority(-1)
{
	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
	m_AspectRatio = vResol.x / vResol.y;
}

CCamera::~CCamera()
{
}


void CCamera::begin()
{
	// 카메라를 우선순위값에 맞게 RenderMgr 에 등록시킴
	CRenderMgr::GetInst()->RegisterCamera(this, m_CameraPriority);
}

void CCamera::finaltick()
{
	// 뷰 행렬을 계산한다.
	// 카메라를 원점으로 이동시키는 이동 행렬
	Vec3 vCamPos = Transform()->GetRelativePos();
	Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// 카메라의 각 우, 상, 전 방 방향을 기저축이랑 일치시키도록 회전하는 회전행렬
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vUp	= Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	Matrix matRotate = XMMatrixIdentity();
	matRotate._11 = vRight.x; matRotate._12 = vUp.x; matRotate._13 = vFront.x;
	matRotate._21 = vRight.y; matRotate._22 = vUp.y; matRotate._23 = vFront.y;
	matRotate._31 = vRight.z; matRotate._32 = vUp.z; matRotate._33 = vFront.z;

	// 이동 x 회전 = view 행렬
	m_matView = matTrans * matRotate;


	// 투영 방식에 따른 투영 행렬을 계산한다.
	m_matProj = XMMatrixIdentity();

	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// 직교투영
		Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
		m_matProj = XMMatrixOrthographicLH(vResol.x * m_Scale, (vResol.x / m_AspectRatio) * m_Scale, 1.f, m_Far);
	}
	else
	{
		// 원근투영
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}

	
}

void CCamera::SetCameraPriority(int _Priority)
{
	m_CameraPriority = _Priority;
}



void CCamera::LayerCheck(UINT _LayerIdx, bool _bCheck)
{	
	if (_bCheck)
	{
		m_LayerCheck |= (1 << _LayerIdx);
	}
	else
	{
		m_LayerCheck &= ~(1 << _LayerIdx);
	}
}

void CCamera::LayerCheck(const wstring& _strLayerName, bool _bCheck)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(_strLayerName);

	if (nullptr == pLayer)
		return;

	int idx = pLayer->GetLayerIdx();
	LayerCheck(idx, _bCheck);
}

void CCamera::SortObject()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (int i = 0; i < LAYER_MAX; ++i)
	{
		// 카메라가 찍도록 설정된 Layer 가 아니면 무시
		if (false == (m_LayerCheck & (1 << i)))
			continue;

		CLayer* pLayer = pCurLevel->GetLayer(i);
		const vector<CGameObject*>& vecObjects = pLayer->GetLayerObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			// 메쉬, 재질, 쉐이더 확인
			if (!( vecObjects[j]->GetRenderComopnent()
				&& vecObjects[j]->GetRenderComopnent()->GetMesh().Get()
				&& vecObjects[j]->GetRenderComopnent()->GetMaterial().Get()
				&& vecObjects[j]->GetRenderComopnent()->GetMaterial()->GetShader().Get()))
			{
				continue;
			}

			SHADER_DOMAIN domain = vecObjects[j]->GetRenderComopnent()->GetMaterial()->GetShader()->GetDomain();

			switch (domain)
			{
			case SHADER_DOMAIN::DOMAIN_OPAQUE:
				m_vecOpaque.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_MASKED:
				m_vecMaked.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
				m_vecTransparent.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
				m_vecPostProcess.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_DEBUG:
				break;
			}
		}
	}
}

void CCamera::render()
{
	// 계산한 view 행렬과 proj 행렬을 전역변수에 담아둔다.
	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;

	// Domain 순서대로 렌더링
	render(m_vecOpaque);	
	render(m_vecMaked);
	render(m_vecTransparent);

	// 후처리 작업
	render_postprocess();
}

void CCamera::render(vector<CGameObject*>& _vecObj)
{
	for (size_t i = 0; i < _vecObj.size(); ++i)
	{
		_vecObj[i]->render();
	}
	_vecObj.clear();
}

void CCamera::render_postprocess()
{
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		// 최종 렌더링 이미지를 후처리 타겟에 복사
		CRenderMgr::GetInst()->CopyRenderTargetToPostProcessTarget();

		// 복사받은 후처리 텍스쳐를 t13 레지스터에 바인딩
		Ptr<CTexture> pPostProcessTex = CRenderMgr::GetInst()->GetPostProcessTex();
		pPostProcessTex->UpdateData(13);

		// 후처리 오브젝트 렌더링
		m_vecPostProcess[i]->render();
	}

	m_vecPostProcess.clear();
}

void CCamera::SaveToFile(FILE* _File)
{
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_FOV, sizeof(float), 1, _File);
	fwrite(&m_Width, sizeof(float), 1, _File);
	fwrite(&m_Scale, sizeof(float), 1, _File);
	fwrite(&m_AspectRatio, sizeof(float), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _File);
	fwrite(&m_CameraPriority, sizeof(int), 1, _File);
}

void CCamera::LoadFromFile(FILE* _File)
{
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_FOV, sizeof(float), 1, _File);
	fread(&m_Width, sizeof(float), 1, _File);
	fread(&m_Scale, sizeof(float), 1, _File);
	fread(&m_AspectRatio, sizeof(float), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_LayerCheck, sizeof(UINT), 1, _File);
	fread(&m_CameraPriority, sizeof(int), 1, _File);
}
