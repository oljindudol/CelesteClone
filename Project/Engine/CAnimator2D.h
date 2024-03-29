#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnim;


class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnim*>    m_mapAnim;
    CAnim*                  m_CurAnim;
    bool                    m_bRepeat;
    Vec2                    m_MulScale;

public:
    void SetMulScale(Vec2 _MulScale) { m_MulScale = _MulScale; }
    Vec2 GetMulScale() { return m_MulScale; }

    virtual void finaltick() override;

    void UpdateData();
    static void Clear();

    // _LeftTop, _SliceSize, _Offset : Pixel Unit
    void Create(const wstring& _strKey, Ptr<CTexture> _AltasTex, Vec2 _LeftTop, Vec2 _vSliceSize, Vec2 _OffsetSize, Vec2 _Background, int _FrmCount, float _Delay);

    void CreateFromFolder(const wstring& _strKey, const wstring& _Path, float _Delay, Vec2 _offset = Vec2(0.f,0.f));

    CAnim* FindAnim(const wstring& _strAnimName);
    void Play(const wstring& _strAnimName, bool _bRepeat = true);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CAnim* GetCurAnim() { return m_CurAnim; }

    CLONE(CAnimator2D);
public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _OriginAnimator);
    ~CAnimator2D();

public:
    auto& GetMapAnim() { return m_mapAnim; }
};

