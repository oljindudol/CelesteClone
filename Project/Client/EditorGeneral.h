#pragma once
#include "UI.h"
class EditorGeneral :
    public UI
{
private:
public:
    virtual void render_update() override;

public:
    EditorGeneral();
    ~EditorGeneral();
};
