#include "pch.h"
#include "CAfterImage.h"
#include "CTimeMgr.h"
#include "CTransform.h"

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

void CAfterImage::RenderPlayer(Color Color, bool _OnePixelDown, Vec3 offset)
{
}

void CAfterImage::RenderHair(Color Color, bool _OnePixelDown, Vec3 offset)
{
}


void CAfterImage::render()
{
	//process Event
	for (auto iter = m_EventList.begin(); iter!= m_EventList.end(); )
	{
		if (iter->AccTime > iter->Duration)
		{
			iter = m_EventList.erase(iter) ;
		}
		else
		{
			iter->AccTime += DT;
			Transform()->UpdateData(iter->PlayerWorldMat);
			
			
			//플레이어 랜더
			GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HairTex);


			//RenderPlayer();
			
			
			//헤어 랜더
			bool OnePixDown = false;
			//auto PAniator = GetOwner()->GetParent()->Animator2D();
			//if (L"idle" == PAniator->GetCurAnimName()
			//    && 2< PAniator->GetCurAnim()->GetCurIdx())
			//{
			//    OnePixDown = true;
			//}



			//auto HC = iter->m_RenderInfo.HairColor;
			//auto BC = m_RenderInfo.HairColor;
			//auto alpha = 0.3f;

			//const float OutLineWidth = 0.7f;
			//const float OutLineDepth = 0.01f;

			//RenderHair(BC, OnePixDown, Vec3(OutLineWidth, 0.0f, OutLineDepth));
			//RenderHair(BC, OnePixDown, Vec3(-OutLineWidth, 0.0f, OutLineDepth));
			//RenderHair(BC, OnePixDown, Vec3(0.0f, OutLineWidth, OutLineDepth));
			//RenderHair(BC, OnePixDown, Vec3(0.0f, -OutLineWidth, OutLineDepth));
			//RenderHair(HC, OnePixDown);
			//RenderHair();

			++iter;
		}
	}
}


void CAfterImage::finaltick()
{
}

void CAfterImage::UpdateData()
{
}
