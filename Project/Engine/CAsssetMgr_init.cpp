#include "pch.h"
#include "CAssetMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"


void CAssetMgr::init()
{
	InitSound();

	CreateDefaultMesh();

	CreateDefaultGraphicsShader();

	CreateDefaultComputeShader();

	CreateDefaultMaterial();
}

void CAssetMgr::CreateDefaultMesh()
{
	CMesh* pMesh = nullptr;

	Vtx vPoint;
	UINT Idx = 0;
	pMesh = new CMesh(true);
	pMesh->Create(&vPoint, 1, &Idx, 1);
	AddAsset(STR_KEY_PointMesh, pMesh);


	// 전역변수에 삼각형 위치 설정
	//   0(Red)-- 1(Blue)	     
	//    |   \   |	     
	//   3(G)---- 2(Magenta)  
	Vtx arrVtx[4] = {};

	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);

	// 인덱스
	UINT arrIdx[6] = {};
	arrIdx[0] = 0;
	arrIdx[1] = 1;
	arrIdx[2] = 2;

	arrIdx[3] = 0;
	arrIdx[4] = 2;
	arrIdx[5] = 3;

	pMesh = new CMesh(true);
	pMesh->Create(arrVtx, 4, arrIdx, 6);
	AddAsset(STR_KEY_RectMesh, pMesh);


	// Topology LineStrip 용도	
	//   0(Red)-- 1(Blue)	     
	//    |       |	     
	//   3(G)---- 2(Magenta)   
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2;	arrIdx[3] = 3; 	arrIdx[4] = 0;

	pMesh = new CMesh(true);
	pMesh->Create(arrVtx, 4, arrIdx, 5);
	AddAsset(STR_KEY_RectMesh_Debug, pMesh);

	// =================
	// CircleMesh 만들기
	// =================
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	// 중심 점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	UINT iSlice = 40;
	float fTheta = 0.f;
	float fRadius = 0.5f;

	for (UINT i = 0; i <= iSlice; ++i)
	{
		fTheta = (XM_2PI / iSlice) * i;

		v.vPos = Vec3(fRadius * cosf(fTheta), fRadius * sinf(fTheta), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(0.f, 0.f);

		vecVtx.push_back(v);
	}

	for (UINT i = 0; i < iSlice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(STR_KEY_CircleMesh, pMesh);


	// CircleMesh_Debug
	vecIdx.clear();
	for (int i = 1; i < vecVtx.size(); ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(STR_KEY_CircleMesh_Debug, pMesh);
	vecVtx.clear();
	vecIdx.clear();

	// =================
	// CrossMesh 만들기
	// =================
	v.vPos = Vec3(0.f, 0.5f, 0.f);
	v.vUV = Vec2(0.f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.f, -0.5f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, 0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.f, 0.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0); vecIdx.push_back(1);
	vecIdx.push_back(2); vecIdx.push_back(3);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(STR_KEY_CrossMesh, pMesh);
	vecVtx.clear();
	vecIdx.clear();
}

void CAssetMgr::CreateDefaultGraphicsShader()
{
	Ptr<CGraphicsShader> pShader = nullptr;


	// ====================
	// DrawLineShader 기본GS+애니메이터PS
	// ====================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_DrawLine, STR_FUNC_NAME_VTXShaderDrawLine);
	pShader->CreateGeometryShader(STR_FILE_PATH_DrawLine, STR_FUNC_NAME_GEOShaderDrawLine);
	pShader->CreatePixelShader(STR_FILE_PATH_DrawLine, STR_FUNC_NAME_PIXShaderDrawLine);

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);		
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset(STR_KEY_DrawLineRenderShader, pShader.Get());



	// ====================
	// DreamParticleRenderShader 드림GS+드림PS
	// ====================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_VTXShaderParticle);
	pShader->CreateGeometryShader(STR_FILE_PATH_DreamParticleShader, STR_FUNC_NAME_GEOShaderDreamParticle);
	pShader->CreatePixelShader(STR_FILE_PATH_DreamParticleShader, STR_FUNC_NAME_PIXShaderDreamParticle);

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);		// 깊이 테스트는 진행, 깊이는 기록 X
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(STR_KEY_DreamParticleRenderShader, pShader.Get());



	// ====================
	// AnimatedParticleRenderShader(Custom) 기본GS+애니메이터PS
	// ====================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_VTXShaderParticle);
	pShader->CreateGeometryShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_GEOShaderParticle);
	pShader->CreatePixelShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_PIXShaderParticleAnimated);

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);		// 깊이 테스트는 진행, 깊이는 기록 X
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(STR_KEY_AnimatedParticleRenderShader, pShader.Get());


	// ====================
	// CustomParticleRenderShader(Custom) 커스텀GS+기본PS
	// ====================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_VTXShaderParticle);
	pShader->CreateGeometryShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_GEOShaderParticleCustom);
	pShader->CreatePixelShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_PIXShaderParticle);

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);		// 깊이 테스트는 진행, 깊이는 기록 X
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(STR_KEY_CustomParticleRenderShader, pShader.Get());


	// =================================
	// Distortion Shader	
	// RS_TYPE		: CULL_BACK
	// DS_TYPE		: NO_TEST_NO_WRITE
	// BS_TYPE		: Default
	// Domain		: DOMAIN_POSTPROCESS
	// =================================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_PostProcessShader, STR_FUNC_NAME_VTXShaderDistorton);
	pShader->CreatePixelShader(STR_FILE_PATH_PostProcessShader, STR_FUNC_NAME_PIXShaderDistorton);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(STR_KEY_ShockWaveShader, pShader.Get());




	// =================================
	// HairShader
	// =================================

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_PlayerShader, STR_FUNC_NAME_VTXShaderHair);
	pShader->CreatePixelShader(STR_FILE_PATH_PlayerShader, STR_FUNC_NAME_PIXShaderHair);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	// Parameter	
	AddAsset(STR_KEY_PlayerShader, pShader.Get());

	// =================================
	// Std2DShader
	// =================================

	pShader = new CGraphicsShader;	
	pShader->CreateVertexShader(STR_FILE_PATH_Std2dShader, STR_FUNC_NAME_VTXShaderStd2d);
	pShader->CreatePixelShader(STR_FILE_PATH_Std2dShader, STR_FUNC_NAME_PIXShaderStd2d);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
		
	// Parameter	
	pShader->AddScalarParam(SCALAR_PARAM::INT_0, "Test Param");
	pShader->AddTexParam(TEX_PARAM::TEX_0, "Output Texture 1");
	
	AddAsset(STR_KEY_Std2dShader, pShader.Get());




	// =================================
	// EffectShader
	// =================================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_Std2dShader, STR_FUNC_NAME_VTXShaderStd2d);
	pShader->CreatePixelShader(STR_FILE_PATH_Std2dShader, STR_FUNC_NAME_PIXShaderStd2d);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(STR_KEY_EffectShader, pShader.Get());

	// =============
	// TileMapShader
	// =============
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_TileMapShader, STR_FUNC_NAME_VTXShaderTileMap);
	pShader->CreatePixelShader(STR_FILE_PATH_TileMapShader, STR_FUNC_NAME_PIXShaderTileMap);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset(STR_KEY_TileMapShader, pShader.Get());

	// =============
	// GridShader
	// =============
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_TileMapShader, STR_FUNC_NAME_VTXShaderGrid);
	pShader->CreatePixelShader(STR_FILE_PATH_TileMapShader, STR_FUNC_NAME_PIXShaderGrid);

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset(STR_KEY_GridShader, pShader.Get());

	// ====================
	// ParticleRenderShader
	// ====================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_VTXShaderParticle);
	pShader->CreateGeometryShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_GEOShaderParticle);
	pShader->CreatePixelShader(STR_FILE_PATH_ParticleShader, STR_FUNC_NAME_PIXShaderParticle);

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);		// 깊이 테스트는 진행, 깊이는 기록 X
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	
	AddAsset(STR_KEY_ParticleRenderShader, pShader.Get());

	// =================================
	// GrayFilter Shader
	// Mesh			: RectMesh
	// RS_TYPE		: CULL_BACK
	// DS_TYPE		: NO_TEST_NO_WRITE
	// BS_TYPE		: Default
	// Domain		: DOMAIN_POSTPROCESS
	// =================================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_PostProcessShader, STR_FUNC_NAME_VTXShaderGrayFilter);
	pShader->CreatePixelShader(STR_FILE_PATH_PostProcessShader, STR_FUNC_NAME_PIXShaderGrayFilter);

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(STR_KEY_GrayFilterShader, pShader.Get());

	// =================================
	// Distortion Shader	
	// RS_TYPE		: CULL_BACK
	// DS_TYPE		: NO_TEST_NO_WRITE
	// BS_TYPE		: Default
	// Domain		: DOMAIN_POSTPROCESS
	// =================================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_PostProcessShader, STR_FUNC_NAME_VTXShaderDistorton);
	pShader->CreatePixelShader(STR_FILE_PATH_PostProcessShader, STR_FUNC_NAME_PIXShaderDistorton);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(STR_KEY_DistortionShader, pShader.Get());

	// =================================
	// DebugShape Shader
	// =================================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(STR_FILE_PATH_DebugShader, STR_FUNC_NAME_VTXShaderDebugShape);
	pShader->CreatePixelShader(STR_FILE_PATH_DebugShader, STR_FUNC_NAME_PIXShaderDebugShape);

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);

	AddAsset(STR_KEY_DebugShapeShader, pShader.Get());
}



void CAssetMgr::CreateDefaultMaterial()
{
	CMaterial* pMtrl = nullptr;

	// DrawLineMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_DrawLineRenderShader));
	AddAsset<CMaterial>(STR_KEY_DrawLineMeterial, pMtrl);

	// DreamParticleMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_DreamParticleRenderShader));
	AddAsset<CMaterial>(STR_KEY_DreamParticleMeterial, pMtrl);

	// AnimatedParticleMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_AnimatedParticleRenderShader));
	AddAsset<CMaterial>(STR_KEY_AnimatedParticleMeterial, pMtrl);

	// CustomParticleMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_CustomParticleRenderShader));
	AddAsset<CMaterial>(STR_KEY_CustomParticleMeterial, pMtrl);

	// DistortionMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_ShockWaveShader));
	AddAsset<CMaterial>(STR_KEY_ShockWaveMeterial, pMtrl);

	// HairMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_PlayerShader));
	AddAsset<CMaterial>(STR_KEY_HairMeterial, pMtrl);

	// Std2DMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_Std2dShader));
	AddAsset<CMaterial>(STR_KEY_Std2dMeterial, pMtrl);

	// TestMtrl
	//pMtrl = new CMaterial(true);
	//pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_Std2dShader));
	//pMtrl->SetTexParam(TEX_0, Load<CTexture>(L"texture\\Background.jpg", L"texture\\Background.jpg"));
	//pMtrl->SetScalarParam(INT_0, 10);
	//pMtrl->Save(L"material\\testmtrl.mtrl");
	//AddAsset<CMaterial>(L"TestMtrl", pMtrl);
	

	// BackgroundMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_Std2dShader));
	AddAsset<CMaterial>(STR_KEY_BackGroundMeterial, pMtrl);

	// TileMapMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_TileMapShader));
	AddAsset<CMaterial>(STR_KEY_TileMapMeterial, pMtrl);

	// GridMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_GridShader));
	AddAsset<CMaterial>(STR_KEY_GridMeterial, pMtrl);

	// ParticleMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_ParticleRenderShader));
	AddAsset<CMaterial>(STR_KEY_ParticleMeterial, pMtrl);

	// GrayFilterMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_GrayFilterShader));
	AddAsset<CMaterial>(STR_KEY_GrayFilterMeterial, pMtrl);

	// DistortionMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_DistortionShader));
	AddAsset<CMaterial>(STR_KEY_DistortionMeterial, pMtrl);

	// DebugShapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicsShader>(STR_KEY_DebugShapeShader));
	AddAsset<CMaterial>(STR_KEY_DebugMeterial, pMtrl);


}




#include "CDreamParticleUpdate.h"
#include "CSetColorShader.h"
#include "CParticleUpdate.h"
void CAssetMgr::CreateDefaultComputeShader()
{
	Ptr<CComputeShader> pShader = nullptr;

	// SetColorShader
	pShader = new CSetColorShader;	
	AddAsset(STR_KEY_SetColorShader, pShader.Get());

	// ParticleUpdateShader
	pShader = new CParticleUpdate;
	AddAsset(STR_KEY_ParticleUpdateShader, pShader.Get());

	// DreamParticleUpdateShader
	pShader = new CDreamParticleUpdate;
	AddAsset(STR_KEY_DreamParticleUpdateShader, pShader.Get());
}


#include "CSound.h"
void CAssetMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}
