#pragma once
#include "CRenderComponent.h"
#include "CTransitionParticleUpdate.h"

class CTransitionParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    UINT                    m_MaxParticleCount;

    tParticleModule         m_Module;
    CStructuredBuffer* m_ParticleModuleBuffer;

    CStructuredBuffer* m_SpawnCountBuffer;

    //Ptr<CParticleUpdate>    m_CSParticleUpdate;
    Ptr<CTransitionParticleUpdate>    m_CSParticleUpdate;

    Ptr<CTexture>           m_ParticleTex;
    vector<Ptr<CTexture>>   m_vecParticleTex;
    Ptr<CTexture>           m_ParticleArrTex;

    float                   m_Time;
    bool                    m_bThisFrameGenerate;
    bool                    m_bDebug = true;

    float                   m_CurTime = 0.f;
    float                   m_EventTime = .3f;
    bool                    m_bToRightEvent = false;
    float                   m_ScaleIncreaseRatio = 10.f;


public:
    float& GetScaleIncreaseRatioRef() { return m_ScaleIncreaseRatio; }
    bool& GetToRightEventRef() { return m_bToRightEvent; }
    float& GetEventTimeRef() { return m_EventTime; }
    void ReFreshEvent() { m_CurTime = m_EventTime; }

    void GenerateParticle(const Vec4& _Color, const Vec4& _VibColor) {
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
    CLONE(CTransitionParticleSystem);
public:
    tParticleModule GetModule() {
        return m_Module;
    }
    void SetModule(tParticleModule _Module) {
        m_Module = _Module;
    }
    void SetDebug(bool _bDebug) { m_bDebug = _bDebug; }
    bool GetDebug() { return m_bDebug; }
    CTransitionParticleSystem();
    CTransitionParticleSystem(const CTransitionParticleSystem& _OriginParticle);
    ~CTransitionParticleSystem();
};

