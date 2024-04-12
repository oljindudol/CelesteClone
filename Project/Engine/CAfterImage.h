#pragma once
#include "CRenderComponent.h"

#include "CHair.h"

struct tAfterImageEvent
{
    //공통 정보.
    float Duration;
    float AccTime;

    //플레이어 정보.
    Matrix PlayerWorldMat;
    Matrix HairWorldMat;
    Vec2 vMult;
    Facings facing;
    //                알파값도 여기에 직접세팅
    Color Color;
    Ptr<CTexture> PlayerAnimTex;

    //헤어 정보.
    tRenderInfo HairInfo;
    
    //드림대쉬
    bool bDreamDash;
};


class CAfterImage :
    public CRenderComponent
{
private:
    Ptr<CTexture> m_HairTex;
    vector<Ptr<CTexture>> m_vecBangTex;
    tRenderInfo m_RenderInfo = {};

    void RenderPlayer(Color _Color, Facings _facing, Vec3 _MulScale = Vec3(1.f, 1.f, 1.f));
    void RenderHair(Color _Color, Vec3 _offset = Vec3(0, 0, 0));
    list< tAfterImageEvent> m_EventList;

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;
    void PushEvent(tAfterImageEvent _Event) { m_EventList.push_back(_Event); }
    CAfterImage();
    ~CAfterImage();

    CLONE(CAfterImage);


};

