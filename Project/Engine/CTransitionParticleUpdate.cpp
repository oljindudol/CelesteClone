#include "pch.h"
#include "CTransitionParticleUpdate.h"


CTransitionParticleUpdate::CTransitionParticleUpdate()
	: CComputeShader(32, 1, 1)
{
	Create(STR_FILE_PATH_ParticleUpdateShader, STR_FUNC_NAME_CSShaderParticleUpdate);
}

CTransitionParticleUpdate::~CTransitionParticleUpdate()
{
}
int CTransitionParticleUpdate::UpdateData()
{
	if (nullptr == m_ParticleBuffer)
		return E_FAIL;

	m_Const.iArr[0] = m_ParticleBuffer->GetElementCount();
	m_Const.v4Arr[0] = m_vParticleWorldPos;

	m_ParticleBuffer->UpdateData_CS_UAV(0);
	m_SpawnCountBuffer->UpdateData_CS_UAV(1);

	m_ParticleModuleBuffer->UpdateData_CS_SRV(20);

	return S_OK;
}

void CTransitionParticleUpdate::UpdateGroupCount()
{
	UINT GroupX = (m_ParticleBuffer->GetElementCount() / m_ThreadX);
	if (0 != m_ParticleBuffer->GetElementCount() % m_ThreadX)
		GroupX += 1;

	SetGroup(GroupX, 1, 1);
}

void CTransitionParticleUpdate::Clear()
{
	m_ParticleBuffer->Clear_CS_UAV();
	m_ParticleBuffer = nullptr;

	m_ParticleModuleBuffer->Clear_CS_SRV();
	m_ParticleModuleBuffer = nullptr;

	m_SpawnCountBuffer->Clear_CS_UAV();
	m_SpawnCountBuffer = nullptr;
}


