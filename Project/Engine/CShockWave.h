#pragma once

#include "CRenderComponent.h"

struct tShockEvent
{
    Matrix ShockMat;
    float AccTime;
};


class CShockWave :
    public CRenderComponent
{
private:
    list<tShockEvent> m_ShockEventList;


public:
    void PushShockEvent(tShockEvent _ShockEvent) { m_ShockEventList.push_back(_ShockEvent); }
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;

    CShockWave();
    ~CShockWave();
    CLONE(CShockWave);

};

