#pragma once
#include "ComponentUI.h"
class CameraUI:
    public ComponentUI
{
public:
    virtual void render_update() override;
public:
    CameraUI();
    ~CameraUI();
};
