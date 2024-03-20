#include "pch.h"
#include "TileMapUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTileMap.h>

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


	//============= 1. Ÿ�ϰ��� =============

	static int Face[2];
	Face[0] = (int)GetTargetObject()->TileMap()->GetFaceX();
	Face[1] = (int)GetTargetObject()->TileMap()->GetFaceY();

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

	GetTargetObject()->TileMap()->SetFace((UINT)Face[0], (UINT)Face[1]);

	for (int i = 0; i < Face[1]; ++i)
	{
		for (int j = 0; j < Face[0]; ++j)
		{
			GetTargetObject()->TileMap()->SetTileIndex(i, j, i * Face[0] + j);
		}
	}


	//============= 2. Ÿ�� ũ�� =============
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

}

