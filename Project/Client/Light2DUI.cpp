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
	Light-> SetLightColor(LColor);

	auto LSpec = Light->GetSpecular();
	Light->SetSpecular(LSpec);

	auto LAmbient = Light->GetAmbient();
	Light->SetAmbient(LAmbient);


	auto LType = Light->GetLightType();
	Light->SetLightType(LType);

	//if(LType == LIGHT_TYPE::POINT)
	auto LRadius = Light->GetRadius();
	Light->SetRadius(LRadius);

	auto LAngle = Light->GetAngle();
	Light->SetAngle(LAngle);

}
