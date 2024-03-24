#include "pch.h"
#include "CPlatFormScript.h"

CPlatFormScript::CPlatFormScript()
	: CScript(PLATFORMSCRIPT)
{
}

CPlatFormScript::~CPlatFormScript()
{
}


void CPlatFormScript::begin()
{
}

void CPlatFormScript::tick()
{
}

void CPlatFormScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlatFormScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlatFormScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlatFormScript::SaveToFile(FILE* _File)
{
}

void CPlatFormScript::LoadFromFile(FILE* _File)
{
}

