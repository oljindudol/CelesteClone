#pragma once
#include "CRenderComponent.h"

class CStructuredBuffer;
class CTileMapGrid;


struct tTileInfo
{
    int TileType;
    int AtlasIdx;
    int TileIdx;
    int padding;

    tTileInfo()
        :TileType(0),
        AtlasIdx(0),
        TileIdx(-1),
        padding(0)
    {

    }
};



class CTileMap :
    public CRenderComponent
{
private:
    // ���� Ÿ�� ����
    UINT                m_Row;            
    // ���� Ÿ�� ����
    UINT                m_Col;            
    // Ÿ�� 1ĭ ������
    Vec2                m_vTileRenderSize;  

    //pair : Ptr�ؽ��� , ��Ʋ�� Ÿ�ϻ����� UV
    vector<std::pair<Ptr<CTexture>,Vec2>> m_vecTileAtlas;
    Vec2                m_vTilePixelSize;
    //Vec2                m_vSliceSizeUV;

    UINT                m_MaxCol;
    UINT                m_MaxRow;

    vector<tTileInfo>   m_vecTileInfo;
    CStructuredBuffer*  m_TileInfoBuffer;

    bool m_bGridVisible;
    CTileMapGrid* m_pGrid;
    int m_IdxHighLight;

public:


    void SetRowCol(UINT _Row, UINT _Col);
    UINT GetRow() { return m_Row; }
    UINT GetCol() { return m_Col; }

    void SetTileIndex(UINT _Row, UINT _Col, int _ImgIdx);
    void SetTileIndexWithOutGridInit(UINT _Row, UINT _Col, int _ImgIdx);
    void GridInit();

    // Ÿ�� 1ĭ ������
    Vec2 GetTileSize() {
        return m_vTileRenderSize;
    }

    // Ÿ�� 1ĭ ������
    void SetTileSize(Vec2 _vTileRenderSize) {
        m_vTileRenderSize = _vTileRenderSize;
    }

    vector<tTileInfo>& GetTilesInfo() {
        return m_vecTileInfo;
    }


    void SetTilesInfo(vector<tTileInfo>& _vecTileInfo)
    {
        m_vecTileInfo = _vecTileInfo;
    }

    void SetIdxHighLight(int _idx)
    {
        m_IdxHighLight = _idx;
    }
    
    void InitHightLight()
    {
        m_IdxHighLight = -1;
    }

    //��Ʋ�󽺰����Լ�
    auto& GetTileAtlases() { return m_vecTileAtlas; }
    void SetTileAtlas(Ptr<CTexture> _Atlas,Vec2 _AtlasPixelSize = Vec2(8.f, 8.f));
    auto& GetTileAtlas(UINT _idx) { 
        //true�� ���
        assert(_idx < m_vecTileAtlas.size());
        return m_vecTileAtlas[_idx].first;
    }
    Vec2 GetAtlasTileSize(int _idx) {
        //true�� ���
        assert(_idx < m_vecTileAtlas.size());
        return m_vecTileAtlas[_idx].second;
    }

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CTileMap);
public:
    CTileMap();
    CTileMap(const CTileMap& _OriginTileMap);
    ~CTileMap();
};

