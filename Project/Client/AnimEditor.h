#pragma once
#include "UI.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CAsset.h>
#include <Engine\CTexture.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CAnim.h>

#include <Windows.h>
#include <commdlg.h>
#include <DirectXMath.h>

class AnimEditor :
    public UI
{
public:
    virtual void render_update() override;

public:
    AnimEditor();
    ~AnimEditor();

private:
    CGameObject* m_TargetGO;
    int m_targetidx;

    float m_acctime;
    int m_curframe;
    vector<bool> m_selectedfrm;
    bool m_isPlaying;

public:
    void SetpGO(CGameObject* _TagetAnim) { m_TargetGO = _TagetAnim; }
    void SetTargetIdx(int _TagetIdx) { m_targetidx = _TagetIdx; }

    void initialize()
    {
        m_acctime = 0.f;
        m_curframe = 0;
        m_selectedfrm.resize(0);
        m_isPlaying = true;
    }

};
