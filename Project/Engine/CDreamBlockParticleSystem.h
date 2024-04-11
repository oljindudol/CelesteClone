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

    float                   m_randomtime = 0.f;

    float                   wobbleEase = 0.f;
    float                   wobbleFrom = 0.f;
    float                   wobbleTo = 0.f;

    //Ptr<CMesh>              m_LineMesh;
    Ptr<CMaterial>          m_LineMat;

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
    void WoolbeUpdate();
    void DrawWoobleLineGenerateCPU(Vec2 from, Vec2 to, float _z);
    void DrawWoobleLineGenerateCPU(Vec2 from, Vec2 to, float _z, vector<float>& _out_from, vector<float>& _out_to);
    void DrawWoobleLineGenerateGPU(Vec2 _from, Vec2 _to, float _z);
    void DrawLine(Vec2 _from,Vec2 _to , float _z);
    float RandomInterVal();

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

