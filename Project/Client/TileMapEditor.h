#pragma once
#include "UI.h"

class CTileMap;

#define IMGUI_COLOR_GREEN IM_COL32(10, 240, 10, 128)
#define IMGUI_COLOR_RED IM_COL32(240, 10, 10, 128)

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
	int m_iSelectedTileIdx; // ������ Ÿ���� �ε���
	int m_iGridColor;

private:
	// Tile Canvas�� ���� ��� ����

public:
	virtual void render_update() override;
	virtual void Activate() override;


private:
	bool _IsTileSelectedInCanvas() { 
		return m_iSelectedTileIdx < 0 ? false : true; 
	}
	void _RenderPalette();
	void _SelectTexture(DWORD_PTR _strKey, DWORD_PTR _NONE);
	void _Clear();
	void _OptimizeCollisionArea(); // �浹���� Ÿ�� ����ȭ�ϱ�
	tTileInfo* _GetTile(int _x, int _y);
	void _AutoTile(); // ����Ÿ��
	void GetEndIdxOfRectArea(int** _grid, int _startX, int _startY, int& _endX, int& _endY);

public:
	TileMapEditor();
	virtual ~TileMapEditor() override;
};