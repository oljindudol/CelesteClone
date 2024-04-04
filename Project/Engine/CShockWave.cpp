#include "pch.h"
#include "CShockWave.h"
#include "CTimeMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CRenderMgr.h"

CShockWave::CShockWave()
	:CRenderComponent(COMPONENT_TYPE::SHOCKWAVE)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_ShockWaveMeterial));
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_03.jpg"));
}

CShockWave::~CShockWave()
{
}

void CShockWave::finaltick()
{
	static const float accel = 1.5f;

	static const float ShockDuration = 1.f  ;

	for (auto iter = m_ShockEventList.begin(); iter != m_ShockEventList.end(); )
	{
		iter->AccTime = iter->AccTime + DT * accel;

		if (ShockDuration < iter->AccTime)
		{
			iter = m_ShockEventList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CShockWave::UpdateData()
{
	if (nullptr != GetMaterial())
	{
		GetMaterial()->UpdateData();
	}
}

void CShockWave::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	Animator2D()->Clear();

	UpdateData();


	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::SHOCKWAVE_DATA);
	for (auto& e: m_ShockEventList)
	{
		// 최종 렌더링 이미지를 후처리 타겟에 복사
		if (0.1f > e.AccTime)
		{
			continue;
		}

		CRenderMgr::GetInst()->CopyRenderTargetToPostProcessTarget();

		tShock s = {};
		s.ShockAccTime = e.AccTime;

		Transform()->UpdateData(e.ShockMat);

		pCB->SetData(&s);
		pCB->UpdateData();
		GetMesh()->render();
	}


}

