#include "pch.h"
#include "DreamBlockParticleSystemUI.h"
#include "Engine\CDreamBlockParticleSystem.h"



DreamBlockParticleSystemUI::DreamBlockParticleSystemUI()
	: ComponentUI("ParticleSystem", "##ParticleSystem", COMPONENT_TYPE::DREAMBLOCKPARTICLESYSTEM)
{
	SetSize(ImVec2(0.f, 1000.f));
	SetComopnentTitle("DreamBlockParticleSystemUI");
}

DreamBlockParticleSystemUI::~DreamBlockParticleSystemUI()
{
}

void DreamBlockParticleSystemUI::render_update()
{
	ComponentUI::render_update();


	static const ImVec4 Red = ImVec4(1.f, 0.f, 0.f, 1.0f);
	static const ImVec4 Green = ImVec4(0.f, 1.f, 0.f, 1.0f);
	static const ImVec4 Blue = ImVec4(0.f, 0.f, 1.f, 1.0f);
	static const ImVec4 Gray = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	static const ImVec4 Burgundy = ImVec4(0.5647f, 0.f, 0.1255f, 1.0f);
	static ImVec4 ModuleColor = Burgundy;

	auto TO = GetTargetObject();
	auto pPS = TO->DreamBlockParticleSystem();
	if (ImGui::Button("Save"))
	{
		pPS->CreateDreamFab();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		pPS->LoadDreamFab();
	}

	tDreamParticleModule CurModule = pPS->GetModule();
	tDreamParticleModule NewModule = CurModule;

	if (ImGui::Button("ReSpawn"))
	{
		pPS->DeleteParticle();
		pPS->GenerateParticle();
	}
	ImGui::Text("Spawn Rate"); ImGui::SameLine();
	ImGui::SetNextItemWidth(150);
	ImGui::DragFloat("##Spawn Count", &NewModule.SpawnCount, 0.05f, 0, 1000); ImGui::SameLine();
	if (ImGui::Button("Spawn"))
	{
		pPS->GenerateParticle(); 
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear"))
	{
		pPS->DeleteParticle();
	}

	ImGui::Text("Layer Spawn Rate"); ImGui::SameLine();
	ImGui::DragFloat3("##Layer Spawn Rate", NewModule.vSpawnRate, .1f, 0.0f, 100.0f);


	ImGui::Separator();
	CreateStaticColorButton("LAYER 0", ModuleColor);
	ImGui::Text("Box Scale"); ImGui::SameLine();
	ImGui::DragFloat4("##Box Scale0", &NewModule.vSpawnBoxScale[0].x, 0.1f, 0.0f, 1000.0f);
	ImGui::Text("Spawn Scale"); ImGui::SameLine();
	ImGui::DragFloat("##Spawn Scale0", &NewModule.vSpawnScale[0], 0.1f, 0.0f, 1000.0f);
	ImGui::Text("Spawn Depth"); ImGui::SameLine();
	ImGui::DragFloat("##Spawn Depth0", &NewModule.vSpawnDepth[0], 0.1f, 0.0f, 1000.0f);
	ImGui::Text("Random Color1"); ImGui::SameLine();
	ImGui::ColorEdit4("##Random Color01", &NewModule.vSpawnColor[0].x);
	ImGui::Text("Random Color2"); ImGui::SameLine();
	ImGui::ColorEdit4("##Random Color02", &NewModule.vSpawnColor[1].x);
	ImGui::Text("Random Color3"); ImGui::SameLine();
	ImGui::ColorEdit4("##Random Color03", &NewModule.vSpawnColor[2].x);

	ImGui::Separator();
	CreateStaticColorButton("LAYER 1", ModuleColor);
	ImGui::Text("Box Scale"); ImGui::SameLine();
	ImGui::DragFloat4("##Box Scale1", &NewModule.vSpawnBoxScale[1].x, 0.1f, 0.0f, 1000.0f);
	ImGui::Text("Spawn Scale"); ImGui::SameLine();
	ImGui::DragFloat("##Spawn Scale1", &NewModule.vSpawnScale[1], 0.1f, 0.0f, 1000.0f);
	ImGui::Text("Spawn Depth"); ImGui::SameLine();
	ImGui::DragFloat("##Spawn Depth1", &NewModule.vSpawnDepth[1], 0.1f, 0.0f, 1000.0f);
	ImGui::Text("Random Color1"); ImGui::SameLine();
	ImGui::ColorEdit4("##Random Color11", &NewModule.vSpawnColor[3].x);
	ImGui::Text("Random Color2"); ImGui::SameLine();
	ImGui::ColorEdit4("##Random Color12", &NewModule.vSpawnColor[4].x);
	ImGui::Text("Random Color3"); ImGui::SameLine();
	ImGui::ColorEdit4("##Random Color13", &NewModule.vSpawnColor[5].x);

	ImGui::Separator();
	CreateStaticColorButton("LAYER 2", ModuleColor);
	ImGui::Text("Box Scale"); ImGui::SameLine();
	ImGui::DragFloat4("##Box Scale2", &NewModule.vSpawnBoxScale[2].x, 0.1f, 0.0f, 1000.0f);
	ImGui::Text("Spawn Scale"); ImGui::SameLine();
	ImGui::DragFloat("##Spawn Scale2", &NewModule.vSpawnScale[2], 0.1f, 0.0f, 1000.0f);
	ImGui::Text("Spawn Depth"); ImGui::SameLine();
	ImGui::DragFloat("##Spawn Depth2", &NewModule.vSpawnDepth[2], 0.1f, 0.0f, 1000.0f);
	ImGui::Text("Random Color1"); ImGui::SameLine();
	ImGui::ColorEdit4("##Random Color21", &NewModule.vSpawnColor[6].x);
	ImGui::Text("Random Color2"); ImGui::SameLine();
	ImGui::ColorEdit4("##Random Color22", &NewModule.vSpawnColor[7].x);
	ImGui::Text("Random Color3"); ImGui::SameLine();
	ImGui::ColorEdit4("##Random Color23", &NewModule.vSpawnColor[8].x);


	ImGui::Separator();
	CreateStaticColorButton("Animation Particle", ModuleColor);
	ImGui::Text("Duration"); ImGui::SameLine();
	ImGui::DragFloat("##animduration", &NewModule.FrameDuration);


	//Noise Force
	ImGui::Separator();
	CreateStaticColorButton("Noise Force", ModuleColor);
	if (1 == CurModule.arrModuleCheck[(UINT)DREAM_PARTICLE_MODULE::NOISE_FORCE]) {
		ImGui::PushStyleColor(ImGuiCol_Button, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Green);
		if (ImGui::Button("On##NoiseForce")) {
			NewModule.arrModuleCheck[(UINT)DREAM_PARTICLE_MODULE::NOISE_FORCE] = 0;
		}
		ImGui::PopStyleColor(3);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Gray);
		if (ImGui::Button("Off##NoiseForce")) {
			NewModule.arrModuleCheck[(UINT)DREAM_PARTICLE_MODULE::NOISE_FORCE] = 1;
		}
		ImGui::PopStyleColor(3);
	}
	ImGui::Text("NoiseForceScale"); ImGui::SameLine();
	ImGui::DragFloat("##NoiseForceScale", &NewModule.NoiseForceScale, 0.1f, 0.0f, 100.0f);
	ImGui::Text("NoiseForceTerm"); ImGui::SameLine();
	ImGui::DragFloat("##NoiseForceTerm", &NewModule.NoiseForceTerm, 0.01f, 0.0f, 1.0f);



	//Noise Rotation
	ImGui::Separator();
	CreateStaticColorButton("Noise Angle", ModuleColor);
	if (1 == CurModule.arrModuleCheck[(UINT)DREAM_PARTICLE_MODULE::NOISE_ROTATION]) {
		ImGui::PushStyleColor(ImGuiCol_Button, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Green);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Green);
		if (ImGui::Button("On##NoiseAngle")) {
			NewModule.arrModuleCheck[(UINT)DREAM_PARTICLE_MODULE::NOISE_ROTATION] = 0;
		}
		ImGui::PopStyleColor(3);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Gray);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Gray);
		if (ImGui::Button("Off##NoiseAngle")) {
			NewModule.arrModuleCheck[(UINT)DREAM_PARTICLE_MODULE::NOISE_ROTATION] = 1;
		}
		ImGui::PopStyleColor(3);
	}
	ImGui::Text("NoiseAngleScale"); ImGui::SameLine();
	ImGui::DragFloat("##NoiseAngleScale", &NewModule.NoiseAngleScale, 0.1f, 0.0f, 100.0f);
	ImGui::Text("NoiseAngleTerm"); ImGui::SameLine();
	ImGui::DragFloat("##NoiseAngleTerm", &NewModule.NoiseAngleTerm, 0.01f, 0.0f, 1.0f);



	pPS->SetModule(NewModule);
}