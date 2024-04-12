#pragma once
#include "CRenderComponent.h"

#include "CHair.h"

struct tAfterImageEvent
{
    //���� ����.
    float Duration;
    float AccTime;

    //�÷��̾� ����.
    Matrix PlayerWorldMat;
    Matrix HairWorldMat;
    Vec2 vMult;
    Facings facing;
    //                ���İ��� ���⿡ ��������
    Color Color;
    Ptr<CTexture> PlayerAnimTex;

    //��� ����.
    tRenderInfo HairInfo;
    
    //�帲�뽬
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

