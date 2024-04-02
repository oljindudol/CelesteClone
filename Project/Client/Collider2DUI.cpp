#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CCollider2D.h>
#include <Engine/CGameObject.h>


Collider2DUI::Collider2DUI()
	: ComponentUI("Collider2D", "##Collider2D", COMPONENT_TYPE::COLLIDER2D)
{
	SetSize(ImVec2(0.f, 120.f));
	SetComopnentTitle("Collider2D");

	for (int i = 0; i < (int)COLLIDER2D_TYPE::COL_TYPE_END; ++i)
	{
		m_items.push_back((COLLIDER2D_TYPE)i);
	}
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::render_update()
{
	if (nullptr == GetTargetObject())
		return;

	ComponentUI::render_update();

	auto Collider = (CCollider2D*)GetTargetObject()->Collider2D();

	Vec2 vPos = Collider->GetOffsetPos();
	Vec2 vScale = Collider->GetOffsetScale();

	ImGui::Text("Position"); ImGui::SameLine();  ImGui::DragFloat2("##Relative Position", (float*) & vPos);
	ImGui::Text("Scale   "); ImGui::SameLine(); ImGui::DragFloat2("##Relative Scale", (float*)&vScale);

	Collider->SetOffsetPos(vPos);
	Collider->SetOffsetScale(vScale);

	// Absolute 값 
	bool bAbsolute = Collider->IsAbsolute();
	ImGui::Text("Scale Absolute"); ImGui::SameLine(); ImGui::Checkbox("##TransformAbsolute", &bAbsolute);
	Collider->SetAbsolute(bAbsolute);

	// Coltype
	auto ctype = Collider->GetType();
	auto enumname = magic_enum::enum_name(ctype);
	ImGui::Text("Col Type"); 
	ImGui::SameLine(); 

	// Coltype 콤보
	ImGui::SetNextItemWidth(150);


	

	if (ImGui::BeginCombo("##comboColType",ToString(enumname).c_str())) {
		for (int i = 0; i < m_items.size(); i++) {
			const bool isSelected = ((int)ctype == i);
			if (ImGui::Selectable(ToString(magic_enum::enum_name(m_items[i])).c_str(), isSelected)) {
				ctype = (COLLIDER2D_TYPE)i;
			}

			// 항목 선택 시 자동으로 스크롤
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	Collider->SetColliderType(ctype);

}