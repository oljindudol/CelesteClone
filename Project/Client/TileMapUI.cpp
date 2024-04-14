#include "pch.h"
#include "TileMapUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTileMap.h>
#include "TileMapEditor.h"
#include <Engine\CKeyMgr.h>

TileMapUI::TileMapUI()
	: ComponentUI("TileMap", "##TileMap", COMPONENT_TYPE::TILEMAP)
{
	SetSize(ImVec2(0.f, 300.f));
	SetComopnentTitle("TileMap");
}

TileMapUI::~TileMapUI()
{
}

void TileMapUI::Deactivate()
{
	ComponentUI::Deactivate();
}

void TileMapUI::render_update()
{
	if (nullptr == GetTargetObject())
		return;

	ComponentUI::render_update();

	if (ImGui::Button("ClearTIle"))
	{
		GetTargetObject()->TileMap()->ClearTile();
	}
	if (ImGui::Button("CreateTileFab"))
	{
		GetTargetObject()->TileMap()->CreateTileFab();
	}
	if (ImGui::Button("LoadTileFab"))
	{
		GetTargetObject()->TileMap()->LoadTileFab();
	}


	//============= 1. 타일갯수 =============

	static int Face[2] = { 0, 0 };

	static int FacePrev[2] = { 0, 0 };


	//auto visible = GetTargetObject()->TileMap()->GetGridVisible();
	//ImGui::Text("Grid Visible"); ImGui::SameLine();
	//ImGui::Checkbox("##Grid Visible", &visible);


	if (KEY_TAP(KEY::G))
	{
		GetTargetObject()->TileMap()->SetGridVisible(!GetTargetObject()->TileMap()->GetGridVisible());
	}

	Face[0] = (int)GetTargetObject()->TileMap()->GetCol();
	Face[1] = (int)GetTargetObject()->TileMap()->GetRow();
	int NewFace[2] = { Face[0] , Face[1] };


	ImGui::Text("Face"); ImGui::SameLine();  ImGui::DragInt2("##Face", NewFace);

	int min = 1;

	if (NewFace[0] < min)
	{
		NewFace[0] = min;
	}

	if (NewFace[1] < min)
	{
		NewFace[1] = min;
	}
	if (Face[0] != NewFace[0] || Face[1] != NewFace[1])
	{
		GetTargetObject()->TileMap()->SetColRow(NewFace[0], NewFace[1]);
		GetTargetObject()->TileMap()->GridInit();
	}



	//if (!(FacePrev[0] == Face[0] && FacePrev[1] == Face[1]))
	//{
	//	GetTargetObject()->TileMap()->SetRowCol((UINT)Face[0], (UINT)Face[1]);
	//
	//	//for (int i = 0; i < Face[1]; ++i)
	//	//{
	//	//	for (int j = 0; j < Face[0]; ++j)
	//	//	{
	//	//		GetTargetObject()->TileMap()->SetTileIndexWithOutGridInit(i, j, i * Face[0] + j);
	//	//	}
	//	//}
	//	GetTargetObject()->TileMap()->GridInit();
	//}

	FacePrev[0] = Face[0];
	FacePrev[1] = Face[1];

	//============= 2. 타일 크기 =============
	static float TileSize[2];
	TileSize[0] = GetTargetObject()->TileMap()->GetTileSize().x;
	TileSize[1] = GetTargetObject()->TileMap()->GetTileSize().y;

	ImGui::Text("TileSize"); ImGui::SameLine();  ImGui::DragFloat2("##TileSize", TileSize);

	float minsize = 1.f;

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

void TileMapUI::Activate()
{
	UI::Activate();
}

