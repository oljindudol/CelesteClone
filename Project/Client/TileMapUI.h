#pragma once
#include "ComponentUI.h"


class TileMapUI :
    public ComponentUI
{
public:
    virtual void render_update() override;
    virtual void Activate() override;
public:
    TileMapUI();
    ~TileMapUI();
};

