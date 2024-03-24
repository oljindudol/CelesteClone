#include "pch.h"
#include "CPhysics.h"
#include "Engine/CGameObject.h"
#include "Engine/CTimeMgr.h"
#include "Engine/components.h"


CPhysics::CPhysics()
	:m_vForce{},
	m_vAccel{},
	m_fMass(0.5f),
	m_vVelocity{},
	m_fDrag(0.2f),
	m_fSpeed(0.f),
	m_fMaxSpeed(15.f),
	m_fFriction(0.f),
	m_bIsFrictionActive(true),
	m_vGravityAccel(0.0f, 0.98066f, 0.0f), // default (0.0f, 9.80665f, 0.0f)
	m_bUseGravity(true),
	m_bIsKinematic(false),
	m_Owner(nullptr)
{
}

CPhysics::CPhysics(CGameObject* _Owner)
	:CPhysics()
{
	m_Owner = _Owner;
}

CPhysics::CPhysics(const CPhysics& _other)
	: m_vForce{ _other.m_vForce },
	m_vAccel{ _other.m_vAccel },
	m_fMass(_other.m_fMass),
	m_vVelocity{ _other.m_vVelocity },
	m_fDrag(_other.m_fDrag),
	m_fSpeed(_other.m_fSpeed),
	m_fMaxSpeed(_other.m_fMaxSpeed),
	m_fFriction(_other.m_fFriction),
	m_bIsFrictionActive(_other.m_fFriction),
	m_vGravityAccel{ _other.m_vGravityAccel },
	m_bUseGravity(_other.m_bUseGravity),
	m_bIsKinematic(_other.m_bIsKinematic),
	m_Owner(nullptr)
{
	// TODO : OwnerObject Setting
}




CPhysics::~CPhysics()
{

}

void CPhysics::Update()
{
	if (nullptr == m_Owner)
		return;
	if (m_bIsKinematic)
		return;

	if (0 == m_fMass)
		m_vAccel = {};
	else
		m_vAccel = m_vForce / m_fMass; // 가속도

	if (m_bUseGravity)
		m_vVelocity.y += m_vGravityAccel.y * DT;

	m_vVelocity = m_vVelocity + (m_vAccel * DT);

	// 공기 저항
	Vector3 vDragDir = m_vVelocity;
	vDragDir.Normalize();
	Vector3 vDragVec = -vDragDir * m_fDrag * DT;
	float fDragSpeed = vDragVec.Length();
	m_fSpeed = m_vVelocity.Length();
	if (fDragSpeed > m_fSpeed)
		m_vVelocity = Vector3(0.0f, 0.0f, 0.0f);
	else {
		m_vVelocity += vDragVec;
	}

	// 최대 속도 제한
	m_fSpeed = m_vVelocity.Length();
	float fMaxSpeed = m_fMaxSpeed * DT;
	if (m_fSpeed > fMaxSpeed) {
		m_vVelocity.Normalize();
		m_vVelocity = m_vVelocity * fMaxSpeed;
	}

	Vector3 vObjPos = m_Owner->Transform()->GetRelativePos();
	m_Owner->Transform()->SetRelativePos(vObjPos - m_vVelocity);

	m_vForce = Vector3(0.f, 0.f, 0.f);
}

//bool CPhysics::SaveToScene(FILE* _pFile)
//{
//	fwrite(m_vForce, _pFile);
//	fwrite(m_fMass, _pFile);
//	fwrite(m_fDrag, _pFile);
//	fwrite(m_fMaxSpeed, _pFile);
//	fwrite(m_fFriction, _pFile);
//	fwrite(m_bIsFrictionActive, _pFile);
//	fwrite(m_vGravityAccel, _pFile);
//	fwrite(m_bUseGravity, _pFile);
//	fwrite(m_bIsKinematic, _pFile);
//
//	return true;
//}
//
//bool CPhysics::LoadFromScene(FILE* _pFile)
//{
//	fread(m_vForce, _pFile);
//	fread(m_fMass, _pFile);
//	fread(m_fDrag, _pFile);
//	fread(m_fMaxSpeed, _pFile);
//	fread(m_fFriction, _pFile);
//	fread(m_bIsFrictionActive, _pFile);
//	fread(m_vGravityAccel, _pFile);
//	fread(m_bUseGravity, _pFile);
//	fread(m_bIsKinematic, _pFile);
//
//	return true;
//}