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

	//m_LineMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(STR_KEY_RectMesh);
	//m_LineMat = CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_Std2dMeterial);
	m_LineMat = CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_DrawLineMeterial);

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
	UpdateParallaxCorrection();
	m_Time += DT;


	tSpawnCount count = {};
	if (true == m_bThisFrameDelete)
	{
		count = tSpawnCount{ -1, 0, 0, 0 };
		m_bThisFrameDelete = false;
	}
	else if (false == m_bThisFrameDelete && true == m_bThisFrameGenerate)
	{
		count = tSpawnCount{ m_Module.SpawnCount, 0, 0, 0 };
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
		//CreateDreamFab();
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
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4( pos.x,pos.y, pos.z,0.f));
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_1, Vec4(scale.x,scale.y, scale.z,0.f));
	auto camerapos = CRenderMgr::GetInst()->GetCameraPos();
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_2, Vec4(camerapos.x, camerapos.y, camerapos.z, 0.f));

	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_ParticleTex);
	GetMaterial()->UpdateData();

	//GetMesh()->render();
	GetMesh()->render_asparticle(m_MaxParticleCount);

	// 렌더링때 사용한 리소스 바인딩 Clear
	m_ParticleBuffer->Clear(20);
	m_ParticleModuleBuffer->Clear(21);

	float left = pos.x - scale.x * 0.5f;
	float right = pos.x + scale.x * 0.5f;
	float up = pos.y + scale.y * 0.5f;
	float bottom = pos.y - scale.y * 0.5f;


	WoolbeUpdate();

	const float linewidth = 1.f;

	//Line Color
	m_LineMat->SetScalarParam(SCALAR_PARAM::VEC4_2, Vec4(1.f, 1.f, 1.f, 1.f));
	//Line Width
	m_LineMat->SetScalarParam(SCALAR_PARAM::FLOAT_0, linewidth);
	//Wooble Line
	m_LineMat->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	//Wooble from
	m_LineMat->SetScalarParam(SCALAR_PARAM::FLOAT_1, wobbleFrom);
	//Wooble To
	m_LineMat->SetScalarParam(SCALAR_PARAM::FLOAT_2, wobbleTo);
	//Wooble Ease
	m_LineMat->SetScalarParam(SCALAR_PARAM::FLOAT_2, wobbleEase);

	static vector<float> v1from;
	static vector<float> v1to1;
	static vector<float> v1to2;

	static vector<float> v2from;
	static vector<float> v2to1;
	static vector<float> v2to2;

	static vector<float> v3from;
	static vector<float> v3to1;
	static vector<float> v3to2;

	static vector<float> v4from;
	static vector<float> v4to1;
	static vector<float> v4to2;
	m_randomtime += DT;
	if (m_randomtime > (2.f))
	{
		m_randomtime = 0.f;
		v1to1.swap(v1to2);
		v2to1.swap(v2to2);
		v3to1.swap(v3to2);
		v4to1.swap(v4to2);

	}
	DrawWoobleLineGenerateCPU(Vec2(left, bottom), Vec2(left, up), pos.z , v1from, v1to1);
	DrawWoobleLineGenerateCPU(Vec2(left, up), Vec2(right, up), pos.z, v2from, v2to1);
	DrawWoobleLineGenerateCPU(Vec2(right, up), Vec2(right, bottom), pos.z, v3from, v3to1);
	DrawWoobleLineGenerateCPU(Vec2(right, bottom), Vec2(left, bottom), pos.z, v4from, v4to1);
	
	m_LineMat->SetScalarParam(SCALAR_PARAM::INT_0, 0);	
	float bs = 2.f;
	m_LineMat->SetScalarParam(SCALAR_PARAM::FLOAT_0, bs);

	DrawLine(Vec2(left, bottom), Vec2(left+ bs, bottom+ bs), pos.z);
	DrawLine(Vec2(right- bs, bottom), Vec2(right, bottom+ bs), pos.z);
	DrawLine(Vec2(left, up- bs), Vec2(left+ bs, up), pos.z);
	DrawLine(Vec2(right- bs, up- bs), Vec2(right, up), pos.z);
}


void CDreamBlockParticleSystem::DrawWoobleLineGenerateGPU(Vec2 _from, Vec2 _to, float _z)
{
	//GpuWooble =true;
	m_LineMat->SetScalarParam(SCALAR_PARAM::INT_0, 1);

	//from
	m_LineMat->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(_from.x, _from.y, _z, 0.f));
	//to
	m_LineMat->SetScalarParam(SCALAR_PARAM::VEC4_1, Vec4(_to.x, _to.y, _z, 0.f));
	m_LineMat->UpdateData();
	GetMesh()->render_point();
}


float CDreamBlockParticleSystem::RandomInterVal()
{
	// 가중치 선택 1,2,2,1
	const vector<float> w = { 1.f,2.f,10.f,10.f };
	const vector<std::pair<float, float>> val =
	{
		{1,4}
		,{5,8}
		,{9,12}
		,{13,16}
	};
	float r = RandomFloat() * (w[0]+w[1]+w[2]+w[3]);
	//1~4
	if (w[0] > r)
	{
		return val[0].first + RandomFloat() * (val[0].second- val[0].first);
	}
	//5~8
	else if ((w[0]+w[1]) > r)
	{
		return val[1].first + RandomFloat() * (val[1].second - val[1].first);
	}
	//9~12
	else if ((w[0] + w[1] + w[2]) > r)
	{
		return val[2].first + RandomFloat() * (val[2].second - val[2].first);
	}
	//13~16
	else
	{
		return val[3].first + RandomFloat() * (val[3].second - val[3].first);
	}
}

void CDreamBlockParticleSystem::WoolbeUpdate()
{
	wobbleEase += DT * 2.f;
	if (wobbleEase > 1.f)
	{
		wobbleEase = 0.f;
		wobbleFrom = wobbleTo;
		wobbleTo = RandomFloat(6.2831855f);
	}
}

float LineAmplitude(float seed, float index)
{
	return (float)(sin((double)(seed + index / 16.f) + sin((double)(seed * 2.f + index / 32.f)) * 6.2831854820251465f) + 1.0f) * 1.5f;
}

void CDreamBlockParticleSystem::DrawWoobleLineGenerateCPU(Vec2 from, Vec2 to, float _z, vector<float>& _out_from , vector<float>& _out_to)
{
	//가로 or 세로 판정
	Vec2 posdiff = Vec2(to.x - from.x, to.y - from.y);
	Vec2 normal = posdiff;
	normal.Normalize();
	posdiff.x = abs(posdiff.x);
	posdiff.y = abs(posdiff.y);

	bool vertical;
	if (posdiff.x < posdiff.y)
	{
		vertical = true;
	}
	else
	{
		vertical = false;
	}
	//Wooble =false;
	m_LineMat->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	m_LineMat->SetScalarParam(SCALAR_PARAM::INT_1, vertical ? 1 : 0);

	float length = Vector2::Distance(from, to);
	Vec2 perp = Vec2(normal.y, -normal.x);
	float lastAmp = 0.f;
	int   interval = 10;
	int   i = 2;
	int n = 0;
	float lastdiff = 0.f;
	while ((float)i < length - 2.f)
	{
		float amp = Lerp(LineAmplitude(wobbleFrom, (float)i), LineAmplitude(wobbleTo, (float)i), wobbleEase) / 1.5f;

		float thisinterval;  
		if (n == _out_from.size())
		{
			thisinterval = RandomInterVal();
			_out_from.push_back(thisinterval);
		}
		while (_out_to.size() <= n)
		{
			_out_to.push_back(RandomInterVal());
		}

		float lerp = Lerp(_out_from[n], _out_to[n], DT*5.f );
		float thisdiff = lerp - _out_from[n];
		_out_from[n] = thisinterval = (lerp - lastdiff);
		lastdiff = thisdiff;

		if ((float)(i + thisinterval) >= length)
		{
			amp = 0.f;
		}
		float len = min((float)thisinterval, length - 2.f - (float)i);
		Vec2 start = from + normal * (float)i + perp * lastAmp;
		Vec2 end = from + normal * ((float)i + len) + perp * amp;

		//DrawLine(start - perp, end - perp, 2, vertical, _OutStream);
		//DrawLine(start - perp * 2.f, end - perp * 2.f, 3, vertical, _OutStream);
		//DrawLine(start, end, 1, vertical, _OutStream);

		//from
		m_LineMat->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(start.x, start.y, _z, 0.f));
		//to
		m_LineMat->SetScalarParam(SCALAR_PARAM::VEC4_1, Vec4(end.x, end.y, _z, 0.f));
		m_LineMat->UpdateData();
		GetMesh()->render_point();

		lastAmp = amp;
		i += thisinterval;
		n++;
	}
}

void CDreamBlockParticleSystem::DrawWoobleLineGenerateCPU(Vec2 from, Vec2 to, float _z)
{
	//가로 or 세로 판정
	Vec2 posdiff = Vec2(to.x - from.x , to.y- from.y);
	Vec2 normal = posdiff;
	normal.Normalize();
	posdiff.x = abs(posdiff.x);
	posdiff.y = abs(posdiff.y);

	bool vertical;
	if (posdiff.x < posdiff.y)
	{
		vertical = true;
	}
	else
	{
		vertical = false;
	}
	//Wooble =false;
	m_LineMat->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	m_LineMat->SetScalarParam(SCALAR_PARAM::INT_1, vertical ? 1 :0);

	float length = Vector2::Distance(from,to);
	Vec2 perp = Vec2(normal.y, -normal.x);
	float lastAmp = 0.f;
	int   interval = 10;
	int   i = 2;

	while ((float)i < length - 2.f)
	{
		float amp = Lerp(LineAmplitude(wobbleFrom, (float)i), LineAmplitude(wobbleTo, (float)i), wobbleEase) / 1.5f;
		if ((float)(i + interval) >= length)
		{
			amp = 0.f;
		}
		float len = min((float)interval, length - 2.f - (float)i);
		Vec2 start = from + normal * (float)i + perp * lastAmp;
		Vec2 end = from + normal * ((float)i + len) + perp * amp;

		//DrawLine(start - perp, end - perp, 2, vertical, _OutStream);
		//DrawLine(start - perp * 2.f, end - perp * 2.f, 3, vertical, _OutStream);
		//DrawLine(start, end, 1, vertical, _OutStream);
		
		//from
		m_LineMat->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(start.x, start.y, _z, 0.f));
		//to
		m_LineMat->SetScalarParam(SCALAR_PARAM::VEC4_1, Vec4(end.x, end.y, _z, 0.f));
		m_LineMat->UpdateData();
		GetMesh()->render_point();

		lastAmp = amp;
		i += interval;
	}
}

void CDreamBlockParticleSystem::DrawLine(Vec2 _from, Vec2 _to, float _z)
{
	//Wooble =false;
	m_LineMat->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	//from
	m_LineMat->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(_from.x, _from.y, _z, 0.f));
	//to
	m_LineMat->SetScalarParam(SCALAR_PARAM::VEC4_1, Vec4(_to.x, _to.y, _z, 0.f));
	m_LineMat->UpdateData();
	GetMesh()->render_point();
}

void CDreamBlockParticleSystem::begin()
{
	CRenderComponent::begin();
	LoadDreamFab();
	m_bThisFrameGenerate = true;
}

void CDreamBlockParticleSystem::UpdateParallaxCorrection()
{
	for (int i = 0; i < 3; ++i)
	{
		// 0=0.5 90=1 180 = 1.5
		m_Module.NdcBoxbyDepth[i] = m_Module.vSpawnDepth[i] / 180.f + 0.5f;

		// 0 = 0 ,90 = 1 ,180 = 2;
		m_Module.ScaleMultibyDepth[i] = m_Module.vSpawnDepth[i] / 90.f;
	}
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


