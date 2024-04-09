#include "pch.h"
#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CTimeMgr.h"

#include "CAssetMgr.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CDreamBlockParticleSystem.h"
#include "CRenderMgr.h"

CDreamBlockParticleSystem::CDreamBlockParticleSystem() :
	CRenderComponent(COMPONENT_TYPE::DREAMBLOCKPARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_MaxParticleCount(1000)
{
	// ���� �޽��� ���� ���� ���
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(STR_KEY_PointMesh));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_DreamParticleMeterial));

	// ������ �ػ�
	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	// ��ƼŬ�� �����ϴ� ����ȭ ����
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tDreamParticle), m_MaxParticleCount, SB_TYPE::READ_WRITE, true);

	// ��ƼŬ ��������� �����ϴ� ����ȭ����
	m_ParticleModuleBuffer = new CStructuredBuffer;
	UINT ModuleAddSize = 0;
	if (sizeof(tDreamParticleModule) % 16 != 0)
	{
		ModuleAddSize = 16 - (sizeof(tDreamParticleModule) % 16);
	}
	m_ParticleModuleBuffer->Create(sizeof(tDreamParticleModule) + ModuleAddSize, 1, SB_TYPE::READ_ONLY, true);

	// ��ƼŬ ������Ʈ�� ��ǻƮ ���̴� ����
	m_CSParticleUpdate = (CDreamParticleUpdate*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(STR_KEY_DreamParticleUpdateShader).Get();


	// SpawnCount ���޿� ����ȭ����
	m_SpawnCountBuffer = new CStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::READ_WRITE, true);


	// �ʱ� ��� ����		
	m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;

	//m_Module.vSpawnColor = Vec4(1.f, 1.f, 1.f, 1.f);
	//m_Module.vSpawnMinScale = Vec4(7.f, 7.f, 1.f, 1.f);
	//m_Module.vSpawnMaxScale = Vec4(7.f, 7.f, 1.f, 1.f);
	//m_Module.vSpawnBoxScale = Vec4(1000.f, 1000.f, 0.f, 0.f);
	//m_Module.SpawnCount = 500.f;

	// Noise Force
	m_Module.arrModuleCheck[(UINT)DREAM_PARTICLE_MODULE::NOISE_FORCE] = 0;
	m_Module.NoiseForceScale = 5.f;
	m_Module.NoiseForceTerm = 0.3f;

	m_ParticleTex = CAssetMgr::GetInst()->Load<CTexture>(STR_FILE_PATH_DREAMBLOCK_PARTICLE
		, STR_FILE_PATH_DREAMBLOCK_PARTICLE);

	m_Module.FrameDuration = 0.3f;
}

CDreamBlockParticleSystem::CDreamBlockParticleSystem(const CDreamBlockParticleSystem& _OriginParticle)
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

CDreamBlockParticleSystem::~CDreamBlockParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_ParticleModuleBuffer)
		delete m_ParticleModuleBuffer;

	if (nullptr != m_SpawnCountBuffer)
		delete m_SpawnCountBuffer;
}

void CDreamBlockParticleSystem::finaltick()
{	

	//if (true == m_bDebug)
	//{
	//	m_bThisFrameGenerate = true;
	//}

	m_Time += DT;


	tSpawnCount count = {};
	if (true == m_bThisFrameDelete)
	{
		count = tSpawnCount{ -1, 0, 0, 0 };
		m_bThisFrameDelete = false;
	}
	else if (false == m_bThisFrameDelete && true == m_bThisFrameGenerate)
	{
		count = tSpawnCount{ (int)m_Module.SpawnCount, 0, 0, 0 };
		m_bThisFrameGenerate = false;
	}
	else
	{

	}


	m_SpawnCountBuffer->SetData(&count);

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


void CDreamBlockParticleSystem::render()
{

	// View, Proj ��� ����
	Transform()->UpdateData();

	// ParticleBuffer ���ε�
	m_ParticleBuffer->UpdateData(20);
	m_ParticleModuleBuffer->UpdateData(21);

	// ��� ��ƼŬ ������
	// ��ƼŬ ���� ������ -> �ν��Ͻ�
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	auto pos = Transform()->GetWorldPos();
	auto scale = Transform()->GetWorldScale();
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4( pos.x,pos.y,0.f,0.f));
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_1, Vec4(scale.x,scale.y, scale.z,0.f));
	auto camerapos = CRenderMgr::GetInst()->GetCameraPos();
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_2, Vec4(camerapos.x, camerapos.y, camerapos.z, 0.f));
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_ParticleTex);
	//GetMaterial()->SetTexParam(TEX_PARAM::TEXARR_0, m_ParticleArrTex);
	GetMaterial()->UpdateData();

	GetMesh()->render_asparticle(m_MaxParticleCount);

	// �������� ����� ���ҽ� ���ε� Clear
	m_ParticleBuffer->Clear(20);
	m_ParticleModuleBuffer->Clear(21);
}

void CDreamBlockParticleSystem::UpdateData()
{
}

void CDreamBlockParticleSystem::SaveToFile(FILE* _File)
{
	// ��ƼŬ �ִ� ���� �� ��� ���� ����
	fwrite(&m_MaxParticleCount, sizeof(UINT), 1, _File);
	fwrite(&m_Module, sizeof(tDreamParticleModule), 1, _File);

	// ����ϴ� CS �� ������ ����
	//SaveAssetRef(m_CSParticleUpdate, _File);

	// ��ƼŬ ���� �ؽ��� ���� ����
	SaveAssetRef(m_ParticleTex, _File);
}

void CDreamBlockParticleSystem::LoadFromFile(FILE* _File)
{
	// ��ƼŬ �ִ� ���� �� ��� ���� �ε�
	fread(&m_MaxParticleCount, sizeof(UINT), 1, _File);
	fread(&m_Module, sizeof(tDreamParticleModule), 1, _File);

	// ����ϴ� CS �� ������ �ε�
	/*Ptr<CComputeShader> cs;
	LoadAssetRef(cs, _File);
	m_CSParticleUpdate = (CParticleUpdate*)cs.Get();*/

	// ��ƼŬ ���� �ؽ��� ���� �ε�
	LoadAssetRef(m_ParticleTex, _File);
}


