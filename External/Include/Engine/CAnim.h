#pragma once
#include "CEntity.h"

#include "CTexture.h"

struct tAnimFrm
{
    Vec2    vLeftTop;
    Vec2    vSlice;
    Vec2    vOffset;
    Vec2    vBackground;
    float   Duration;
};

class CAnimator2D;

class CAnim :
    public CEntity
{
private:
    CAnimator2D*        m_Animator;

    vector<tAnimFrm>    m_vecFrm;
    int                 m_CurFrmIdx;
    bool                m_bFinish;


    float               m_AccTime;

public:
    Ptr<CTexture>       m_AtlasTex;

    void finaltick();
    void UpdateData();

    bool IsFinish() { return m_bFinish; }
    void Reset()
    {
        m_bFinish = false;
        m_CurFrmIdx = 0;
        m_AccTime = 0.f;
    }

    static void Clear();
    void Create(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, Vec2 _vLeftTop
        , Vec2 _vSliceSize, Vec2 _vOffset, Vec2 _vBackground, int _FrmCount, float _FPS);


    void SaveToFile(FILE* _File);
    void LoadFromFile(FILE* _File);
    CLONE(CAnim);
public:
    CAnim();
    CAnim(const CAnim& _OriginAnim);
    ~CAnim();

public:
    auto& GetVecFrm() { return m_vecFrm; }
    void SetVecFrm(vector<tAnimFrm> _vecFrm) { m_vecFrm = _vecFrm; }

    friend class CAnimator2D;
};

