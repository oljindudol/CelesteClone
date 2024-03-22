#pragma once
#include "CRenderComponent.h"

class CStructuredBuffer;
class CTileMapGrid;


struct tTileInfo
{
    int AtlasType;
    int AtlasIdx;
    int TileIdx;
    int padding;
};



class CTileMap :
    public CRenderComponent
{
private:
    // 가로 타일 개수
    UINT                m_Row;            
    // 세로 타일 개수
    UINT                m_Col;            
    // 타일 1칸 사이즈
    Vec2                m_vTileRenderSize;  

    Ptr<CTexture>       m_TileAtlas;
    Vec2                m_vTilePixelSize;
    Vec2                m_vSliceSizeUV;

    UINT                m_MaxCol;
    UINT                m_MaxRow;

    vector<tTileInfo>   m_vecTileInfo;
    CStructuredBuffer*  m_TileInfoBuffer;

    bool m_bGridVisible;
    CTileMapGrid* m_pGrid;

public:
    void SetTileAtlas(Ptr<CTexture> _Atlas, Vec2 _TilePixelSize);
    auto& GetTileAtlas() { return m_TileAtlas; }

    void SetRowCol(UINT _Row, UINT _Col);
    UINT GetRow() { return m_Row; }
    UINT GetCol() { return m_Col; }

    void SetTileIndex(UINT _Row, UINT _Col, int _ImgIdx);
    void SetTileIndexWithOutGridInit(UINT _Row, UINT _Col, int _ImgIdx);
    void GridInit();

    // 타일 1칸 사이즈
    Vec2 GetTileSize() {
        return m_vTileRenderSize;
    }

    // 타일 1칸 사이즈
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

