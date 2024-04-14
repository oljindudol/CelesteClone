#include "pch.h"
#include "CTileMapScript.h"
#include <Engine\CTileMap.h>

CTileMapScript::CTileMapScript()
	: CScript((UINT)SCRIPT_TYPE::TILEMAPSCRIPT)
{
}

CTileMapScript::~CTileMapScript()
{
}

void CTileMapScript::begin()
{
	// FORE TILE OBJ
	CGameObject* pChidObj = new CGameObject;
	pChidObj->SetName(STR_KEY_TILEFAP_FORE_GROUND);

	pChidObj->AddComponent(new CTransform);
	pChidObj->AddComponent(new CTileMap);

	pChidObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	static const std::filesystem::path base_path = CPathMgr::GetContentPath();
	wstring ForeTilePath = base_path;
	ForeTilePath += STR_FOLDER_PATH_FORE_TILE;
	auto files = getImagesFromDirectory(ForeTilePath);

	for (auto& p : files)
	{
		std::filesystem::path relative_path = std::filesystem::relative(p, base_path);
		Ptr<CTexture> pAtlasTex = CAssetMgr::GetInst()->Load<CTexture>(relative_path, relative_path);
		pChidObj->TileMap()->SetTileAtlas(pAtlasTex);
	}
	Ptr<CTexture> pTileAtlas = CAssetMgr::GetInst()->CreateArrayTexture(STR_KEY_TEXARR_FORE_TILE, pChidObj->TileMap()->GetTileAtlases(), 1);
	pChidObj->TileMap()->SetArrAtlas(pTileAtlas);
	pChidObj->TileMap()->SetColRow(40, 23);
	pChidObj->TileMap()->SetGridColor(GamePlayStatic::Yellow);
	pChidObj->TileMap()->SetGridVisible(true);

	GetOwner()->AddChild(pChidObj);
	GamePlayStatic::SpawnGameObject(pChidObj, LAYER_FORETILE);


	// BACK TILE OBJ
	pChidObj = new CGameObject;
	pChidObj->SetName(STR_KEY_TILEFAP_BACK_GROUND);

	pChidObj->AddComponent(new CTransform);
	pChidObj->AddComponent(new CTileMap);

	pChidObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	wstring BackTilePath = base_path;
	BackTilePath += STR_FOLDER_PATH_BACK_TILE;
	files = getImagesFromDirectory(BackTilePath);

	for (auto& p : files)
	{
		std::filesystem::path relative_path = std::filesystem::relative(p, base_path);
		Ptr<CTexture> pAtlasTex = CAssetMgr::GetInst()->Load<CTexture>(relative_path, relative_path);
		pChidObj->TileMap()->SetTileAtlas(pAtlasTex);
	}
	pTileAtlas = CAssetMgr::GetInst()->CreateArrayTexture(STR_KEY_TEXARR_BACK_TILE, pChidObj->TileMap()->GetTileAtlases(), 1);
	pChidObj->TileMap()->SetArrAtlas(pTileAtlas);
	pChidObj->TileMap()->SetColRow(40, 23);
	pChidObj->TileMap()->SetGridColor(GamePlayStatic::Mazenta);
	pChidObj->TileMap()->SetGridVisible(false);

	GetOwner()->AddChild(pChidObj);
	GamePlayStatic::SpawnGameObject(pChidObj, LAYER_BACKTILE);
}

void CTileMapScript::SaveToFile(FILE* _File)
{
}

void CTileMapScript::LoadFromFile(FILE* _File)
{
}


