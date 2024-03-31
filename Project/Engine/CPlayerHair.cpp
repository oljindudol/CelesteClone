#include "pch.h"
#include "CPlayerHair.h"
#include < fstream >
#include <iostream>

CPlayerHair::CPlayerHair()
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
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_DebugMeterial));

    SetMetaData();
}

CPlayerHair::~CPlayerHair()
{
}


void CPlayerHair::SetMetaData()
{
    auto conpath = CPathMgr::GetContentPath();
    wstring fp = conpath;
    fp += L"metadata";
    string filepath = ToString(fp);

    //std::unordered_map<std::string, tPlayerHairInfo> hairInfoMap;
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
    int a = 0;


}

void CPlayerHair::finaltick()
{
}

void CPlayerHair::UpdateData()
{
}

void CPlayerHair::render()
{
}


