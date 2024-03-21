#pragma once
#include "CRenderComponent.h"

class CStructuredBuffer;
class CTileMapGrid;


struct tTileInfo
{
    Vec2 vLeftTopUV;    
    int  bRender;
    int  padding;
};


class CTileMap :
    public CRenderComponent
{
private:
    UINT                m_FaceX;            // ���� Ÿ�� ����
    UINT                m_FaceY;            // ���� Ÿ�� ����
    Vec2                m_vTileRenderSize;  // Ÿ�� 1ĭ ������

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

    void SetFace(UINT _FaceX, UINT _FaceY);
    UINT GetFaceX() { return m_FaceX; }
    UINT GetFaceY() { return m_FaceY; }

    void SetTileIndex(UINT _Row, UINT _Col, UINT _ImgIdx);
    void SetTileIndexWithOutGridInit(UINT _Row, UINT _Col, UINT _ImgIdx);
    void GridInit();

    Vec2 GetTileSize() {
        return m_vTileRenderSize;
    }

    void SetTileSize(Vec2 _vTileRenderSize) {
        m_vTileRenderSize = _vTileRenderSize;
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

