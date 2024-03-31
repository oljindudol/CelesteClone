#include "pch.h"
#include "CHair.h"
#include < fstream >
#include <iostream>
#include "CRenderMgr.h"
#include "CAnimator2D.h"
#include "CAnim.h"

CHair::CHair()
	: CRenderComponent(COMPONENT_TYPE::PLAYERHAIR)
{

	//hair �ؽ��� �Ҵ�
	m_HairTex = CAssetMgr::GetInst()->Load<CTexture>(STR_FILE_PATH_HAIR, STR_FILE_PATH_HAIR);
	
	//bangs �ؽ��� �Ҵ�
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
	//�Ž� ���׸��� �Ҵ�
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_HairMeterial));
    GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HairTex);
    auto a = GetMaterial();
    SetMetaData();
}

CHair::~CHair()
{
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
        std::cerr << "������ �� �� �����ϴ�: " << filepath << std::endl;
        return;
    }

    while (getline(file, line)) {
        std::istringstream lineStream(line);
        std::string path, hairData;
        size_t startPos, endPos;

        // path �Ľ�
        startPos = line.find("path=\"") + 6;
        endPos = line.find("\"", startPos);
        path = line.substr(startPos, endPos - startPos);

        // hair ������ �Ľ�
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

tPlayerHairInfo CHair::GettHairInfo()
{
    tPlayerHairInfo ret = {};
    auto p = GetOwner()->GetParent();
    auto Animator = p->Animator2D();
    if(nullptr == Animator)
        return ret;
    if(nullptr == Animator->GetCurAnim())
        return ret;

    auto AnimName = Animator->GetCurAnimName();
    auto AnimIdx = Animator->GetCurAnim()->GetCurIdx();

    string key = ToString(AnimName) + std::to_string(AnimIdx);
    auto iter = m_umHairInfo.find(key);
    if (m_umHairInfo.end() != iter)
        ret = iter->second;

    return ret;
}

void CHair::finaltick()
{
    if (CRenderMgr::GetInst()->IsDebugPosition())
    {
        GamePlayStatic::DrawDebugCross(Transform()->GetWorldPos(), 20.f, Vec3(0.f, 1.f, 0.f), true);
    }
}

void CHair::UpdateData()
{
    if (nullptr != GetMaterial())
    {
        GetMaterial()->UpdateData();
    }

    Transform()->UpdateData();
}

void CHair::render()
{
    if (nullptr == GetMesh() || nullptr == GetMaterial())
        return;
    if (Animator2D())
    {
        Animator2D()->UpdateData();
    }
    else
    {
        Animator2D()->Clear();
    }
    UpdateData();

    GetMesh()->render();
}


