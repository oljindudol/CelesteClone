#pragma once
#include "ComponentUI.h"
class TransitionParticleSystemUI :
    public ComponentUI
{
public:
    virtual void render_update() override;

public:
    TransitionParticleSystemUI();
    ~TransitionParticleSystemUI();
};



