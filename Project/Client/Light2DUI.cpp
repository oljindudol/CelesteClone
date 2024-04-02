#include "pch.h"
#include "Light2DUI.h"
#include "Engine\CLight2D.h"

Light2DUI::Light2DUI()
	: ComponentUI("Light2D", "##Light2D", COMPONENT_TYPE::LIGHT2D)
{
	SetSize(ImVec2(0.f, 300.f));
	SetComopnentTitle("Light2D");
	auto a = magic_enum::enum_names< LIGHT_TYPE>();

	m_items = vector<string>(a.begin(), a.end());
	//for (int i = 0; i < (int)LIGHT_TYPE::LIGHT_TYPE_END; ++i)
	//{
	//	m_items.push_back(magic_enum::enum_name( (LIGHT_TYPE)i);
	//}
}

Light2DUI::~Light2DUI()
{
}


void Light2DUI::render_update()
{
	ComponentUI::render_update();

	auto Light = GetTargetObject()->Light2D();

	auto LColor = Light-> GetLightColor();
	ImGui::Text("Color"); ImGui::SameLine();  ImGui::DragFloat3("##Color Input", (float*)&LColor);
	LColor.x = ClampFloat(LColor.x, 0.f, 255.f);
	LColor.y = ClampFloat(LColor.y, 0.f, 255.f);
	LColor.z = ClampFloat(LColor.z, 0.f, 255.f);
	Light-> SetLightColor(LColor);

	auto LSpec = Light->GetSpecular();
	ImGui::Text("Specular"); ImGui::SameLine();  ImGui::DragFloat4("##Specular Input", (float*)&LSpec);
	LSpec.x = ClampFloat(LSpec.x, 0.f, 255.f);
	LSpec.y = ClampFloat(LSpec.y, 0.f, 255.f);
	LSpec.z = ClampFloat(LSpec.z, 0.f, 255.f);
	Light->SetSpecular(LSpec);

	auto LAmbient = Light->GetAmbient();
	LAmbient= LAmbient * 255.f;
	ImGui::Text("Ambient"); ImGui::SameLine();  ImGui::DragFloat4("##Ambient Input", (float*)&LAmbient);
	LAmbient.x = ClampFloat(LAmbient.x, 0.f, 255.f);
	LAmbient.y = ClampFloat(LAmbient.y, 0.f, 255.f);
	LAmbient.z = ClampFloat(LAmbient.z, 0.f, 255.f);
	LAmbient = LAmbient / 255.f;
	Light->SetAmbient(LAmbient);


	auto LType = Light->GetLightType();
	ImGui::SetNextItemWidth(150);
	if (ImGui::BeginCombo("##comboColType", ToString(magic_enum::enum_name(LType) ).c_str())) {
		for (int i = 0; i < m_items.size(); i++) {
			const bool isSelected = ((int)LType == i);
			if (ImGui::Selectable(m_items[i].c_str(), isSelected)) {
				LType = (LIGHT_TYPE)i;
			}

			// 항목 선택 시 자동으로 스크롤
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	Light->SetLightType(LType);

	//if(LType == LIGHT_TYPE::POINT)
	auto LRadius = Light->GetRadius();
	ImGui::Text("Radius"); ImGui::SameLine();  ImGui::DragFloat("##Radius Input", (float*)&LRadius);
	Light->SetRadius(LRadius);

	auto LAngle = Light->GetAngle();
	ImGui::Text("Angle"); ImGui::SameLine();  ImGui::DragFloat("##Angle Input", (float*)&LAngle);
	Light->SetAngle(LAngle);

}
