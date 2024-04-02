#include "pch.h"
#include "Light2DUI.h"
#include "Engine\CLight2D.h"

Light2DUI::Light2DUI()
	: ComponentUI("Light2D", "##Light2D", COMPONENT_TYPE::LIGHT2D)
{
	SetSize(ImVec2(0.f, 120.f));
	SetComopnentTitle("Light2D");
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
	ImGui::Text("Ambient"); ImGui::SameLine();  ImGui::DragFloat4("##Ambient Input", (float*)&LAmbient);
	LAmbient.x = ClampFloat(LAmbient.x, 0.f, 255.f);
	LAmbient.y = ClampFloat(LAmbient.y, 0.f, 255.f);
	LAmbient.z = ClampFloat(LAmbient.z, 0.f, 255.f);
	Light->SetAmbient(LAmbient);


	auto LType = Light->GetLightType();
	//ImGui::Text("LType"); ImGui::SameLine();  ImGui::DragFloat4("##Ambient Input", (float*)&LAmbient);
	//LAmbient.x = ClampFloat(LAmbient.x, 0.f, 255.f);
	Light->SetLightType(LType);

	//if(LType == LIGHT_TYPE::POINT)
	auto LRadius = Light->GetRadius();
	Light->SetRadius(LRadius);

	auto LAngle = Light->GetAngle();
	Light->SetAngle(LAngle);

}
