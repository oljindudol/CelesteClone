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

	ImGui::Separator();
	// Drag Module
	ImGui::Text("Drag Module : "); ImGui::SameLine();
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

	ImGui::Separator();
	// Spawn Module
	ImGui::Text("Spawn Module : "); ImGui::SameLine();
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
	ImGui::Text("SpaceType : "); ImGui::SameLine();
	if (1 == CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN]) {
		if (ImGui::Button("WorldSpace")) {
			NewModule.SpaceType = 0;
		}
	}
	else {
		if (ImGui::Button("LocalSpace")) {
			NewModule.SpaceType = 1;
		}
	}


	CurModule.vSpawnColor = Vec4(0.2f, 0.4f, 0.9f, 1.f);
	CurModule.vSpawnMinScale = Vec4(50.f, 25.f, 1.f, 1.f);
	CurModule.vSpawnMaxScale = Vec4(50.f, 25.f, 1.f, 1.f);
	CurModule.MinLife = 5.f;
	CurModule.MaxLife = 5.f;
	CurModule.MinMass = 1.f;
	CurModule.MaxMass = 1.f;
	CurModule.SpawnShape = 1; // 0 : Sphere, 1 : Box
	CurModule.Radius = 100.f;
	CurModule.vSpawnBoxScale = Vec4(10.f, 10.f, 0.f, 0.f);
	CurModule.SpawnRate = 50;

	// Add Velocity Module
	CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
	CurModule.AddVelocityType = 0; // 0 : From Center, 1: To Center, 2: Fix Direction
	CurModule.MinSpeed = 500;
	CurModule.MaxSpeed = 500;
	CurModule.FixedDirection;
	CurModule.FixedAngle;

	// Scale
	CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = 0;
	CurModule.vScaleRatio = Vec3(0.1f, 0.1f, 0.1f);

	// Noise Force
	CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 1;
	CurModule.NoiseForceScale = 10.f;
	CurModule.NoiseForceTerm = 0.3f;

	// Calculate Force
	CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 1;
	
	// Render 
	CurModule.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 1;
	CurModule.VelocityAlignment = 1; // 속도에 따른 방향 정렬
	CurModule.AlphaBasedLife = 1; // 0 : off, 1 : NomrlizedAge, 2: Age
	CurModule.AlphaMaxAge = 2.f;


	pPS->SetModule(NewModule);

}

void ParticleSystemUI::MaterialSelect(DWORD_PTR _ptr)
{
}

