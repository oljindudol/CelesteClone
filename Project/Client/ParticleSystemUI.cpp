#include "pch.h"
#include "ParticleSystemUI.h"
#include "Engine\CParticleSystem.h"

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("ParticleSystem", "##ParticleSystem", COMPONENT_TYPE::PARTICLESYSTEM)
{
	SetSize(ImVec2(0.f, 600.f));
	SetComopnentTitle("ParticleSystemUI");
}

ParticleSystemUI::~ParticleSystemUI()
{
}


void ParticleSystemUI::render_update()
{
	ComponentUI::render_update();

	auto TO = GetTargetObject();

	auto pPS = TO->ParticleSystem();
	tParticleModule CurModule = pPS->GetModule();
	tParticleModule NewModule = CurModule;

	// Drag Module
	ImGui::Separator();
	ImGui::Text("Drag Module"); ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] )
	{
		if (ImGui::Button("On##DragModule"))
		{
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 0;
		}
	}
	else
	{
		if (ImGui::Button("Off##DragModule"))
		{
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 1;
		}
	}
	ImGui::Text("DragTime"); ImGui::SameLine();
	ImGui::DragFloat("##DragTime", &NewModule.DragTime, 0.01f, 0.01f, 10000.f);



	// Spawn Module
	ImGui::Separator();
	ImGui::Text("Spawn Module"); ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN]) {
		if (ImGui::Button("On##SpawnModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 0;
		}
	}
	else {
		if (ImGui::Button("Off##SpawnModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;
		}
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
	ImGui::DragFloat4("##Box Scale", &NewModule.vSpawnBoxScale.x, 0.1f, 0.0f, 100.0f);
	ImGui::Text("Spawn Rate"); ImGui::SameLine();
	ImGui::DragInt("##Spawn Rate", &NewModule.SpawnRate, 1, 0, 1000);

	// Add Velocity Module
	ImGui::Separator();
	ImGui::Text("Add Velocity Module"); ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY]) {
		if (ImGui::Button("On##AddVelocityModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 0;
		}
	}
	else {
		if (ImGui::Button("Off##AddVelocityModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
		}
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
	ImGui::DragFloat4("##FixedDirection", &NewModule.FixedDirection.x, 0.1, 0, 1000);
	ImGui::Text("FixedAngle"); ImGui::SameLine();
	ImGui::DragFloat("##FixedAngle", &NewModule.FixedAngle, 0.1, 0, 1000);

	// Scale Module
	ImGui::Separator();
	ImGui::Text("Scale Module"); ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE]) {
		if (ImGui::Button("On##ScaleModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = 0;
		}
	}
	else {
		if (ImGui::Button("Off##ScaleModule")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = 1;
		}
	}
	ImGui::Text("Scale Ratio"); ImGui::SameLine();
	ImGui::DragFloat3("##Scale Ratio", &NewModule.vScaleRatio.x, 0.01f, 0.0f, 10.0f);


	// Noise Force// Noise Force
	ImGui::Separator();
	ImGui::Text("Noise Force"); ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE]) {
		if (ImGui::Button("On##NoiseForce")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 0;
		}
	}
	else {
		if (ImGui::Button("Off##NoiseForce")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 1;
		}
	}
	ImGui::Text("NoiseForceScale"); ImGui::SameLine();
	ImGui::DragFloat("##NoiseForceScale", &NewModule.NoiseForceScale, 0.1f, 0.0f, 100.0f);
	ImGui::Text("NoiseForceTerm"); ImGui::SameLine();
	ImGui::DragFloat("##NoiseForceTerm", &NewModule.NoiseForceTerm, 0.01f, 0.0f, 1.0f);

	// Calculate Force
	ImGui::Separator();
	ImGui::Text("Calculate Force"); ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE]) {
		if (ImGui::Button("On##CalculateForce")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 0;
		}
	}
	else {
		if (ImGui::Button("Off##CalculateForce")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 1;
		}
	}

	// Render
	ImGui::Separator();
	ImGui::Text("Render"); ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER]) {
		if (ImGui::Button("On##Render")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 0;
		}
	}
	else {
		if (ImGui::Button("Off##Render")) {
			NewModule.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 1;
		}
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

