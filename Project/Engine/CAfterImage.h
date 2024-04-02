#pragma once
#include "CRenderComponent.h"

#include "CHair.h"

struct tAfterImageEvent
{
    //공통
    float Duration;
    float AccTime;
    //플레이어
    Matrix PlayerWorldMat;
    Facings facing;
    //알파값도 여기에 직접세팅
    Color Color;
    Ptr<CTexture> PlayerAnimTex;

    tRenderInfo HairInfo;
};


class CAfterImage :
    public CRenderComponent
{
private:
    Ptr<CTexture> m_HairTex;
    vector<Ptr<CTexture>> m_vecBangTex;

    void RenderPlayer(Color Color, Vec3 offset = Vec3(0, 0, 0));
    void RenderHair(Color Color, Vec3 offset = Vec3(0, 0, 0));
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

