#pragma once
#include "CComponent.h"

enum class COLLIDER2D_TYPE
{
    COL_TYPE_RECT,
    COL_TYPE_CIRCLE,
    COL_TYPE_END,
};



class CCollider2D :
    public CComponent
{
private:
    Vec3            m_vOffsetPos;
    Vec3            m_vOffsetScale;
    int             m_CollisionCount;   // 다른 충돌체와 충돌중인 횟수
    bool            m_bAbsolute;

    Matrix          m_matColWorld;
    COLLIDER2D_TYPE m_Type;

public:
    void SetAbsolute(bool _bAbsol) { m_bAbsolute = _bAbsol; }
    void SetOffsetPos(Vec2 _vOffset) { m_vOffsetPos = Vec3(_vOffset.x, _vOffset.y, 0.f); }
    void SetOffsetScale(Vec2 _vOffsetScale) { m_vOffsetScale = Vec3(_vOffsetScale.x, _vOffsetScale.y, 1.f); }
    void SetColliderType(COLLIDER2D_TYPE _Type) { m_Type = _Type; }

    bool IsAbsolute() { return m_bAbsolute; }
    Vec2 GetOffsetPos() { return Vec2(m_vOffsetPos.x, m_vOffsetPos.y); }
    Vec2 GetOffsetScale() { return Vec2(m_vOffsetScale.x, m_vOffsetScale.y); }
    COLLIDER2D_TYPE GetType() { return m_Type; }

    const Matrix& GetColliderWorldMat() { return m_matColWorld; }

public:
    virtual void finaltick() override;

private:
    Vec2 m_WorldPos;
    Vec2 m_WorldScale;
    Vec2 m_PrevWolrdPos;
    Vec2 m_PrevWorldScale;

    void CalcWorldTransform();
    void UpdatePrevWorldTransform() { 
        m_PrevWolrdPos = m_WorldPos; 
        m_PrevWorldScale = m_WorldScale;
    }

    //void CalcWorldScale();
    //void UpdatePrevWorldScale() { }

public:
    Vec2& GetWorldPos() { return m_WorldPos; }
    Vec2& GetPrevWorldPos() { return m_PrevWolrdPos; }
    Vec2& GetWorldScale() { return m_WorldScale; }
    Vec2& GetPrevWorldScale() { return m_PrevWorldScale; }


public:
    void BeginOverlap(CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OtherCollider);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CCollider2D);
public:
    CCollider2D();
    CCollider2D(const CCollider2D& _OriginCollider2D);
    ~CCollider2D();
};

