#pragma once
#include "UI.h"

class CTileMap;

#define IMGUI_COLOR_GREEN IM_COL32(10, 240, 10, 128)
#define IMGUI_COLOR_RED IM_COL32(240, 10, 10, 128)

struct MaskInfo
{
	int neighbourMask;
	int neighbourCount;
	int extendedNeighbourCount;
	int emptyNeighbourSlot;

	MaskInfo():neighbourMask(0), neighbourCount(0), extendedNeighbourCount(0), emptyNeighbourSlot(0){}

};

enum class MaskInfoReflection
{
	TOP,
	LEFT,
	RIGHT,
	BOTTOM,

	TOPLEFT,
	TOPRIGHT,
	BOTTOMLEFT,
	BOTTOMRIGHT,

	TOP2,
	LEFT2,
	RIGHT2,
	BOTTOM2,
};


class TileMapEditor :
    public UI
{
public:
	void SetTargetObject(CGameObject* _pTargetObj) { m_pTargetObject = _pTargetObj; }
	CGameObject* GetTargetObject() { return m_pTargetObject; }

private:
	CGameObject* m_pTargetObject;
	CTileMap* m_pTileMap;
	int m_IdxAtlas;
	CTexture* m_pAtlasTileTex;
	Vec2 m_vAtlasTileUvSize;

	int m_arrFaceTileCnt[2];
	int m_vAtlasTilePixelSize[2];
	int m_iBrushSize;
	bool m_bDeleteMode;
	int m_iSelectedTileIdx; // 선택한 타일의 인덱스
	int m_iGridColor;

private:
	int neighbourOffsets[24];
	//idx, 랜덤 아틀라스 갯수
	std::pair<int, int> IdxPair[21];

public:
	virtual void render_update() override;
	virtual void Activate() override;
	// Neighbouring Tiles	

private:
	bool _IsTileSelectedInCanvas() { 
		return m_iSelectedTileIdx < 0 ? false : true; 
	}
	void _RenderPalette();
	void _SelectTexture(DWORD_PTR _strKey, DWORD_PTR _NONE);
	void _Clear();
	void _OptimizeCollisionArea(); // 충돌영역 타일 최적화하기
	MaskInfo& _GetMaskInfo(int _x, int _y , bool _bSimplify =true);
	string ToBinaryString(int _Mask);
	void ReflectMaskInfo(int _Mask);
	tTileInfo* _GetTile(int _x, int _y);
	void _AutoTile(); // 오토타일
	void GetEndIdxOfRectArea(int** _grid, int _startX, int _startY, int& _endX, int& _endY);

public:
	TileMapEditor();
	virtual ~TileMapEditor() override;
};