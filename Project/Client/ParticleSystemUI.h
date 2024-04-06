#pragma once
#include "ComponentUI.h"

class ParticleSystemUI :
    public ComponentUI
{
public:
    virtual void render_update() override;

public:
    void MaterialSelect(DWORD_PTR _ptr);

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

