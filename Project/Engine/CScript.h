#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
//#include "CResMgr.h"

#include "CGameObject.h"
#include "components.h"
class CPhysics;

struct tScriptParam
{
    SCRIPT_PARAM    Type;    
    //string          Desc;
    void*           pData;
};

class CScript :
    public CComponent
{
private:
    const UINT              m_iScriptType;
    //vector<tScriptParam>    m_ScriptParam;
    std::unordered_map<string, tScriptParam>  m_umScriptParam;
    CPhysics* m_Physics;


public:
    UINT GetScriptType() { return m_iScriptType; }
    const auto& GetScriptParam() { return m_umScriptParam; }


protected:
    void Instantiate(Ptr<CPrefab> _Prefab, Vec3 _vWorldPos, int _LayerIdx);
    //void AddScriptParam(SCRIPT_PARAM _Param, const string& _Desc, void* _Data) { m_ScriptParam.push_back(tScriptParam{ _Param , _Desc, _Data }); }
    void AppendScriptParam(const string& _Key, SCRIPT_PARAM _Param, void* _Data) { m_umScriptParam[_Key] = tScriptParam{ _Param,_Data }; }

public:
    virtual void finaltick() final {}
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}

    virtual void SaveToFile(FILE* _File) = 0;
    virtual void LoadFromFile(FILE* _File) = 0;

    CPhysics* GetPhysics() { return m_Physics; }
    void SetPhysics(CPhysics* _Physics) { m_Physics = _Physics; }

    CScript* Clone() = 0;
public:
    CScript(UINT _ScriptType);
    ~CScript();
};

