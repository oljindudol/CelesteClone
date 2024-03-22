#include "pch.h"
#include "TileMapUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTileMap.h>
#include "TileMapEditor.h"

TileMapUI::TileMapUI()
	: ComponentUI("TileMap", "##TileMap", COMPONENT_TYPE::TILEMAP)
{
	SetSize(ImVec2(0.f, 120.f));
	SetComopnentTitle("TileMap");
}

TileMapUI::~TileMapUI()
{
}

void TileMapUI::render_update()
{
	if (nullptr == GetTargetObject())
		return;

	ComponentUI::render_update();


	//============= 1. 타일갯수 =============

	static int Face[2] = { 0, 0 };

	static int FacePrev[2] = { 0, 0 };

	Face[0] = (int)GetTargetObject()->TileMap()->GetRow();
	Face[1] = (int)GetTargetObject()->TileMap()->GetCol();

	ImGui::Text("Face"); ImGui::SameLine();  ImGui::DragInt2("##Face", Face);

	int min = 1;

	if (Face[0] < min)
	{
		Face[0] = min;
	}

	if (Face[1] < min)
	{
		Face[1] = min;
	}

	if (!(FacePrev[0] == Face[0] && FacePrev[1] == Face[1]))
	{
		GetTargetObject()->TileMap()->SetRowCol((UINT)Face[0], (UINT)Face[1]);

		for (int i = 0; i < Face[1]; ++i)
		{
			for (int j = 0; j < Face[0]; ++j)
			{
				GetTargetObject()->TileMap()->SetTileIndexWithOutGridInit(i, j, i * Face[0] + j);
			}
		}
		GetTargetObject()->TileMap()->GridInit();
	}





	//============= 2. 타일 크기 =============
	static float TileSize[2];
	TileSize[0] = GetTargetObject()->TileMap()->GetTileSize().x;
	TileSize[1] = GetTargetObject()->TileMap()->GetTileSize().y;

	ImGui::Text("TileSize"); ImGui::SameLine();  ImGui::DragFloat2("##TileSize", TileSize);

	int minsize = 1.f;

	if (Face[0] < minsize)
	{
		TileSize[0] = minsize;
	}

	if (TileSize[1] < minsize)
	{
		TileSize[1] = minsize;
	}

	GetTargetObject()->TileMap()->SetTileSize(Vec2(TileSize[0], TileSize[1]));

	//============= 3. 에디터 열기 ============

	if (ImGui::Button("Open TileMap Editor"))
	{
		TileMapEditor* pEdit = (TileMapEditor*)CImGuiMgr::GetInst()->FindUI(STR_COMPONENT_UI_TileMapEditor);
		CGameObject* pTarget = GetTargetObject();
		pEdit->SetTargetObject(pTarget);
		pEdit->Activate();
	}

}

