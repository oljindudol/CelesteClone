#include "pch.h"
#include "CHair.h"
#include < fstream >
#include <iostream>
#include "CRenderMgr.h"
#include "CAnimator2D.h"
#include "CAnim.h"
#include "CDevice.h"
#include "CConstBuffer.h"

CHair::CHair()
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
    SetMetaData();
}

CHair::~CHair()
{
}


void CHair::RenderHair(Color _Color, Vec3 _offset)
{
    auto vec = m_RenderInfo.vecHairNodes;
    static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::HAIR);
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
    }


    //bang render
    GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
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
    //data.bang = 1;
    pCB->SetData(&data);
    pCB->UpdateData();
    GetMesh()->render();
}



void CHair::render()
{
    if (nullptr == GetMesh() || nullptr == GetMaterial())
        return;
    Animator2D()->Clear();

    auto AN = ToString(GetOwner()->GetParent()->Animator2D()->GetCurAnimName());
    if(0 == AN.find("dreamDash", 0))
        return;
    //an.find

    //UpdateData();

    //if (nullptr != GetMaterial())
    //{
    //    GetMaterial()->UpdateData();
    //}

    //hair render
    GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
    GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HairTex);
    GetMaterial()->UpdateData();

    Transform()->UpdateData();

    //RenderHair(m_RenderInfo.BorderColor, Vec3(1 , 0,1));
    //RenderHair(m_RenderInfo.BorderColor, Vec3(-1, 0,1));
    //RenderHair(m_RenderInfo.BorderColor, Vec3(0 , 1,1));
    //RenderHair(m_RenderInfo.BorderColor, Vec3(0, -1,1));
    //bool OnePixDown = false;
    //auto PAniator = GetOwner()->GetParent()->Animator2D();
    //if (L"idle" == PAniator->GetCurAnimName()
    //    && 2< PAniator->GetCurAnim()->GetCurIdx())
    //{
    //    OnePixDown = true;
    //}

    auto BC = m_RenderInfo.BorderColor;
    auto HC = m_RenderInfo.HairColor;

    const float OutLineWidth = 0.7f;
    const float OutLineDepth = 0.02f;
    const float HairDepth = 0.01f;

    RenderHair(BC, Vec3(OutLineWidth, 0.0f, OutLineDepth));
    RenderHair(BC, Vec3(-OutLineWidth, 0.0f, OutLineDepth));
    RenderHair(BC, Vec3(0.0f, OutLineWidth, OutLineDepth));
    RenderHair(BC, Vec3(0.0f, -OutLineWidth, OutLineDepth));
    RenderHair(HC, Vec3(0.0f, 0.f, HairDepth)) ;

    //RenderHair(Vec4(BC.x, BC.y, BC.z, alpha), OnePixDown,Vec3(OutLineWidth, 0.0f, OutLineDepth));
    //RenderHair(Vec4(BC.x, BC.y, BC.z, alpha), OnePixDown,Vec3(-OutLineWidth, 0.0f, OutLineDepth));
    //RenderHair(Vec4(BC.x, BC.y, BC.z, alpha), OnePixDown,Vec3(0.0f, OutLineWidth, OutLineDepth));
    //RenderHair(Vec4(BC.x, BC.y, BC.z, alpha), OnePixDown,Vec3(0.0f, -OutLineWidth, OutLineDepth));
    //RenderHair(Vec4(HC.x, HC.y, HC.z, alpha), OnePixDown);
    //RenderHair(m_RenderInfo.HairColor, OnePixDown);

}


void CHair::finaltick()
{
    //if (CRenderMgr::GetInst()->IsDebugPosition())
    //{
    //    GamePlayStatic::DrawDebugCross(Transform()->GetWorldPos(), 20.f, Vec3(0.f, 1.f, 0.f), true);
    //}
}

void CHair::UpdateData()
{
    //if (nullptr != GetMaterial())
    //{
    //    GetMaterial()->UpdateData();
    //}

    //auto originpos = Transform()->GetWorldPos();

    ////Transform()->SetRelativePos(Vec3(originpos.x - 50.f, originpos.y - 50.f, originpos.z - 50.f));
    //Transform()->UpdateData();
}

tPlayerHairInfo CHair::GettHairInfo()
{
    tPlayerHairInfo ret = {};
    auto p = GetOwner()->GetParent();
    auto Animator = p->Animator2D();
    if (nullptr == Animator)
        return ret;
    if (nullptr == Animator->GetCurAnim())
        return ret;

    auto AnimName = Animator->GetCurAnimName();
    auto AnimIdx = Animator->GetCurAnim()->GetCurIdx();

    string key = ToString(AnimName) + std::to_string(AnimIdx);
    auto iter = m_umHairInfo.find(key);
    if (m_umHairInfo.end() != iter)
    {
        ret = iter->second;
    }
    else
    {
        int a = 0;
    }

    return ret;
}

void CHair::SetMetaData()
{
    auto conpath = CPathMgr::GetContentPath();
    wstring fp = conpath;
    fp += L"metadata";
    string filepath = ToString(fp);

    std::ifstream file(filepath);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "파일을 열 수 없습니다: " << filepath << std::endl;
        return;
    }

    while (getline(file, line)) {
        std::istringstream lineStream(line);
        std::string path, hairData;
        size_t startPos, endPos;

        // path 파싱
        startPos = line.find("path=\"") + 6;
        endPos = line.find("\"", startPos);
        path = line.substr(startPos, endPos - startPos);

        // hair 데이터 파싱
        startPos = line.find("hair=\"", endPos) + 6;
        endPos = line.find("\"", startPos);
        hairData = line.substr(startPos, endPos - startPos);

        std::istringstream hairStream(hairData);
        std::string hairItem;
        int idx = 0;

        while (getline(hairStream, hairItem, '|')) {
            tPlayerHairInfo info;
            if (hairItem == "x") {
                info.HasHair = false;
            }
            else {
                std::istringstream itemStream(hairItem);
                std::string part;
                getline(itemStream, part, ',');
                info.HairOffset.x = std::stoi(part);
                getline(itemStream, part, ',');
                size_t colonPos = part.find(':');
                if (colonPos != std::string::npos) {
                    info.HairOffset.y = std::stoi(part.substr(0, colonPos));
                    info.BangFrame = std::stoi(part.substr(colonPos + 1));
                }
                else {
                    info.HairOffset.y = std::stoi(part);
                }
            }
            m_umHairInfo[path + std::to_string(idx)] = info;
            ++idx;
        }
    }


}