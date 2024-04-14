#pragma once

class CGameObject;
class CTileMap;


class CTileMapGrid
{
private:
	CTileMap* m_pTargetTileMap;

private:
	vector<Vtx> m_vecVtx; // ¡§¡°
	vector<int> m_vecIdx; // ¿Œµ¶Ω∫

	vector<CMesh*> m_vecMesh; // Line Mesh
	Ptr<CMaterial> m_pMtrl;

	Vector4 m_vGridColor;

	bool m_bDidInit;


public:
	void Init();
	void AddBuffer(const Vector3& _vPos1, const Vector3& _vPos2);
	void Clear();

public:
	void UpdateData();
	void SetTargetTileMap(CTileMap* _pTargetTileMap) { m_pTargetTileMap = _pTargetTileMap; }
	bool DidInit() { return m_bDidInit; }

public:
	CLONE(CTileMapGrid);
	CTileMapGrid(CTileMap* _pTargetTileMap);
	CTileMapGrid(const CTileMapGrid& _origin);
	virtual ~CTileMapGrid();

	friend class CTileMap;
};