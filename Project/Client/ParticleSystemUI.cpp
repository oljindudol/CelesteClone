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

	TO->Transform();

}

void ParticleSystemUI::MaterialSelect(DWORD_PTR _ptr)
{
}

