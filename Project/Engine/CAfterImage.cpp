#include "pch.h"
#include "CAfterImage.h"
#include "CTimeMgr.h"
#include "CTransform.h"
#include "CDevice.h"
#include "CConstBuffer.h"


CAfterImage::CAfterImage()
	: CRenderComponent(COMPONENT_TYPE::PLAYERHAIR)
{
	//hair 텍스쳐 할당
	m_HairTex = CAssetMgr::GetInst()->Load<CTexture>(STR_FILE_PATH_HAIR, STR_FILE_PATH_HAIR);

	//bangs 텍스쳐 할당
	std::filesystem::path base_path = CPathMgr::GetContentPath();
	wstring Path = base_path;
	Path += STR_FOLDER_PATH_BANGS;
	auto files = getImagesFromDirectory(Path);

	for (auto& p : files)
	{
		std::filesystem::path relative_path = std::filesystem::relative(p, base_path);
		Ptr<CTexture> pAtlasTex = CAssetMgr::GetInst()->Load<CTexture>(relative_path, relative_path);
		m_vecBangTex.push_back(pAtlasTex);
	}
	//매쉬 메테리얼 할당
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_HairMeterial));
}

CAfterImage::~CAfterImage()
{
}




void CAfterImage::RenderPlayer(Color _Color, Facings _facing,Vec3 _MulScale)
{
	static CConstBuffer* CBp = CDevice::GetInst()->GetConstBuffer(CB_TYPE::HAIR);

	tHair data = {};
	data.vHairColor = (Vec4)_Color;
	data.vOffset = Vec3(0.f,-0.375f, 0.f);
	data.vScale = Vec3((_facing)*_MulScale.x, _MulScale.y, _MulScale.z);
	data.bang = 1;
	CBp->SetData(&data);
	CBp->UpdateData();
	GetMesh()->render();
	int a = 0;
}

void CAfterImage::render()
{
	//process Event
	for (auto iter = m_EventList.begin(); iter != m_EventList.end(); )
	{
		if (iter->AccTime > iter->Duration)
		{
			iter = m_EventList.erase(iter);
		}
		else
		{
			iter->AccTime += DT;

			auto mat1 = Transform()->GetWorldMat();
			Matrix mat2 = iter->PlayerWorldMat;



			float percent = iter->AccTime / iter->Duration;
			float alpha = (percent < 1.f) ? (0.75f * (1 - pow(percent, 3))) : 0.f;
			auto BC = m_RenderInfo.BorderColor;
			auto HC = m_RenderInfo.HairColor;

			BC = Color(BC.x , BC.y , BC.z , BC.w * alpha);
			HC = Color(HC.x , HC.y , HC.z , HC.w * alpha);
			//BC = Color(BC.x * alpha, BC.y * alpha, BC.z * alpha, BC.w * alpha);
			//HC = Color(HC.x * alpha, HC.y * alpha, HC.z * alpha, HC.w * alpha);
			const float OutLineWidth = 0.5f;
			const float OutLineDepth = 0.002f;
			const float HairDepth = 0.001f;


			//플레이어 랜더
			//static const float PlayerYOffset = (0.5f) * 32.f;
			Transform()->UpdateData(iter->PlayerWorldMat);
			GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, iter->PlayerAnimTex );
			GetMaterial()->UpdateData();
			RenderPlayer(HC, iter->facing, Vec3(iter->vMult, 1.f));


			//헤어 랜더
			m_RenderInfo = iter->HairInfo;

			//RenderHair(BC, Vec3(OutLineWidth, 0.0f, OutLineDepth));
			//RenderHair(BC, Vec3(-OutLineWidth, 0.0f, OutLineDepth));
			//RenderHair(BC, Vec3(0.0f, OutLineWidth, OutLineDepth));
			//RenderHair(BC, Vec3(0.0f, -OutLineWidth, OutLineDepth));
			Transform()->UpdateData(iter->HairWorldMat);
			RenderHair(HC ,Vec3(0.0f, 0.f, HairDepth));

			++iter;
		}
	}
}


void CAfterImage::RenderHair(Color _Color, Vec3 _offset)
{
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::HAIR);


	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HairTex);
	GetMaterial()->UpdateData();

	auto vec = m_RenderInfo.vecHairNodes;
	for (size_t i = 1; i < vec.size(); ++i)
	{
		tHair data = {};
		data.vHairColor = (Vec4)_Color;
		data.vOffset = Vec3((vec[i].vOffset - vec[0].vOffset), 0) + Vec3(0, 4, 0) + _offset;
		////오프셋 계산 이관실패1
		//data.vOffset = Vec3((vec[i].vOffset),0.f) + Vec3(0, 4, 0) + _offset;// - vec[0].vOffset), 0) + Vec3(0, 4, 0) +_offset;
		data.vScale = Vec2(10, 10) * vec[i].vScale;
		//data.bang = 0;
		pCB->SetData(&data);
		pCB->UpdateData();
		GetMesh()->render();
		int a = 0;
	}


	//bang render
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_vecBangTex[m_RenderInfo.ThisFrameBangIdx]);
	GetMaterial()->UpdateData();
	tHair data = {};
	data.vHairColor = (Vec4)_Color;
	data.vOffset = Vec2(0, 6) + _offset;
	if (true == m_RenderInfo.BangOnePixelDown)
	{
		data.vOffset = data.vOffset + Vec3(0, -1, 0);
	}
	data.vScale = Vec2(((int)m_RenderInfo.facing) * 10, 10);
	//data.bang = 0;
	pCB->SetData(&data);
	pCB->UpdateData();
	GetMesh()->render();
}



void CAfterImage::finaltick()
{
}

void CAfterImage::UpdateData()
{
}
