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
//#include <Engine\CObjectManager.h>
//#include <Engine\CCursor.h>
//#include <Engine\CSceneManager.h>
//#include <Engine\CScene.h>


TileMapEditor::TileMapEditor()
    : UI("Edit Tile", STR_COMPONENT_UI_TileMapEditor),
	m_pTargetObject(nullptr),
	m_pTileMap{ nullptr },
	m_pAtlasTileTex{ nullptr },
	m_arrFaceTileCnt{ 0,0 },
	m_vAtlasTilePixelSize{ 0, 0 },
	m_iBrushSize(0),
	m_bDeleteMode{ false },
	m_iSelectedTileIdx(-1),

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


}

void TileMapEditor::_RenderCanvas()
{
	if (!m_pTargetObject)
		return;
	if (m_pTargetObject->IsDead())
		return;
	if (!m_pTargetObject->TileMap())
		return;


	m_pTileMap = m_pTargetObject->TileMap();
	m_pAtlasTileTex = m_pTileMap->GetTileAtlas().Get();



	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	
	//에디터카메라 오쏘뷰고정
	CCamera* pEditorCam = CRenderMgr::GetInst()->GetEditorCamera();
	if (pEditorCam) {
		if (PROJ_TYPE::PERSPECTIVE == pEditorCam->GetProjType())
			pEditorCam->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	}
	
	//auto texture = m_pTileMap->GetTileAtlas();
	//texture->GetWidth();
	//
	//ImVec2 vAtlasTexResol = {};
	//if (m_pTileMap->GetTileAtlas().Get()) {
	//	ImVec2(m_pTileMap->GetTileAtlas()->GetWidth(), m_pTileMap->GetTileAtlas()->GetHeight());
	//}
	//ImGui::Text("Atlas Texture Resolution [%d,%d]", vAtlasTexResol.x, vAtlasTexResol.x);
	//
	//ImGui::Text("Tile Face Size: %d %d", m_pTileMap->GetCol(), m_pTileMap->GetRow());
	//
	//ImGui::InputInt2("##Tile Map Size", m_arrFaceTileCnt);
	//m_arrFaceTileCnt[0] = CMyMath::Clamp(m_arrFaceTileCnt[0], 0, INT_MAX);
	//m_arrFaceTileCnt[1] = CMyMath::Clamp(m_arrFaceTileCnt[1], 0, INT_MAX);
	//
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
	//
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
	//
	//// canvas를 이용해서 하나 불러오기
	//_RenderCanvas();
	//
	//
	//// 텍스쳐 설정
	//CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
	//if (!pToolCam) {
	//	assert(pToolCam);
	//}
	//else {
	//	if (!ImGui::IsWindowFocused()) {
	//		m_bDeleteMode = false;
	//		// TODO(Jang) : 타일 클릭 충돌했는지 구하는 코드를 CollisionManager에 넣기
	//		Vector3 vMouseWorldPos = pToolCam->GetScreenToWorld2DPosition(MousePosition);
	//
	//		Vector3 vObjWorldPos = GetTargetObject()->Transform()->GetPosition();
	//		Vector3 vScale = GetTargetObject()->Transform()->GetScale();
	//		Vector3 vHalfScale = vScale * 0.5f;
	//		vHalfScale.x = fabsf(vHalfScale.x);
	//		vHalfScale.y = fabsf(vHalfScale.y);
	//
	//		Vector3 vLBWorldPos = {};
	//		vLBWorldPos.x = vObjWorldPos.x - vHalfScale.x;
	//		vLBWorldPos.y = vObjWorldPos.y - vHalfScale.y;
	//
	//		Vector2 vOffsetLB = -Vector2(vLBWorldPos.x, vLBWorldPos.y);
	//
	//		Vector3 vRTWorldPos = {};
	//		vRTWorldPos.x = vLBWorldPos.x + GetTargetObject()->Transform()->GetScale().x;
	//		vRTWorldPos.y = vLBWorldPos.y + GetTargetObject()->Transform()->GetScale().y;
	//
	//		// 마우스를 타일 내부에 클릭했는가?
	//		bool bIsTileClicked = false;
	//
	//		if (InputKeyHold(E_Key::LBUTTON)) {
	//			if (vLBWorldPos.x < vMouseWorldPos.x && vRTWorldPos.x > vMouseWorldPos.x &&
	//				vLBWorldPos.y < vMouseWorldPos.y && vRTWorldPos.y > vMouseWorldPos.y) {
	//				bIsTileClicked = true;
	//			}
	//
	//		}
	//		if (InputKeyHold(E_Key::LBUTTON) && InputKeyHold(E_Key::RBUTTON))
	//			m_bDeleteMode = true;
	//
	//		// 클릭했을 경우
	//		if (bIsTileClicked) {
	//			vector<TTileInfo>& vecTiles = m_pTileMap->GetTilesInfo();
	//			Vector2 vOriginMousePos = Vector2(vMouseWorldPos.x + vOffsetLB.x, vScale.y - (vMouseWorldPos.y + vOffsetLB.y));
	//
	//			Vector2 vOffsetScale = Vector2(m_pTileMap->GetCol() / vScale.x, m_pTileMap->GetRow() / vScale.y);
	//
	//			vOriginMousePos *= vOffsetScale;
	//
	//			int iClickX = (int)vOriginMousePos.x;
	//			int iClickY = (int)vOriginMousePos.y;
	//
	//			// Brush를 이용하여 그리기
	//			int iMinX, iMinY, iMaxX, iMaxY;
	//			iMinX = iClickX - m_iBrushSize;
	//			iMaxX = iClickX + m_iBrushSize;
	//			iMinY = iClickY - m_iBrushSize;
	//			iMaxY = iClickY + m_iBrushSize;
	//			iMinX = max(0, iMinX);
	//			iMinY = max(0, iMinY);
	//			iMaxX = min(m_pTileMap->GetCol() - 1, iMaxX);
	//			iMaxY = min(m_pTileMap->GetRow() - 1, iMaxY);
	//
	//			for (int y = iMinY; y <= iMaxY; ++y) {
	//				for (int x = iMinX; x <= iMaxX; ++x) {
	//					int idx = y * m_pTileMap->GetCol() + x;
	//
	//					if (m_bDeleteMode)
	//						vecTiles[idx].idx = -1;
	//					else
	//						vecTiles[idx].idx = m_iSelectedTileIdx;
	//				}
	//			}
	//			/*int idx = iClickY * m_pTileMap->GetCol() + iClickX;
	//			vecTiles[idx].idx = m_iSelectedTileIdx;*/
	//		}
	//	}
	//}
	//
	//if (ImGui::Button("CollderCreate##TileMap2D")) {
	//	CGameObject* pObj = CObjectManager::GetInstance()->CreateEmptyGameObject();
	//	pObj->SetName(_T("new Collders"));
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
	//if (ImGui::Button("OptimizeCollider##TileMap2D")) {
	//	_OptimizeCollisionArea();
	//}


}

void TileMapEditor::_SelectTileMap(DWORD_PTR _strKey, DWORD_PTR _NONE)
{
}

void TileMapEditor::_SelectTexture(DWORD_PTR _strKey, DWORD_PTR _NONE)
{
}

void TileMapEditor::_Clear()
{
}

void TileMapEditor::_OptimizeCollisionArea()
{
}

void TileMapEditor::GetEndIdxOfRectArea(int** _grid, int _startX, int _startY, int& _endX, int& _endY)
{
}
