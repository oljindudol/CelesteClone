#include "pch.h"
#include "CScript.h"
#include "Scripts/CPhysics.h"

CScript::CScript(UINT _ScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_iScriptType(_ScriptType)
	, m_Physics(nullptr)
{
}

CScript::~CScript()
{
	if (nullptr != m_Physics)
		delete m_Physics;
}

void CScript::Instantiate(Ptr<CPrefab> _Prefab, Vec3 _vWorldPos, int _layerIdx)
{
	if (nullptr == _Prefab)
		return;

	CGameObject* pNewObj = _Prefab->Instantiate();

	pNewObj->Transform()->SetRelativePos(_vWorldPos);

	GamePlayStatic::SpawnGameObject(pNewObj, _layerIdx);
}

void CScript::SaveToFile(FILE* _File)
{

}

void CScript::LoadFromFile(FILE* _File)
{
}