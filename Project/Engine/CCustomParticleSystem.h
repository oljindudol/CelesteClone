#pragma once
#include "CRenderComponent.h"
#include "CParticleUpdate.h"
class CStructuredBuffer;

class CCustomParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    UINT                    m_MaxParticleCount;

    tParticleModule         m_Module;
    CStructuredBuffer* m_ParticleModuleBuffer;

    CStructuredBuffer* m_SpawnCountBuffer;

    Ptr<CParticleUpdate>    m_CSParticleUpdate;

    Ptr<CTexture>           m_ParticleTex;

    float                   m_Time;
    bool                    m_bThisFrameGenerate;


public:
    void GenerateParticle() { m_bThisFrameGenerate = true; }
    void SetParticleColor(Vec4 _Color) { m_Module.vSpawnColor = _Color; }


    virtual void UpdateData() override;
    virtual void finaltick() override;
    virtual void render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CCustomParticleSystem);
public:
    tParticleModule GetModule() {
        return m_Module;
    }
    void SetModule(tParticleModule _Module) {
        m_Module = _Module;
    }
    CCustomParticleSystem();
    CCustomParticleSystem(const CCustomParticleSystem& _OriginParticle);
    ~CCustomParticleSystem();
};