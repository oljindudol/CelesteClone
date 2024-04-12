#include "pch.h"
#include "TileMapEditor.h"

//#include "CImGuiMgr.h"
#include <Engine/CTileMap.h>
#include <Engine/CGameObject.h>
#include <Engine/CAssetMgr.h>
//#include "ParamGUI.h"
//#include "DebugGUI.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CTexture.h>
#include <Engine\CCollider2D.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CTaskMgr.h>
//#include <Engine\CObjectManager.h>
//#include <Engine\CCursor.h>
//#include <Engine\CSceneManager.h>
//#include <Engine\CScene.h>


TileMapEditor::TileMapEditor()
    : UI("Edit Tile", STR_COMPONENT_UI_TileMapEditor),
	m_pTargetObject(nullptr),
	m_pTileMap{ nullptr },
	m_pAtlasTileTex{ nullptr },
	m_vAtlasTileUvSize{},
	m_arrFaceTileCnt{ 0,0 },
	m_vAtlasTilePixelSize{ 0, 0 },
	m_iBrushSize(0),
	m_bDeleteMode{ false },
	m_iSelectedTileIdx(-1),
	m_IdxAtlas(0),
	// canvas
	m_iGridColor(IMGUI_COLOR_GREEN) // green
{
    Deactivate();
    SetModal(false);
}

TileMapEditor::~TileMapEditor()
{
}


void TileMapEditor::render_update()
{
	if (!m_pTargetObject)
		return;
	if (m_pTargetObject->IsDead())
		return;
	if (!m_pTargetObject->TileMap())
		return;


	m_pTileMap = m_pTargetObject->TileMap();
	auto& vecAtlases = m_pTileMap->GetTileAtlases();

	bool validAtlasIdx = true;

	vector<string> items;

	for (auto& p : vecAtlases)
	{
		items.push_back(ToString(p.first.Get()->GetKey()));
	}


	//idx가 범위를넘어가면 일단 0으로 초기화
	if (m_IdxAtlas >= vecAtlases.size())
	{
		m_IdxAtlas = 0;
	}
	// size가 0이면 idx가 유효하지않음
	if (0 == vecAtlases.size())
	{
		validAtlasIdx = false;
	}

	ImGui::SetNextItemWidth(150);
	if (ImGui::BeginCombo("##combo", items[m_IdxAtlas].c_str())) {
		for (int i = 0; i < items.size(); i++) {
			const bool isSelected = (m_IdxAtlas == i);
			if (ImGui::Selectable(items[i].c_str(), isSelected)) {
				m_IdxAtlas = i;
			}

			// 항목 선택 시 자동으로 스크롤
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	vector<std::pair<Ptr<CTexture>, Vec2>> pair;


	if (true == validAtlasIdx)
	{
		m_pAtlasTileTex = m_pTileMap->GetTileAtlas(m_IdxAtlas).Get();
		m_vAtlasTileUvSize = m_pTileMap->GetAtlasTileSize(m_IdxAtlas);
	}
	else
	{
		m_pAtlasTileTex = nullptr;
	}

	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);

	//에디터카메라 오쏘뷰고정
	CCamera* pCam;
	if (LEVEL_STATE::PLAY == CLevelMgr::GetInst()->GetCurrentLevel()->GetState())
	{
		pCam = CRenderMgr::GetInst()->GetFirstCamera();
	}
	else
	{
		pCam = CRenderMgr::GetInst()->GetEditorCamera();
	}

	if (pCam) {
		if (PROJ_TYPE::PERSPECTIVE == pCam->GetProjType())
			pCam->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	}

	//===============1. 타일맵 정보==============
	ImVec2 vAtlasTexResol = {};
	if (m_pAtlasTileTex) {
		ImVec2((float)m_pAtlasTileTex->GetWidth(), (float)m_pAtlasTileTex->GetHeight());
	}
	ImGui::Text("Atlas Texture Resolution [%d,%d]", vAtlasTexResol.x, vAtlasTexResol.x);

	ImGui::Text("Tile Row:%d  Col:%d", m_pTileMap->GetRow(), m_pTileMap->GetCol());


	//===============2. 타일맵 재생성기==============
	//
	//ImGui::InputInt2("##Tile Map Size", m_arrFaceTileCnt);
	//m_arrFaceTileCnt[0] = 
	// (m_arrFaceTileCnt[0], 0, INT_MAX);
	//m_arrFaceTileCnt[1] = ClampInt(m_arrFaceTileCnt[1], 0, INT_MAX);

	//ImGui::Spacing();
	//
	//ImGui::Text("Texture Tile Size");
	//ImGui::InputInt2("##Texture Tile Size", m_vAtlasTilePixelSize);
	//m_vAtlasTilePixelSize[0] = CMyMath::Clamp(m_vAtlasTilePixelSize[0], 0, INT_MAX);
	//m_vAtlasTilePixelSize[1] = CMyMath::Clamp(m_vAtlasTilePixelSize[1], 0, INT_MAX);
	//ImGui::Spacing();
	//
	//// 타일 생성
	//static bool bBlankCreate = true;
	//ImGui::Checkbox("Blank Tile##TileMapEditorCreateInitType", &bBlankCreate);
	//ImGui::SameLine();
	//if (ImGui::Button("Create")) {
	//	if (0 != m_arrFaceTileCnt[0] &&
	//		0 != m_arrFaceTileCnt[1] &&
	//		0 != m_vAtlasTilePixelSize[0] &&
	//		0 != m_vAtlasTilePixelSize[1] &&
	//		nullptr != m_pTileMap->GetAtlasTexture().Get()) {
	//		m_pTileMap->SetAtlasTilePixelSize(m_vAtlasTilePixelSize[0], m_vAtlasTilePixelSize[1]);
	//		m_pTileMap->CreateTile(m_arrFaceTileCnt[0], m_arrFaceTileCnt[1], bBlankCreate);
	//	}
	//}


	//===============3. 아틀라스 선택기==============
		//ImGui::Separator();
	//
	//ImGui::Text("Mouse Right button clicking : delete mode");
	//ImGui::Text("Brush Size : %d", m_iBrushSize + 1);
	//ImGui::DragInt("##TileMap Brush Size", &m_iBrushSize, 1.f, 0, 10);
	//
	//// 아틀라스 텍스쳐 선택
	//// ListView선택.
	//
	//// 아틀라스 텍스쳐 세팅
	//CTexture* pAtlasTexture = m_pTileMap->GetAtlasTexture().Get();
	//if (pAtlasTexture) {
	//	string strName{};
	//	tstring tstrName = pAtlasTexture->GetName();
	//	TStringToString(tstrName, strName);
	//	ParamGUI::Render_Texture(strName.c_str(), pAtlasTexture, this, (GUI_CALLBACK)&TileMapEditorGUI::_SelectTexture);
	//}
	//else {
	//	if (ImGui::Button("Atlas Texture Select")) {
	//		// 목록 전달
	//		vector<tstring> vecName;
	//		CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Texture, vecName);
	//		ListViewGUI* pListGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
	//		assert(pListGUI);
	//		if (pListGUI) {
	//			pListGUI->SetList(vecName, _T("Atlas Texture"));
	//			pListGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&TileMapEditorGUI::_SelectTexture);
	//			pListGUI->SetPreViewTexRender(true);
	//			pListGUI->SetActive(true);
	//		}
	//	}
	//}

	//===============4. 팔레트 선택기=============
	_RenderPalette();


	//===============5. 레벨 클릭 판정기=============
	ImGui::Separator();
	auto MousePosition = CKeyMgr::GetInst()->GetMousePos();
	//Vector3 vMouseWorldPos = pEditorCam->GetScreenToWorld2DPosition(MousePosition);
	//ImGui::Text("Abs Mouse X:%d  Y:%d", int(vMouseWorldPos.x), int(vMouseWorldPos.y));


	Vector3 vMouseWorldPos;
	vMouseWorldPos = pCam->GetScreenToWorld2DPosition(MousePosition);

	if (!pCam) {
		assert(pCam);
	}
	else {

		vector<tTileInfo>& vecTiles = m_pTileMap->GetTilesInfo();

		if (!ImGui::IsWindowFocused()) {
			m_bDeleteMode = false;
			// TODO(Jang) : 타일 클릭 충돌했는지 구하는 코드를 CollisionManager에 넣기
	
			Vector3 vObjWorldPos = GetTargetObject()->Transform()->GetWorldPos();
			Vector3 vScale = GetTargetObject()->Transform()->GetWorldScale();
			Vector3 vHalfScale = vScale * 0.5f;
			vHalfScale.x = fabsf(vHalfScale.x);
			vHalfScale.y = fabsf(vHalfScale.y);
	
			Vector3 vLBWorldPos = {};
			vLBWorldPos.x = vObjWorldPos.x - vHalfScale.x;
			vLBWorldPos.y = vObjWorldPos.y - vHalfScale.y;
	
			Vector2 vOffsetLB = -Vector2(vLBWorldPos.x, vLBWorldPos.y);
	
			Vector3 vRTWorldPos = {};
			vRTWorldPos.x = vLBWorldPos.x + GetTargetObject()->Transform()->GetWorldScale().x;
			vRTWorldPos.y = vLBWorldPos.y + GetTargetObject()->Transform()->GetWorldScale().y;
	
			// 마우스를 타일 내부에 클릭했는가?
			bool bIsTileClicked = false;
	
			if (KEY_PRESSED(KEY::LBTN)) {
				if (vLBWorldPos.x < vMouseWorldPos.x && vRTWorldPos.x > vMouseWorldPos.x &&
					vLBWorldPos.y < vMouseWorldPos.y && vRTWorldPos.y > vMouseWorldPos.y) {
					bIsTileClicked = true;
				}
	
			}
			if (KEY_PRESSED(KEY::RBTN))
			{
				m_bDeleteMode = true;
				bIsTileClicked = true;
			}

				Vector2 vOriginMousePos = Vector2(vMouseWorldPos.x + vOffsetLB.x, vScale.y - (vMouseWorldPos.y + vOffsetLB.y));
	
				Vector2 vOffsetScale = Vector2(m_pTileMap->GetRow() / vScale.x, m_pTileMap->GetCol() / vScale.y);
	
				vOriginMousePos *= vOffsetScale;
	
				int iClickX = (int)vOriginMousePos.x;
				int iClickY = (int)vOriginMousePos.y;
	
				// Brush를 이용하여 그리기
				int iMinX, iMinY, iMaxX, iMaxY;
				iMinX = iClickX - m_iBrushSize;
				iMaxX = iClickX + m_iBrushSize;
				iMinY = iClickY - m_iBrushSize;
				iMaxY = iClickY + m_iBrushSize;
				iMinX = max(0, iMinX);
				iMinY = max(0, iMinY);
				iMaxX = min(m_pTileMap->GetRow() - 1, iMaxX);
				iMaxY = min(m_pTileMap->GetCol() - 1, iMaxY);


				//if (m_bDeleteMode)
				//	vecTiles[idx].TileIdx = -1;
				//else
				//	vecTiles[idx].TileIdx = m_iSelectedTileIdx;
	
				for (int y = iMinY; y <= iMaxY; ++y) {
					for (int x = iMinX; x <= iMaxX; ++x) {
						int idx = y * m_pTileMap->GetRow() + x;

						m_pTileMap->SetIdxHighLight(idx);
						// 클릭했을 경우
						if (bIsTileClicked) {
						if (m_bDeleteMode)
							vecTiles[idx].TileIdx = -1;
						else
							vecTiles[idx].TileIdx = m_iSelectedTileIdx;
							vecTiles[idx].AtlasIdx = m_IdxAtlas;
						}
					}
				}
				//int idx = iClickY * m_pTileMap->GetCol() + iClickX;
				//vecTiles[idx].idx = m_iSelectedTileIdx;
		}

		//for (int i=0; i < vecTiles.size();++i)
		//{
		//	ImGui::Text(("Tile" + std::to_string(i) + ":"+ std::to_string(vecTiles[i].TileIdx) +std::to_string(vecTiles[i].AtlasIdx)).c_str());
		//}
	}

	//===============6. 컬라이더 생성기=============
	//if (ImGui::Button("CollderCreate##TileMap2D")) {
	//	CGameObject* pObj = CObjectManager::GetInstance()->CreateEmptyGameObject();
	//	pObj->SetName(L"new Collders");
	//	vector<TTileInfo>& vecTiles = m_pTileMap->GetTilesInfo();
	//	int iCol = m_pTileMap->GetCol();
	//	int iRow = m_pTileMap->GetRow();
	//	for (int y = 0; y < iRow; ++y) {
	//		for (int x = 0; x < iCol; ++x) {
	//			int idx = y * iCol + x;
	//			if (vecTiles[idx].idx >= 0) {
	//				// 생성
	//				CGameObject* pColObj = CObjectManager::GetInstance()->CreateEmptyGameObject();
	//				pColObj->AddComponent<CCollider2D>();
	//				TCHAR strName[255];
	//				_stprintf_s(strName, 255, _T("Col[%d,%d]"), x, y);
	//				pColObj->SetName(strName);
	//				pColObj->Transform()->SetLocalPosition(Vector3(x + 0.5f, iRow - y - 0.5f, 0));
	//				CObject::AddChildGameObjectEvn(pObj, pColObj);
	//			}
	//		}
	//	}
	//}
	if (ImGui::Button("OptimizeCollider##TileMap2D")) {
		_OptimizeCollisionArea();
	}
}

void TileMapEditor::_RenderPalette()
{
	static ImVec2 scrolling(0.0f, 0.0f);
	static bool opt_enable_grid = true;
	static bool opt_enable_context_menu = true;
	static bool adding_line = false;

	//static int arrGrid[2] = { 5, 5 };

	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);
	//CTexture* pTileTexture = m_pTileMap->GetTileAtlas().Get();
	Vector2 vAtlasSize{};
	Vector2 vAtlasTileSize{};
	int iAtlasColCnt;
	if (nullptr == m_pAtlasTileTex)
		return;

	//배율
	//float canvas_ratio = canvas_sz.y / canvas_sz.x;
	float mul = 1.f;
	if (m_pAtlasTileTex) {
		vAtlasSize = Vec2(m_pAtlasTileTex->GetWidth(), m_pAtlasTileTex->GetHeight());
		float atlas_ratio = vAtlasSize.y / vAtlasSize.x;

		//세로가길때,세로에맞추기
		if (1.f < atlas_ratio)
		{
			mul = canvas_sz.y / vAtlasSize.y;
		}
		else
		{
			mul = canvas_sz.x / vAtlasSize.x;
		}


		vAtlasSize = vAtlasSize * mul;
		vAtlasTileSize = m_vAtlasTileUvSize * vAtlasSize;
		iAtlasColCnt =int(1.f / m_vAtlasTileUvSize.x);
	}





	// 왼쪽 버튼을 클릭했으면
	if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		// 타일의 위치 값 계산 후 선택한 타일의 인덱스를 가져온다.

		// 타일을 클릭했는가?
		bool isTileClick = false;
		if (vAtlasSize.x > mouse_pos_in_canvas.x && vAtlasSize.y > mouse_pos_in_canvas.y &&
			0 < mouse_pos_in_canvas.x && 0 < mouse_pos_in_canvas.y)
			isTileClick = true;

		// 타일을 클릭하지 않았으면
		if (!isTileClick)
			m_iSelectedTileIdx = -1;
		else { // 타일을 클릭헀으면
			float iCol = mouse_pos_in_canvas.x / vAtlasTileSize.x;
			float iRow = mouse_pos_in_canvas.y / vAtlasTileSize.y;
			modf(iCol, &iCol);
			modf(iRow, &iRow);
			// 선택한 타일의 인덱스를 얻는다.
			m_iSelectedTileIdx = (int)iRow * iAtlasColCnt + (int)iCol;
		}
	}

	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	if (opt_enable_grid)
	{
		Vector2 vGridSize = m_pTileMap->GetTileSize();
		for (float x = fmodf(scrolling.x, vGridSize.x); x < canvas_sz.x; x += vGridSize.x)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), m_iGridColor);
		for (float y = fmodf(scrolling.y, vGridSize.y); y < canvas_sz.y; y += vGridSize.y)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), m_iGridColor);
	}

	// Draw Atlas Texture
	if (m_pAtlasTileTex) {
		ImVec2 vAtlasTexResolution = ImVec2(origin.x + vAtlasSize.x, origin.y + vAtlasSize.y);
		draw_list->AddImage(m_pAtlasTileTex->GetSRV().Get(), origin, vAtlasTexResolution);
	}

	// 타일이 선택 되었으면
	if (_IsTileSelectedInCanvas()) {

		int iRow = m_iSelectedTileIdx / iAtlasColCnt;
		int iCol = m_iSelectedTileIdx % iAtlasColCnt;

		ImVec2 vLTPos = ImVec2(iCol * vAtlasTileSize.x, iRow * vAtlasTileSize.y);
		ImVec2  vRBPos = ImVec2((iCol + 1) * vAtlasTileSize.x, (iRow + 1) * vAtlasTileSize.y);

		vLTPos = ImVec2(vLTPos.x + origin.x, vLTPos.y + origin.y);
		vRBPos = ImVec2(vRBPos.x + origin.x, vRBPos.y + origin.y);
		draw_list->AddRect(vLTPos, vRBPos, IMGUI_COLOR_RED, 0.f, 0, 2.f);
	}

	draw_list->PopClipRect();
}


void TileMapEditor::_SelectTexture(DWORD_PTR _strKey, DWORD_PTR _NONE)
{
}

void TileMapEditor::_Clear()
{
	m_pTargetObject = nullptr;
	m_pTileMap = nullptr;
	m_pAtlasTileTex = nullptr;
	m_arrFaceTileCnt[0] = m_arrFaceTileCnt[1] = 0;
	m_vAtlasTilePixelSize[0] = m_vAtlasTilePixelSize[1] = 0;
	m_iSelectedTileIdx = -1;
}

enum class E_VisitedState { not_visited, visited };

void TileMapEditor::_OptimizeCollisionArea()
{
	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"new Collders");
	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));

	auto ownpos = m_pTileMap->Transform()->GetWorldPos();
	auto ownscale = m_pTileMap->Transform()->GetWorldScale();

	auto TilePixelSize = m_pTileMap->GetTileSize();

	Vec2 TobjLT = Vec2(ownpos.x, ownpos.y) - Vec2(ownscale.x, -ownscale.y) * 0.5f;
	//test
	//CGameObject* pChild = new CGameObject;
	//pChild->SetName(L"child");
	//pChild->AddComponent(new CTransform);
	//pChild->AddComponent(new CCollider2D);

	//pChild->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	//pChild->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
	//pChild->Collider2D()->SetOffsetScale(Vec2(1, 1));
	//pChild->Collider2D()->SetOffsetPos(Vec2(0, 0));
	//pObj->AddChild(pChild);
	//
	//GamePlayStatic::SpawnGameObject(pObj, LAYER_TILECOL);
	//return;

	const vector<tTileInfo>& vecTiles = m_pTileMap->GetTilesInfo();
	//int iCol = m_pTileMap->GetCol();
	//int iRow = m_pTileMap->GetRow();

	int iRow = m_pTileMap->GetCol();
	int iCol = m_pTileMap->GetRow();

	// 최적화를 하기 위해서 map 생성

	// 2차원 배열 동적할당.
	int** grid = new int* [iRow];
	for (int i = 0; i < (int)iRow; ++i)
		grid[i] = new int[(int)iCol];

	for (int i = 0; i < (int)iRow; ++i)
		memset(grid[i], 0, sizeof(int) * iCol);

	// 잡힌 영역을 저장할 벡터

	// grid type -> 0 : not visited, 1: visited
	vector<std::pair<RECT, Vector2>> vecPos; // 콜라이더를 생성시킬 벡터 ( second : 위치)
	// collider 영역 잡기.
	int startPosX = 0;
	int startPosY = 0;
	int endPosX = 0;
	int endPosY = 0;
	for (int y = 0; y < (int)iRow; ++y) {
		for (int x = 0; x < (int)iCol; ++x) {
			endPosX = startPosX = x;
			endPosY = startPosY = y;

			const tTileInfo& pTile = vecTiles[startPosY * iCol + startPosX];
			if ((-1 < pTile.TileIdx) && (int)E_VisitedState::not_visited == grid[startPosY][startPosX]) {
				GetEndIdxOfRectArea(grid, startPosX, startPosY, endPosX, endPosY);
				vecPos.push_back(std::make_pair(RECT{ startPosX,startPosY,endPosX,endPosY }, Vector2(x, y)));
			}
			grid[startPosY][startPosX] = (int)E_VisitedState::visited;
		}
	}

	// 콜라이더 설정
	for (UINT i = 0; i < vecPos.size(); ++i) {
		RECT tRect = vecPos[i].first;
		Vector2 vPos = vecPos[i].second;

		const tTileInfo& pTile = vecTiles[tRect.top * iCol + tRect.left];


		// 콜라이더 오브젝트 생성
		CGameObject* pColObj = new CGameObject;
		auto trans = new CTransform;
		pColObj->AddComponent(trans);
		pColObj->AddComponent(new CCollider2D);
		WCHAR strName[255];
		swprintf_s(strName, 255, L"Col[%d,%d]", (int)vPos.x, (int)vPos.y);
		pColObj->SetName(strName);

		//Vector2 offset{ 0.5f, -y - 0.5f };
		//pColObj->Transform()->SetLocalPosition(Vector3(x + 0.5f, iRow - y - 0.5f, 0));
		pObj->AddChild(pColObj);

		float scaleX = (float)(tRect.right + 1 - tRect.left)* TilePixelSize.x;
		float scaleY = (float)(tRect.bottom + 1 - tRect.top) * TilePixelSize.y;

		//trans->SetAbsolute(true);
		//Vector2 vOffsetPos = Vector2(scaleX * 0.5f, scaleY * 0.5f);
		//float maxX = iCol * 0.5f;
		//float maxY = iRow * 0.5f;
		//vOffsetPos.x -= maxX;
		//vOffsetPos.y -= maxY;
		//Vector3 vResultPos = Vector3(vPos.x + vOffsetPos.x, vPos.y + vOffsetPos.y, ownpos.z);
		Vector3 vResultPos = Vector3(TobjLT.x + (vPos.x * TilePixelSize.x + scaleX *0.5f)
			, TobjLT.y - (vPos.y * TilePixelSize.y + scaleY *0.5f) , ownpos.z);

		//pColObj->Collider2D()->SetAbsolute(false);
		trans->SetRelativeScale(Vector3(scaleX, scaleY, 0.f));
		trans->SetRelativePos(vResultPos);
		pColObj->Collider2D()->SetOffsetPos(Vec2(0, 0));
		pColObj->Collider2D()->SetOffsetScale(Vector2(1, 1));
	}

	for (int i = 0; i < (int)iRow; ++i) {
		delete[] grid[i];
	}
	delete[] grid;

	GamePlayStatic::SpawnGameObject(pObj, LAYER_TILECOL);
	//CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj,LAYER_TILECOL,false);
	//pObj->begin();
	//CTaskMgr::GetInst()->TriggetObjectEvent();
}

void TileMapEditor::GetEndIdxOfRectArea(int** _grid, int _startX, int _startY, int& _endX, int& _endY)
{
	const vector<tTileInfo>& vecTiles = m_pTileMap->GetTilesInfo();
	//int iCol = m_pTileMap->GetCol();
	//int iRow = m_pTileMap->GetRow();

	int iCol = m_pTileMap->GetRow();
	int iRow = m_pTileMap->GetCol();
	const tTileInfo& tTile = vecTiles[_startY * iCol + _startX];

	// get min size of column and row
	int minX = (int)iCol - 1;
	int minY = (int)iRow - 1;
	int y = _startY;
	int x = _startX;
	for (; y < (int)iRow; ++y) {
		x = _startX;

		const tTileInfo& tTile = vecTiles[y * iCol + x];
		if (!(-1 < tTile.TileIdx) || (int)E_VisitedState::visited == _grid[y][x]) {
			minY = y - 1;
			break;
		}

		for (; x <= minX; ++x) {
			const tTileInfo& tTile = vecTiles[y * iCol + x];
			if (!(-1 < tTile.TileIdx) || (int)E_VisitedState::visited == _grid[y][x]) {
				if (minX > x - 1) {
					minX = x - 1;
				}
			}
		}
	}

	// 잡힌 영역을 방문했다고 표시한다.
	for (int y = _startY; y <= minY; ++y) {
		for (int x = _startX; x <= minX; ++x) {
			_grid[y][x] = 1;
		}
	}
	x = _startX;
	y = _startY;

	_endX = minX;
	_endY = minY;
}

void TileMapEditor::Activate()
{
	UI::Activate();

	GetTargetObject()->TileMap()->SetGridVisible(true);
}
