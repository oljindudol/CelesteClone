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
    bool                    m_bDebug = false;


public:
    void GenerateParticle(const Vec4& _Color , const Vec4& _VibColor) { 
        m_bThisFrameGenerate = true; 
        m_Module.vSpawnColor = _Color;
        m_Module.VibColor = _VibColor;
    }
    void GenerateParticle(const Vec4& _Color) {
        m_bThisFrameGenerate = true;
        m_Module.vSpawnColor = _Color;
    }

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
    void SetDebug(bool _bDebug) { m_bDebug = _bDebug; }
    bool GetDebug() { return m_bDebug; }

    CCustomParticleSystem();
    CCustomParticleSystem(const CCustomParticleSystem& _OriginParticle);
    ~CCustomParticleSystem();
};