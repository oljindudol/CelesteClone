#pragma once
#include <Engine/CScript.h>


class CMainCameraScript :
    public CScript
{
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CMainCameraScript);

public:
    CMainCameraScript();
    ~CMainCameraScript();
};

