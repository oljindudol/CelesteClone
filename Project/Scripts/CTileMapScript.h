#pragma once
#include <Engine/CScript.h>

class CTileMapScript :
    public CScript
{
private:

public:
    virtual void begin() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CTileMapScript);
public:
    CTileMapScript();
    ~CTileMapScript();
};

