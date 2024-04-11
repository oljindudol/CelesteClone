#include "pch.h"
#include "CDreamBlockScript.h"
#include "CPlayerScript.h"

CDreamBlockScript::CDreamBlockScript()
	: CScript((UINT)SCRIPT_TYPE::PLATFORMSCRIPT)
{
}

CDreamBlockScript::~CDreamBlockScript()
{
}


void CDreamBlockScript::begin()
{
}

void CDreamBlockScript::tick()
{
}

void CDreamBlockScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	auto vs = _OtherObj->GetScripts();
	CPlayerScript* ps = nullptr;
	for (auto& s : vs)
	{
		ps = dynamic_cast<CPlayerScript*>(s);
		if (nullptr != ps)
		{
			break;
		}
	}

	if (nullptr != ps)
	{
		++ps->DreamBlockColCnt;
	}
}

void CDreamBlockScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CDreamBlockScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	auto vs = _OtherObj->GetScripts();
	CPlayerScript* ps = nullptr;
	for (auto& s : vs)
	{
		ps = dynamic_cast<CPlayerScript*>(s);
		if (nullptr != ps)
		{
			break;
		}
	}

	if (nullptr != ps)
	{
		int newcnt = ps->DreamBlockColCnt -1 ;
		if(-1 < newcnt)
		ps->DreamBlockColCnt = newcnt;
	}
}

void CDreamBlockScript::SaveToFile(FILE* _File)
{
}

void CDreamBlockScript::LoadFromFile(FILE* _File)
{
}


