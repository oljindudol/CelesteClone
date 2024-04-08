#pragma once
#include "ComponentUI.h"
class DreamBlockParticleSystemUI :
    public ComponentUI
{
public:
    virtual void render_update() override;

public:
    DreamBlockParticleSystemUI();
    ~DreamBlockParticleSystemUI();
};


