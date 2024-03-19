#include "pch.h"
#include "Animator2DUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnim.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"
#include "AnimEditor.h"

Animator2DUI::Animator2DUI()
	: ComponentUI("Animator2D", "##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
{
	SetSize(ImVec2(0.f, 120.f));
	SetComopnentTitle("Animator2D");
}

Animator2DUI::~Animator2DUI()
{
}


void Animator2DUI::render_update()
{
	ComponentUI::render_update();
	static CGameObject* pPrevTarget = nullptr;
	static bool selected[50] = {};

	CGameObject* pTarget = GetTargetObject();

	CAnimator2D* pAnimator2D = pTarget->Animator2D();

	map<wstring, CAnim*>& refMapAnim = pAnimator2D->GetMapAnim();

	//이전 오브젝트와 현재오브젝트가 다르다면, 재할당
	if (pPrevTarget != pTarget)
	{
		for (auto& b : selected)
		{
			b = false;
		}
	}

	pPrevTarget = pTarget;


	if (ImGui::Button("SAVE"))
	{
	}

	ImGui::SameLine();
	if (ImGui::Button("LOAD"))
	{
	}


	if (ImGui::Button("ADD"))
	{
	}
	vector<wstring> delanim;

	if (ImGui::BeginTable("split2", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
	{
		int i = 0;
		for (auto& p : refMapAnim)
		{
			//char label[32];
			//snprintf(label, sizeof(label),"##Item %d", i);

			ImGui::TableNextColumn();


			//ImGui::Selectable(label, &selected[i]);


			string s;
			s.assign(p.first.begin(), p.first.end());



			ImVec4 buttonColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

			ImGui::PushStyleColor(ImGuiCol_Button, buttonColor); // 기본 배경색
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonColor); // 호버 배경색도 기본 배경색과 동일하게 설정
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonColor);
			if (ImGui::Button(s.c_str()))
			{
			}
			ImGui::PopStyleColor(3);


			ImGui::SameLine();
			if (ImGui::Button(("EDIT##" + std::to_string(i)).c_str()))
			{
				AnimEditor* pEdit = (AnimEditor*)CImGuiMgr::GetInst()->FindUI("##AnimEditor");
				CGameObject* pTarget = GetTargetObject();
				pEdit->initialize();
				pEdit->SetpGO(pTarget);
				pEdit->SetTargetIdx(i);
				pEdit->Activate();
			}

			//ImGui::SameLine();
			//if (ImGui::Button(("DEL##" + std::to_string(i)).c_str()))
			//{
			//	delanim.push_back(p.first);
			//}


			ImGui::SameLine();
			if (ImGui::Button(("PLAY##" + std::to_string(i)).c_str()))
			{
				auto to = GetTargetObject();
				auto animator = (CAnimator2D*) to->GetComponent(COMPONENT_TYPE::ANIMATOR2D);
				animator->Play(p.first);
			}

			for (auto& f : p.second->GetVecFrm())
			{
				//파일방식으로 개선
				auto srv = f.pFrameTex->GetSRV();
				auto imimid = (ImTextureID)srv.Get();
				auto aw = (float)f.pFrameTex->GetWidth();
				auto ah = (float)f.pFrameTex->GetHeight();


				auto slicesize = ImVec2(f.vSlice.x * aw, f.vSlice.y * ah) / 5;
				auto slice = ImVec2(f.vSlice.x, f.vSlice.y);
				auto lt = f.vLeftTop;
				auto uv = ImVec2(lt.x / aw, lt.y / ah);
				ImGui::Image(imimid, slicesize, ImVec2(lt.x, lt.y), ImVec2(lt.x, lt.y) + slice);
				ImGui::SameLine();
			}
			ImGui::Text(" ");

			++i;
		}

		ImGui::EndTable();
		//먼가릭남음
		for (auto& ws : delanim)
		{
			refMapAnim.erase(ws);
		}
	}
}