#include "pch.h"
#include "ParticleSystemUI.h"

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("ParticleSystem", "##ParticleSystem", COMPONENT_TYPE::PARTICLESYSTEM)
{
	SetSize(ImVec2(0.f, 200.f));
	SetComopnentTitle("ParticleSystemUI");
}

ParticleSystemUI::~ParticleSystemUI()
{
}


void ParticleSystemUI::render_update()
{
}

void ParticleSystemUI::MaterialSelect(DWORD_PTR _ptr)
{
}

