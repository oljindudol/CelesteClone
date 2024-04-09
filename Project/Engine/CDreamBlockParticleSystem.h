#pragma once
#include "CRenderComponent.h"
#include "CDreamParticleUpdate.h"
class CDreamBlockParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    UINT                    m_MaxParticleCount;

    tDreamParticleModule         m_Module;
    CStructuredBuffer* m_ParticleModuleBuffer;

    CStructuredBuffer* m_SpawnCountBuffer;

    //Ptr<CParticleUpdate>    m_CSParticleUpdate;
    Ptr<CDreamParticleUpdate>    m_CSParticleUpdate;

    Ptr<CTexture>           m_ParticleTex;
    vector<Ptr<CTexture>>   m_vecParticleTex;
    Ptr<CTexture>           m_ParticleArrTex;

    float                   m_Time;
    bool                    m_bThisFrameGenerate = false;
    bool                    m_bThisFrameDelete = false;
    bool                    m_bDebug = true;


public:
    void GenerateParticle() {
        m_bThisFrameGenerate = true;
    }
    void DeleteParticle() {
        m_bThisFrameDelete = true;
    }

    void CreateDreamFab();
    void LoadDreamFab();

    virtual void UpdateData() override;
    virtual void finaltick() override;
    virtual void render() override;
    virtual void begin() override;
    void UpdateParallaxCorrection() ;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CDreamBlockParticleSystem);
public:
    tDreamParticleModule GetModule() {
        return m_Module;
    }
    void SetModule(tDreamParticleModule _Module) {
        m_Module = _Module;
    }
    CDreamBlockParticleSystem();
    CDreamBlockParticleSystem(const CDreamBlockParticleSystem& _OriginParticle);
    ~CDreamBlockParticleSystem();
};

