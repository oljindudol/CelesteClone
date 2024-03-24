#pragma once



class CPhysics
{
protected:
	// 힘
	Vector3 m_vForce; 
	// 가속도
	Vector3 m_vAccel; 
	// 질량
	float m_fMass;	
	// 속도
	Vector3 m_vVelocity; 
	// 공기 저항
	float m_fDrag; 

	// 속력
	float m_fSpeed; 
	// 최대 속력
	float m_fMaxSpeed; 

	// TOOD : 접촉할 경우의 마찰력 추가
	// 마찰력
	float m_fFriction; 
	// 마찰력 유무
	bool m_bIsFrictionActive; 

	// 중력 가속도
	Vector3 m_vGravityAccel; 
	// 중력 사용 플래그
	bool m_bUseGravity; 
	// 물리 효과 없애기 플래그
	bool m_bIsKinematic; 
	CGameObject* m_Owner;

public:
	virtual void PrevUpdate() {}
	virtual void Update();
	virtual void LateUpdate() {}

public:
	virtual bool SaveToScene(FILE* _pFile) {}
	virtual bool LoadFromScene(FILE* _pFile) {}

public:
	// 최대 속도
	void SetMaxSpeed(float _fMaxSpeed) { m_fMaxSpeed = _fMaxSpeed; } 
	float GetMaxSpeed() { return m_fMaxSpeed; }
	// 질량 (0 ~ alpha)
	void SetMass(float _fMass) { m_fMass = max(_fMass, 0.0f); } 
	float GetMass() const { return m_fMass; }
	void SetVelocity(Vector3 _fVelocity) { m_vVelocity = _fVelocity; }
	const Vector3& GetVelocity() const { return m_vVelocity; }
	// 공기 저항 (0 ~ alpha)
	void SetDrag(float _fDrag) { m_fDrag = max(_fDrag, 0.0f); } 
	float GetDrag() const { return m_fDrag; }
	float GetSpeed() { return m_fSpeed; }
	// 회전값에 대한 저항
	// inline void SetAngularDrag(float _fAngularDrag); 
	// 중력 사용 여부 설정
	void UseGravity(bool _bUseGravity) { m_bUseGravity = _bUseGravity; } 
	bool IsOnGravity() const { return m_bUseGravity; }
	// 물리 효과 없애기
	void IsKinematic(bool _bIsKinematic) { m_bIsKinematic = _bIsKinematic; } 
	bool IsOnKinematic() const { return m_bIsKinematic; }

public:
	// 힘을 추가하다.
	void AddForce(Vector3 _vForce) { m_vForce += -_vForce; } 
	// 속도를 추가하다.
	void AddVelocity(Vector3 _vVelocity) { m_vVelocity += _vVelocity; } 
	//read only
	const Vector3& GetForce() { return m_vForce; } 

	//주인 오브젝트를 설정하다.
	void SetOwner(CGameObject* _Owner) { m_Owner = _Owner; }
	CGameObject* GetOwner() { return m_Owner; }

protected:
	CPhysics(const CPhysics& _other);
	CPhysics();

public:
	CPhysics(CGameObject* _Owner);
	CLONE(CPhysics);
	~CPhysics();

	friend class CCollisionMgr;
};
