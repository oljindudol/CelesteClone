#pragma once

#include <Engine/CScript.h>
class CPhysics;

template <typename T>
class CCustomStateMachine;

class CPlayerScript :
    public CScript
{
private:
    Ptr<CPrefab>    m_Missile;
    CCustomStateMachine<CPlayerScript>* m_SM;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void NormalBegin();
    int NormalUpdate();
    void NormalEnd();
    void DashBegin();
    int DashUpdate();
    void DashEnd();

    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

