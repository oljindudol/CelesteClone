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
	// 전용 메쉬와 전용 재질 사용
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(STR_KEY_PointMesh));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_DreamParticleMeterial));

	// 렌더링 해상도
	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	// 파티클을 저장하는 구조화 버퍼
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tDreamParticle), m_MaxParticleCount, SB_TYPE::READ_WRITE, true);

	// 파티클 모듈정보를 저장하는 구조화버퍼
	m_ParticleModuleBuffer = new CStructuredBuffer;
	UINT ModuleAddSize = 0;
	if (sizeof(tDreamParticleModule) % 16 != 0)
	{
		ModuleAddSize = 16 - (sizeof(tDreamParticleModule) % 16);
	}
	m_ParticleModuleBuffer->Create(sizeof(tDreamParticleModule) + ModuleAddSize, 1, SB_TYPE::READ_ONLY, true);

	// 파티클 업데이트용 컴퓨트 쉐이더 참조
	m_CSParticleUpdate = (CDreamParticleUpdate*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(STR_KEY_DreamParticleUpdateShader).Get();


	// SpawnCount 전달용 구조화버퍼
	m_SpawnCountBuffer = new CStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::READ_WRITE, true);



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

	// 파티클 모듈정보 업데이트
	m_ParticleModuleBuffer->SetData(&m_Module);
	m_ParticleModuleBuffer->UpdateData_CS_SRV(20);

	// 파티클 업데이트 컴퓨트 쉐이더
	m_CSParticleUpdate->SetParticleBuffer(m_ParticleBuffer);
	m_CSParticleUpdate->SetParticleModuleBuffer(m_ParticleModuleBuffer);
	m_CSParticleUpdate->SetParticleSpawnCount(m_SpawnCountBuffer);
	m_CSParticleUpdate->SetParticleWorldPos(Transform()->GetWorldPos());
	m_CSParticleUpdate->Execute();

	if (0.f != DT)
	{
		CreateDreamFab();
	}
}


void CDreamBlockParticleSystem::render()
{

	// View, Proj 행렬 전달
	Transform()->UpdateData();

	// ParticleBuffer 바인딩
	m_ParticleBuffer->UpdateData(20);
	m_ParticleModuleBuffer->UpdateData(21);

	// 모든 파티클 렌더링
	// 파티클 개별 랜더링 -> 인스턴싱
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

	// 렌더링때 사용한 리소스 바인딩 Clear
	m_ParticleBuffer->Clear(20);
	m_ParticleModuleBuffer->Clear(21);
}

void CDreamBlockParticleSystem::begin()
{
	LoadDreamFab();
}

void CDreamBlockParticleSystem::CreateDreamFab()
{
	const static wstring extention = L".txt";
	// Level 을 저장할 경로
	wstring strFabPath = CPathMgr::GetContentPath();
	strFabPath += STR_KEY_DREAMFAB_PATH;
	strFabPath += GetOwner()->GetName();
	strFabPath += extention;


	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFabPath.c_str(), L"wb");

	//트랜스폼 정보저장
	auto rpos = Transform()->GetRelativePos();
	fwrite(&rpos, sizeof(Vec3), 1, pFile);

	//모듈 info 저장
	fwrite(&m_Module, sizeof(tDreamParticleModule), 1, pFile);

	fclose(pFile);
}

void CDreamBlockParticleSystem::LoadDreamFab()
{
	const static wstring extention = L".txt";
	// Level 을 저장할 경로
	wstring strFabPath = CPathMgr::GetContentPath();
	strFabPath += STR_KEY_DREAMFAB_PATH;
	strFabPath += GetOwner()->GetName();
	strFabPath += extention;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFabPath.c_str(), L"rb");
	//assert(pFile);

	//트랜스폼 정보 불러오기
	Vec3 rpos = Vec3();
	fread(&rpos, sizeof(Vec3), 1, pFile);
	Transform()->SetRelativePos(rpos);

	//모듈 info 불러오기
	fread(&m_Module, sizeof(tDreamParticleModule), 1, pFile);

	fclose(pFile);
}

void CDreamBlockParticleSystem::UpdateData()
{
}

void CDreamBlockParticleSystem::SaveToFile(FILE* _File)
{
	// 파티클 최대 갯수 및 모듈 정보 저장
	fwrite(&m_MaxParticleCount, sizeof(UINT), 1, _File);
	fwrite(&m_Module, sizeof(tDreamParticleModule), 1, _File);

	// 사용하던 CS 가 누군지 저장
	//SaveAssetRef(m_CSParticleUpdate, _File);

	// 파티클 입자 텍스쳐 정보 저장
	SaveAssetRef(m_ParticleTex, _File);
}

void CDreamBlockParticleSystem::LoadFromFile(FILE* _File)
{
	// 파티클 최대 갯수 및 모듈 정보 로드
	fread(&m_MaxParticleCount, sizeof(UINT), 1, _File);
	fread(&m_Module, sizeof(tDreamParticleModule), 1, _File);

	// 사용하던 CS 가 누군지 로드
	/*Ptr<CComputeShader> cs;
	LoadAssetRef(cs, _File);
	m_CSParticleUpdate = (CParticleUpdate*)cs.Get();*/

	// 파티클 입자 텍스쳐 정보 로드
	LoadAssetRef(m_ParticleTex, _File);
}


