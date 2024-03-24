#pragma once



class CPhysics
{
protected:
	// ��
	Vector3 m_vForce; 
	// ���ӵ�
	Vector3 m_vAccel; 
	// ����
	float m_fMass;	
	// �ӵ�
	Vector3 m_vVelocity; 
	// ���� ����
	float m_fDrag; 

	// �ӷ�
	float m_fSpeed; 
	// �ִ� �ӷ�
	float m_fMaxSpeed; 

	// TOOD : ������ ����� ������ �߰�
	// ������
	float m_fFriction; 
	// ������ ����
	bool m_bIsFrictionActive; 

	// �߷� ���ӵ�
	Vector3 m_vGravityAccel; 
	// �߷� ��� �÷���
	bool m_bUseGravity; 
	// ���� ȿ�� ���ֱ� �÷���
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
	// �ִ� �ӵ�
	void SetMaxSpeed(float _fMaxSpeed) { m_fMaxSpeed = _fMaxSpeed; } 
	float GetMaxSpeed() { return m_fMaxSpeed; }
	// ���� (0 ~ alpha)
	void SetMass(float _fMass) { m_fMass = max(_fMass, 0.0f); } 
	float GetMass() const { return m_fMass; }
	void SetVelocity(Vector3 _fVelocity) { m_vVelocity = _fVelocity; }
	const Vector3& GetVelocity() const { return m_vVelocity; }
	// ���� ���� (0 ~ alpha)
	void SetDrag(float _fDrag) { m_fDrag = max(_fDrag, 0.0f); } 
	float GetDrag() const { return m_fDrag; }
	float GetSpeed() { return m_fSpeed; }
	// ȸ������ ���� ����
	// inline void SetAngularDrag(float _fAngularDrag); 
	// �߷� ��� ���� ����
	void UseGravity(bool _bUseGravity) { m_bUseGravity = _bUseGravity; } 
	bool IsOnGravity() const { return m_bUseGravity; }
	// ���� ȿ�� ���ֱ�
	void IsKinematic(bool _bIsKinematic) { m_bIsKinematic = _bIsKinematic; } 
	bool IsOnKinematic() const { return m_bIsKinematic; }

public:
	// ���� �߰��ϴ�.
	void AddForce(Vector3 _vForce) { m_vForce += -_vForce; } 
	// �ӵ��� �߰��ϴ�.
	void AddVelocity(Vector3 _vVelocity) { m_vVelocity += _vVelocity; } 
	//read only
	const Vector3& GetForce() { return m_vForce; } 

	//���� ������Ʈ�� �����ϴ�.
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
