#include "pch.h"
#include "CTileMap.h"

#include "CStructuredBuffer.h"

#include "CAssetMgr.h"
#include "CMesh.h"

#include "CTransform.h"
#include "CTileMapGrid.h"
#include "CRenderMgr.h"
#include "CCamera.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_Col(40)
	, m_Row(23)
	, m_vTileRenderSize(Vec2(8.f, 8.f))
	, m_TileInfoBuffer(nullptr)
	, m_bGridVisible(true)
	, m_pGrid(nullptr)
	, m_IdxHighLight(-1)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(STR_KEY_TileMapMeterial));

	m_TileInfoBuffer = new CStructuredBuffer;

	SetColRow(m_Col, m_Row);

	if (!m_pGrid)
		m_pGrid = new CTileMapGrid(this);
	m_pGrid->Init();
}

CTileMap::CTileMap(const CTileMap& _OriginTileMap)
	: CRenderComponent(_OriginTileMap)
	, m_Col(_OriginTileMap.m_Col)
	, m_Row(_OriginTileMap.m_Row)
	, m_vTileRenderSize(_OriginTileMap.m_vTileRenderSize)
	, m_vecTileAtlas(_OriginTileMap.m_vecTileAtlas)
	, m_vTilePixelSize(_OriginTileMap.m_vTilePixelSize)
	//, m_vSliceSizeUV(_OriginTileMap.m_vSliceSizeUV)
	, m_MaxCol(_OriginTileMap.m_MaxCol)
	, m_MaxRow(_OriginTileMap.m_MaxRow)
	, m_vecTileInfo(_OriginTileMap.m_vecTileInfo)
	, m_TileInfoBuffer(nullptr)
	, m_pGrid(nullptr)
{
	if (nullptr != _OriginTileMap.m_TileInfoBuffer)
	{
		m_TileInfoBuffer = _OriginTileMap.m_TileInfoBuffer->Clone();

		if (!m_pGrid)
			m_pGrid = new CTileMapGrid(this);
		m_pGrid->Init();
	}	
}

CTileMap::~CTileMap()
{
	if (nullptr != m_TileInfoBuffer)
		delete m_TileInfoBuffer;

	if (nullptr != m_pGrid)
		delete m_pGrid;
}

void CTileMap::finaltick()
{
	// (타일 개수 * 타일 사이즈) 로 사이즈를 변경처리한다.
	Vec3 vTileMapSize = Vec3(m_Col * m_vTileRenderSize.x, m_Row * m_vTileRenderSize.y, 1.f);
	Transform()->SetRelativeScale(vTileMapSize);
}

void CTileMap::render()
{	
	// 재질에 아틀라스 텍스쳐 전달.
	GetMaterial()->SetTexParam(TEX_PARAM::TEXARR_0, m_arrAtlas);
	//GetMaterial()->SetTexParam(TEX_PARAM::TEX_1, m_vecTileAtlas[1].first);
	
	// 타일의 가로 세로 개수
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, m_Col);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, m_Row);
		
	// 아틀라스 이미지에서 타일 1개의 자르는 사이즈(UV 기준)
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, m_vecTileAtlas[0].second);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, m_vecTileAtlas[1].second);

	// Grid 색상
	auto LayerIdx = GetOwner()->GetLayerIdx();
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, (LAYER_FORETILE == LayerIdx) ? GamePlayStatic::Yellow : GamePlayStatic::Mazenta) ;

	// 각 타일 정보를 구조화 버퍼로 이동
	m_TileInfoBuffer->SetData(m_vecTileInfo.data(), (int)m_vecTileInfo.size());

	// 타일 구조화 버퍼를 t20 에 바인딩
	m_TileInfoBuffer->UpdateData(20);

	// 하이라이트할 인덱스
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_2, m_IdxHighLight);
	InitHightLight();

	// 재질 업데이트
	GetMaterial()->UpdateData();	

	Transform()->UpdateData();

	GetMesh()->render();

	if (!m_pGrid->DidInit())
		m_pGrid->Init();
	if (m_bGridVisible && m_pGrid)//&& CRenderMgr::GetInst()->GetFirstCamera()->GetLayerCheck((UINT)GetOwner()->GetLayerIdx()))
	{
		m_pGrid->UpdateData();
	}
}

void CTileMap::begin()
{
	CRenderComponent::begin();
	if (nullptr == m_pGrid)
		return;
	
	auto LayerIdx = GetOwner()->GetLayerIdx();

	if (LAYER_FORETILE == LayerIdx)
	{
		m_pGrid->m_vGridColor = GamePlayStatic::Yellow;
	}
	else
	{
		m_pGrid->m_vGridColor = GamePlayStatic::Mazenta;
	}
}


void CTileMap::UpdateData()
{

}


void CTileMap::SetTileAtlas(Ptr<CTexture> _Atlas, Vec2 _TilePixelSize)
{

	m_MaxCol = _Atlas->GetWidth() / (UINT)_TilePixelSize.x;
	m_MaxRow = _Atlas->GetHeight() / (UINT)_TilePixelSize.y;

	Vec2 AtlasTileUV = Vec2(_TilePixelSize.x / _Atlas->GetWidth()
		, _TilePixelSize.y / _Atlas->GetHeight());

	std::pair< Ptr<CTexture>, Vec2> AtlasPair;
	AtlasPair.first = _Atlas;
	AtlasPair.second = AtlasTileUV;

	m_vecTileAtlas.push_back(AtlasPair);
}



void CTileMap::SetColRow(UINT _Col, UINT _Row)
{
	m_Col = _Col;
	m_Row = _Row;

	vector<tTileInfo> vecTemp;
	m_vecTileInfo.swap(vecTemp);
	m_vecTileInfo.resize(_Row * _Col);

	m_TileInfoBuffer->Create(sizeof(tTileInfo), _Row * _Col, SB_TYPE::READ_ONLY, true);
}

void CTileMap::SetTileIndex(UINT _Row, UINT _Col, int _ImgIdx)
{
	//if (0 == m_vecTileAtlas.size())
	//	return;

	UINT idx = _Row* m_Col + _Col;

	// 렌더링할 타일 정보
	UINT iRow = _ImgIdx / m_MaxCol;
	UINT iCol = _ImgIdx % m_MaxCol;

	//UV방식->idx방식
	//m_vecTileInfo[idx].TileIdx =_ImgIdx;

	//m_vecTileInfo[idx].vLeftTopUV = Vec2((iCol * m_vTilePixelSize.x) / m_TileAtlas->GetWidth()
	//							  , (iRow * m_vTilePixelSize.y) / m_TileAtlas->GetHeight());

	//m_vecTileInfo[idx].bRender = 1;

	//if (!m_pGrid)
	//	m_pGrid = new CTileMapGrid(this); //Test
	m_pGrid->Init();
}

void CTileMap::SetTileIndexWithOutGridInit(UINT _Row, UINT _Col, int _ImgIdx)
{
	//if (nullptr == m_TileAtlas)
	//	return;

	UINT idx = _Row * m_Col + _Col;

	// 렌더링할 타일 정보
	UINT iRow = _ImgIdx / m_MaxCol;
	UINT iCol = _ImgIdx % m_MaxCol;

	//UV방식->idx방식
	//m_vecTileInfo[idx].TileIdx = _ImgIdx;

	//m_vecTileInfo[idx].vLeftTopUV = Vec2((iCol * m_vTilePixelSize.x) / m_TileAtlas->GetWidth()
	//	, (iRow * m_vTilePixelSize.y) / m_TileAtlas->GetHeight());

	//m_vecTileInfo[idx].bRender = 1;

}

void CTileMap::GridInit()
{
	m_pGrid->Init();
}



void CTileMap::SaveToFile(FILE* _File)
{
	// TileMap 정보 저장
	fwrite(&m_Col, sizeof(UINT), 1, _File);
	fwrite(&m_Row, sizeof(UINT), 1, _File);
	fwrite(&m_vTileRenderSize, sizeof(Vec2), 1, _File);
	fwrite(&m_vTileRenderSize, sizeof(Vec2), 1, _File);

	size_t AtlasCount = m_vecTileAtlas.size();
	fwrite(&AtlasCount, sizeof(size_t), 1, _File);
	for (auto& p : m_vecTileAtlas)
	{
		SaveAssetRef(p.first, _File);
		fwrite(&p.second, sizeof(Vec2), 1, _File);
	}
	SaveAssetRef(GetArrAtlas(), _File);

	fwrite(&m_vTilePixelSize, sizeof(Vec2), 1, _File);
	//fwrite(&m_vSliceSizeUV, sizeof(Vec2), 1, _File);

	fwrite(&m_MaxCol, sizeof(UINT), 1, _File);
	fwrite(&m_MaxRow, sizeof(UINT), 1, _File);
	
	size_t InfoCount = m_vecTileInfo.size();
	fwrite(&InfoCount, sizeof(size_t), 1, _File);
	fwrite(m_vecTileInfo.data(), sizeof(tTileInfo), InfoCount, _File);
}

void CTileMap::LoadFromFile(FILE* _File)
{
	// TileMap 정보 저장
	fread(&m_Col, sizeof(UINT), 1, _File);
	fread(&m_Row, sizeof(UINT), 1, _File);
	fread(&m_vTileRenderSize, sizeof(Vec2), 1, _File);
	fread(&m_vTileRenderSize, sizeof(Vec2), 1, _File);

	size_t AtlasCount = m_vecTileAtlas.size();
	fread(&AtlasCount, sizeof(size_t), 1, _File);
	m_vecTileAtlas.resize(AtlasCount);
	for (auto& p : m_vecTileAtlas)
	{
		LoadAssetRef(p.first, _File);
		fread(&p.second, sizeof(Vec2), 1, _File);
	}
	LoadAssetRef(m_arrAtlas, _File);
	//auto pTileAtlas = CAssetMgr::GetInst()->Load<CTexture>(L"TileMapTextureArray",L"");
	//SetArrAtlas(pTileAtlas);


	fread(&m_vTilePixelSize, sizeof(Vec2), 1, _File);
	//fread(&m_vSliceSizeUV, sizeof(Vec2), 1, _File);

	fread(&m_MaxCol, sizeof(UINT), 1, _File);
	fread(&m_MaxRow, sizeof(UINT), 1, _File);

	size_t InfoCount = 0;
	fread(&InfoCount, sizeof(size_t), 1, _File);
	m_vecTileInfo.reserve(InfoCount);
	fread(m_vecTileInfo.data(), sizeof(tTileInfo), InfoCount, _File);
}

void CTileMap::ClearTile()
{
	for (auto& ti : m_vecTileInfo)
	{
		ti.AtlasIdx = -1;
		ti.TileIdx = -1;
	}
}

void CTileMap::CreateTileFab()
{
	const static wstring extention = L".txt";
	// Level 을 저장할 경로
	wstring strFabPath = CPathMgr::GetContentPath();
	strFabPath += STR_KEY_TILEFAB_PATH;
	strFabPath += GetOwner()->GetName();
	strFabPath += extention;


	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFabPath.c_str(), L"wb");

	//트랜스폼 정보저장
	auto rpos = Transform()->GetRelativePos();
	fwrite(&rpos, sizeof(Vec3), 1, pFile);

	//타일info 저장
	size_t InfoCount = m_vecTileInfo.size();
	fwrite(m_vecTileInfo.data(), sizeof(tTileInfo), InfoCount, pFile);

	fclose(pFile);
}

void CTileMap::LoadTileFab()
{
	const static wstring extention = L".txt";
	// Level 을 저장할 경로
	wstring strFabPath = CPathMgr::GetContentPath();
	strFabPath += STR_KEY_TILEFAB_PATH;
	strFabPath += GetOwner()->GetName();
	strFabPath += extention;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFabPath.c_str(), L"rb");
	//assert(pFile);

	//트랜스폼 정보 불러오기
	Vec3 rpos =Vec3();
	fread(&rpos, sizeof(Vec3), 1, pFile);
	Transform()->SetRelativePos(rpos);

	//타일info 불러오기
	size_t InfoCount = m_vecTileInfo.size();
	fread(m_vecTileInfo.data(), sizeof(tTileInfo), InfoCount, pFile);

	fclose(pFile);
}
