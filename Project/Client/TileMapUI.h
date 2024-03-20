#pragma once
#include "ComponentUI.h"


class TileMapUI :
    public ComponentUI
{
public:
    virtual void render_update() override;
public:
    TileMapUI();
    ~TileMapUI();
};

