#include "pch.h"
#include "CCustomParticleSystem.h"
#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CTimeMgr.h"

#include "CAssetMgr.h"
#include "CMesh.h"
#include "CMaterial.h"

CCustomParticleSystem::CCustomParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::CUSTOMPARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_MaxParticleCount(1000)
{
	// ���� �޽��� ���� ���� ���
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(STR_KEY_PointMesh));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_CustomParticleMeterial));

	// ������ �ػ�
	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	// ��ƼŬ�� �����ϴ� ����ȭ ����
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::READ_WRITE, true);

	// ��ƼŬ ��������� �����ϴ� ����ȭ����
	m_ParticleModuleBuffer = new CStructuredBuffer;
	UINT ModuleAddSize = 0;
	if (sizeof(tParticleModule) % 16 != 0)
	{
		ModuleAddSize = 16 - (sizeof(tParticleModule) % 16);
	}
	m_ParticleModuleBuffer->Create(sizeof(tParticleModule) + ModuleAddSize, 1, SB_TYPE::READ_ONLY, true);

	// ��ƼŬ ������Ʈ�� ��ǻƮ ���̴� ����
	m_CSParticleUpdate = (CParticleUpdate*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(STR_KEY_ParticleUpdateShader).Get();

	// SpawnCount ���޿� ����ȭ����
	m_SpawnCountBuffer = new CStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::READ_WRITE, true);


	// �ʱ� ��� ����		
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;

	m_Module.SpaceType = 1;
	m_Module.vSpawnColor = Vec4(1.f, 0.f, 0.f, 1.f);
	m_Module.vSpawnMinScale = Vec4(1.f, 1.f, 1.f, 1.f);
	m_Module.vSpawnMaxScale = Vec4(1.f, 1.f, 1.f, 1.f);
	m_Module.MinLife = 2.5f;
	m_Module.MaxLife = 2.5f;
	m_Module.MinMass = 1.f;
	m_Module.MaxMass = 1.f;
	m_Module.SpawnShape = 1; // 0 : Sphere, 1 : Box
	m_Module.Radius = 100.f;
	m_Module.vSpawnBoxScale = Vec4(10.f, 10.f, 0.f, 0.f);
	m_Module.SpawnRate = 38;

	// Add Velocity Module
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
	m_Module.AddVelocityType = 0; // 0 : From Center, 1: To Center, 2: Fix Direction
	m_Module.MinSpeed = 5;
	m_Module.MaxSpeed = 5;
	m_Module.FixedDirection;
	m_Module.FixedAngle;

	// Scale
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = 0;
	m_Module.vScaleRatio = Vec3(0.1f, 0.1f, 0.1f);

	// Noise Force
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 1;
	m_Module.NoiseForceScale = 5.f;
	m_Module.NoiseForceTerm = 0.3f;

	// Drag Module
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 1;
	m_Module.DragTime = 3.f;

	// Calculate Force
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 1;

	// Render 
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 1;
	m_Module.VelocityAlignment = 0; // �ӵ��� ���� ���� ����
	m_Module.AlphaBasedLife = 1; // 0 : off, 1 : NomrlizedAge, 2: Age
	m_Module.AlphaMaxAge = 2.8f;


	m_ParticleTex = CAssetMgr::GetInst()->Load<CTexture>(STR_FILE_PATH_PARTICLE_GENERAL
		, STR_FILE_PATH_PARTICLE_GENERAL);
}

CCustomParticleSystem::CCustomParticleSystem(const CCustomParticleSystem& _OriginParticle)
	: CRenderComponent(_OriginParticle)
	, m_ParticleBuffer(nullptr)
	, m_MaxParticleCount(_OriginParticle.m_MaxParticleCount)
	, m_Module(_OriginParticle.m_Module)
	, m_ParticleModuleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_CSParticleUpdate(_OriginParticle.m_CSParticleUpdate)
	, m_ParticleTex(_OriginParticle.m_ParticleTex)
	, m_Time(0.f)
{
	if (nullptr != _OriginParticle.m_ParticleBuffer)
		m_ParticleBuffer = _OriginParticle.m_ParticleBuffer->Clone();

	if (nullptr != _OriginParticle.m_ParticleModuleBuffer)
		m_ParticleModuleBuffer = _OriginParticle.m_ParticleModuleBuffer->Clone();

	if (nullptr != _OriginParticle.m_SpawnCountBuffer)
		m_SpawnCountBuffer = _OriginParticle.m_SpawnCountBuffer->Clone();
}

CCustomParticleSystem::~CCustomParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_ParticleModuleBuffer)
		delete m_ParticleModuleBuffer;

	if (nullptr != m_SpawnCountBuffer)
		delete m_SpawnCountBuffer;
}


void CCustomParticleSystem::finaltick()
{
	m_Time += DT;

	if ((1.f / m_Module.SpawnRate) < m_Time)
	{
		// ���� �ð��� ���� �������� ���� ��
		float fSpawnCount = m_Time / (1.f / m_Module.SpawnRate);

		// ���� ������ ������ �ܷ��� ���� �����ð����� ����
		m_Time -= (1.f / m_Module.SpawnRate) * floorf(fSpawnCount);

		tSpawnCount count = tSpawnCount{ (int)fSpawnCount, 0, 0, 0 };
		m_SpawnCountBuffer->SetData(&count);
	}
	else
	{
		tSpawnCount count = tSpawnCount{ 0, 0, 0, 0 };
		m_SpawnCountBuffer->SetData(&count);
	}


	// ��ƼŬ ������� ������Ʈ
	m_ParticleModuleBuffer->SetData(&m_Module);
	m_ParticleModuleBuffer->UpdateData_CS_SRV(20);

	// ��ƼŬ ������Ʈ ��ǻƮ ���̴�
	m_CSParticleUpdate->SetParticleBuffer(m_ParticleBuffer);
	m_CSParticleUpdate->SetParticleModuleBuffer(m_ParticleModuleBuffer);
	m_CSParticleUpdate->SetParticleSpawnCount(m_SpawnCountBuffer);
	m_CSParticleUpdate->SetParticleWorldPos(Transform()->GetWorldPos());
	m_CSParticleUpdate->Execute();
}

void CCustomParticleSystem::render()
{
	// View, Proj ��� ����
	Transform()->UpdateData();

	// ParticleBuffer ���ε�
	m_ParticleBuffer->UpdateData(20);
	m_ParticleModuleBuffer->UpdateData(21);

	// ��� ��ƼŬ ������
	// ��ƼŬ ���� ������ -> �ν��Ͻ�
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_ParticleTex);
	GetMaterial()->UpdateData();

	GetMesh()->render_asparticle(m_MaxParticleCount);

	// �������� ����� ���ҽ� ���ε� Clear
	m_ParticleBuffer->Clear(20);
	m_ParticleModuleBuffer->Clear(21);
}

void CCustomParticleSystem::UpdateData()
{

}

void CCustomParticleSystem::SaveToFile(FILE* _File)
{
	// ��ƼŬ �ִ� ���� �� ��� ���� ����
	fwrite(&m_MaxParticleCount, sizeof(UINT), 1, _File);
	fwrite(&m_Module, sizeof(tParticleModule), 1, _File);

	// ����ϴ� CS �� ������ ����
	//SaveAssetRef(m_CSParticleUpdate, _File);

	// ��ƼŬ ���� �ؽ��� ���� ����
	SaveAssetRef(m_ParticleTex, _File);
}

void CCustomParticleSystem::LoadFromFile(FILE* _File)
{
	// ��ƼŬ �ִ� ���� �� ��� ���� �ε�
	fread(&m_MaxParticleCount, sizeof(UINT), 1, _File);
	fread(&m_Module, sizeof(tParticleModule), 1, _File);

	// ����ϴ� CS �� ������ �ε�
	/*Ptr<CComputeShader> cs;
	LoadAssetRef(cs, _File);
	m_CSParticleUpdate = (CParticleUpdate*)cs.Get();*/

	// ��ƼŬ ���� �ؽ��� ���� �ε�
	LoadAssetRef(m_ParticleTex, _File);
}


