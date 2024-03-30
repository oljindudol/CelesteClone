#include "pch.h"
#include "CEntity.h"


UINT CEntity::g_NextID = 0;

CEntity::CEntity()
	: m_ID(g_NextID++)	
	,m_facing(Facings::FacingsRight)
{
}

CEntity::CEntity(const CEntity& _Origin)
	: m_ID(g_NextID++)
	, m_strName(_Origin.m_strName)
	, m_facing(_Origin.m_facing)
{
}

CEntity::~CEntity()
{
}
