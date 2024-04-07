#include "pch.h"
#include "ParticleSystemUI.h"
#include "Engine\CCustomParticleSystem.h"

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("ParticleSystem", "##ParticleSystem", COMPONENT_TYPE::CUSTOMPARTICLESYSTEM)
{
	SetSize(ImVec2(0.f, 800.f));
	SetComopnentTitle("ParticleSystemUI");
}

ParticleSystemUI::~ParticleSystemUI()
{
}

void CreateStaticColorButton(const string& _name, const ImVec4& _Color)
{
	ImGui::PushStyleColor(ImGuiCol_Button, _Color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _Color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, _Color);
	ImGui::Button(_name.c_str());
	ImGui::PopStyleColor(3);
}


void ParticleSystemUI::render_update()
{
	ComponentUI::render_update();


	static const ImVec4 Red = ImVec4(1.f, 0.f, 0.f, 1.0f);
	static const ImVec4 Green = ImVec4(0.f, 1.f, 0.f, 1.0f);
	static const ImVec4 Blue = ImVec4(0.f, 0.f, 1.f, 1.0f);
	static const ImVec4 Gray = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	static const ImVec4 Burgundy = ImVec4(0.5647f, 0.f, 0.1255f, 1.0f);

	auto TO = GetTargetObject();
	auto pPS = TO->CustomParticleSystem();
	tParticleModule CurModule = pPS->GetModule();
	tParticleModule NewModule = CurModule;

	static ImVec4 ModuleColor = Burgundy;



	// VibColor
	ImGui::Separator();
	CreateStaticColorButton("VibColor Module", ModuleColor);
	ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::Vibration]) {
		ImGui::PushStyleColor(ImGuiCol_Button, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Green);
		if (ImGui::Button("On##VibColor")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::Vibration] = 0;
		}
		ImGui::PopStyleColor(3);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Gray);
		if (ImGui::Button("Off##VibColor")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::Vibration] = 1;
		}
		ImGui::PopStyleColor(3);
	}
	ImGui::Text("VibColorTerm"); ImGui::SameLine();
	ImGui::DragFloat("##VibColorTerm", &NewModule.VibTime, 0.01f, 0.0f, 1.0f);
	ImGui::Text("Vib Color"); ImGui::SameLine();
	ImGui::ColorEdit4("##Vib Color", &NewModule.VibColor.x);

	// Spawn Module
	ImGui::Separator();
	CreateStaticColorButton("Spawn Module", ModuleColor);
	ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN]) {
		ImGui::PushStyleColor(ImGuiCol_Button, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Green);
		if (ImGui::Button("On##SpawnModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 0;
		}
		ImGui::PopStyleColor(3);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Gray);
		if (ImGui::Button("Off##SpawnModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;
		}
		ImGui::PopStyleColor(3);
	}
	ImGui::Text("SpaceType"); ImGui::SameLine();
	if (1 == CurModule.SpaceType) {
		if (ImGui::Button("WorldSpace")) {
			NewModule.SpaceType = 0;
		}
	}
	else {
		if (ImGui::Button("LocalSpace")) {
			NewModule.SpaceType = 1;
		}
	}
	ImGui::Text("Spawn Color"); ImGui::SameLine();
	ImGui::ColorEdit4("##Spawn Color", &NewModule.vSpawnColor.x);
	ImGui::Text("Min Scale"); ImGui::SameLine();
	ImGui::DragFloat4("##Min Scale", &NewModule.vSpawnMinScale.x, 0.1f, 0.0f, 1000.0f);
	ImGui::Text("Max Scale"); ImGui::SameLine();
	ImGui::DragFloat4("##Max Scale", &NewModule.vSpawnMaxScale.x, 0.1f, 0.0f, 1000.0f);
	ImGui::Text("Min Life"); ImGui::SameLine();
	ImGui::DragFloat("##Min Life", &NewModule.MinLife, 0.1f, 0.0f, 100.0f);
	ImGui::Text("Max Life"); ImGui::SameLine();
	ImGui::DragFloat("##Max Life", &NewModule.MaxLife, 0.1f, 0.0f, 100.0f);
	ImGui::Text("Min Mass"); ImGui::SameLine();
	ImGui::DragFloat("##Min Mass", &NewModule.MinMass, 0.1f, 0.0f, 100.0f);
	ImGui::Text("Max Mass"); ImGui::SameLine();
	ImGui::DragFloat("##Max Mass", &NewModule.MaxMass, 0.1f, 0.0f, 100.0f);
	ImGui::Text("Spawn Shape"); ImGui::SameLine();
	if (1 == CurModule.SpawnShape) {
		if (ImGui::Button("Box")) {
			NewModule.SpawnShape = 0;
		}
	}
	else {
		if (ImGui::Button("Sphere")) {
			NewModule.SpawnShape = 1;
		}
	}
	ImGui::Text("Radius"); ImGui::SameLine();
	ImGui::DragFloat("##Radius", &NewModule.Radius, 1.0f, 0.0f, 1000.0f);
	ImGui::Text("Box Scale"); ImGui::SameLine();
	ImGui::DragFloat4("##Box Scale", &NewModule.vSpawnBoxScale.x, 0.1f, 0.0f, 1000.0f);
	ImGui::Text("Spawn Rate"); ImGui::SameLine();
	ImGui::DragInt("##Spawn Rate", &NewModule.SpawnRate, 1, 0, 1000);

	// Add Velocity Module
	ImGui::Separator();
	CreateStaticColorButton("Add Velocity Module", ModuleColor);
	ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY]) {
		ImGui::PushStyleColor(ImGuiCol_Button, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Green);
		if (ImGui::Button("On##AddVelocityModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 0;
		}
		ImGui::PopStyleColor(3);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Gray);
		if (ImGui::Button("Off##AddVelocityModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
		}
		ImGui::PopStyleColor(3);
	}
	ImGui::Text("AddVelocityType"); ImGui::SameLine();
	if (0 == CurModule.AddVelocityType) {
		if (ImGui::Button("From Center")) {
			NewModule.AddVelocityType = 1;
		}
	}
	else if(1 == CurModule.AddVelocityType) {
		if (ImGui::Button("To Center")) {
			NewModule.AddVelocityType = 2;
		}
	}
	else
	{
		if (ImGui::Button("Fix Direction")) {
			NewModule.AddVelocityType = 0;
		}
	}
	ImGui::Text("Min Speed"); ImGui::SameLine();
	ImGui::DragFloat("##Min Speed", &NewModule.MinSpeed, 1, 0, 1000);
	ImGui::Text("Max Speed"); ImGui::SameLine();
	ImGui::DragFloat("##Max Speed", &NewModule.MaxSpeed, 1, 0, 1000);
	ImGui::Text("FixedDirection"); ImGui::SameLine();
	ImGui::DragFloat4("##FixedDirection", &NewModule.FixedDirection.x, 0.1, -100, 100);
	//ImGui::Text("FixedAngle"); ImGui::SameLine();
	//ImGui::DragFloat("##FixedAngle", &NewModule.FixedAngle, 0.1, 0, 1000);

	// Drag Module
	ImGui::Separator();
	CreateStaticColorButton("Drag Module", ModuleColor);
	ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG])
	{
		ImGui::PushStyleColor(ImGuiCol_Button, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Green);
		if (ImGui::Button("On##DragModule"))
		{
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 0;
		}
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Gray);
		if (ImGui::Button("Off##DragModule"))
		{
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 1;
		}
		ImGui::PopStyleColor(3);
	}
	ImGui::Text("DragTime"); ImGui::SameLine();
	ImGui::DragFloat("##DragTime", &NewModule.DragTime, 0.01f, 0.01f, 10000.f);


	// Scale Module
	ImGui::Separator();
	CreateStaticColorButton("Scale Module", ModuleColor);
	ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE]) {
		ImGui::PushStyleColor(ImGuiCol_Button, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Green);
		if (ImGui::Button("On##ScaleModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = 0;
		}
		ImGui::PopStyleColor(3);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Gray);
		if (ImGui::Button("Off##ScaleModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = 1;
		}
		ImGui::PopStyleColor(3);
	}
	ImGui::Text("Scale Ratio"); ImGui::SameLine();
	ImGui::DragFloat3("##Scale Ratio", &NewModule.vScaleRatio.x, 0.01f, 0.0f, 10.0f);


	// Noise Force
	ImGui::Separator();
	CreateStaticColorButton("Noise Force", ModuleColor);
	ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE]) {
		ImGui::PushStyleColor(ImGuiCol_Button, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Green);
		if (ImGui::Button("On##NoiseForce")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 0;
		}
		ImGui::PopStyleColor(3);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Gray);
		if (ImGui::Button("Off##NoiseForce")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 1;
		}
		ImGui::PopStyleColor(3);
	}
	ImGui::Text("NoiseForceScale"); ImGui::SameLine();
	ImGui::DragFloat("##NoiseForceScale", &NewModule.NoiseForceScale, 0.1f, 0.0f, 100.0f);
	ImGui::Text("NoiseForceTerm"); ImGui::SameLine();
	ImGui::DragFloat("##NoiseForceTerm", &NewModule.NoiseForceTerm, 0.01f, 0.0f, 1.0f);

	// Calculate Force
	ImGui::Separator();
	ImGui::Text("Calculate Force"); ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE]) {
		ImGui::PushStyleColor(ImGuiCol_Button, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Green);
		if (ImGui::Button("On##CalculateForce")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 0;
		}
		ImGui::PopStyleColor(3);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Gray);
		if (ImGui::Button("Off##CalculateForce")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 1;
		}
		ImGui::PopStyleColor(3);
	}

	// Render
	ImGui::Separator();
	CreateStaticColorButton("Render Module", ModuleColor);
	ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER]) {
		ImGui::PushStyleColor(ImGuiCol_Button, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Green);
		if (ImGui::Button("On##Render")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 0;
		}
		ImGui::PopStyleColor(3);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Gray);
		if (ImGui::Button("Off##Render")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 1;
		}
		ImGui::PopStyleColor(3);
	}
	ImGui::Text("VelocityAlignment"); ImGui::SameLine();
	if (0 == CurModule.VelocityAlignment) {
		if (ImGui::Button("Off")) {
			NewModule.VelocityAlignment = 1;
		}
	}
	else if (1 == CurModule.VelocityAlignment) {
		if (ImGui::Button("On")) {
			NewModule.VelocityAlignment = 0;
		}
	}
	ImGui::Text("AlphaBasedLife"); ImGui::SameLine();
	if (0 == CurModule.AlphaBasedLife) {
		if (ImGui::Button("Off")) {
			NewModule.AlphaBasedLife = 1;
		}
	}
	else if (1 == CurModule.AlphaBasedLife) {
		if (ImGui::Button("NomrlizedAge")) {
			NewModule.AlphaBasedLife = 2;
		}
	}
	else
	{
		if (ImGui::Button("Age")) {
			NewModule.AlphaBasedLife = 0;
		}
	}
	ImGui::Text("AlphaMaxAge"); ImGui::SameLine();
	ImGui::DragFloat("##AlphaMaxAge", &NewModule.AlphaMaxAge, 0.1f, 0.0f, 10.0f);





	pPS->SetModule(NewModule);

}

void ParticleSystemUI::MaterialSelect(DWORD_PTR _ptr)
{
}

