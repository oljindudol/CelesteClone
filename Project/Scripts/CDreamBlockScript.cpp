#include "pch.h"
#include "CDreamBlockScript.h"

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
}

void CDreamBlockScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CDreamBlockScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CDreamBlockScript::SaveToFile(FILE* _File)
{
}

void CDreamBlockScript::LoadFromFile(FILE* _File)
{
}


