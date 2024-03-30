#include "pch.h"
#include "CAnim.h"

#include "CTimeMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CAnimator2D.h"



CAnim::CAnim()
	: m_Animator(nullptr)
	, m_CurFrmIdx(0)
	, m_bFinish(false)
	, m_AccTime(0.f)
	, m_iRoop(0)
{
}

CAnim::CAnim(const CAnim& _OriginAnim)
	: m_Animator(nullptr)
	, m_vecFrm(_OriginAnim.m_vecFrm)
	, m_CurFrmIdx(_OriginAnim.m_CurFrmIdx)
	, m_bFinish(_OriginAnim.m_bFinish)
	, m_AtlasTex(_OriginAnim.m_AtlasTex)
	, m_AccTime(_OriginAnim.m_AccTime)
{
}

CAnim::~CAnim()
{
}

void CAnim::finaltick()
{	
	m_AccTime += DT;

	if (m_vecFrm.size() <= m_CurFrmIdx)
	{
		return;
	}

	if (m_Duration < m_AccTime)
	{
		++m_CurFrmIdx;
		if (m_vecFrm.size() <= m_CurFrmIdx)
		{
			m_CurFrmIdx = (int)m_vecFrm.size() - 1;
			m_bFinish = true;
		}
		m_AccTime = 0.f;
	}
}

void CAnim::UpdateData()
{
	// 현재 프레임 정보를 상수버퍼로 옮기고 b2 레지스터로 바인딩
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
	tAnimData2D data = {};
	data.UseAnim2D = 1;
	data.vLeftTop = m_vecFrm[m_CurFrmIdx].vLeftTop;
	data.vOffset = m_vecFrm[m_CurFrmIdx].vOffset;
	data.vSlizeSize = m_vecFrm[m_CurFrmIdx].vSlice;
	data.vBackGround = m_vecFrm[m_CurFrmIdx].vBackground;
	auto vMul = m_Animator->GetMulScale();
	int facing =  (int)m_Animator->GetOwner()->m_facing;
	data.vMulScale = Vec3(facing * vMul.x, vMul.y, 1.f);

	pCB->SetData(&data);
	pCB->UpdateData();

	// 아틀라스 텍스쳐 t10 에 바인딩
	//m_AtlasTex->UpdateData(10);

	m_vecFrm[m_CurFrmIdx].pFrameTex->UpdateData(10);
}

void CAnim::Clear()
{
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
	tAnimData2D data = {};
	data.UseAnim2D = 0;

	pCB->SetData(&data);
	pCB->UpdateData();
}

void CAnim::Create(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, Vec2 _vLeftTop
	, Vec2 _vSliceSize, Vec2 _vOffset, Vec2 _vBackground, int _FrmCount, float _Delay)
{
	m_Animator = _Animator;
	m_AtlasTex = _Atlas;
	m_Duration = _Delay;

	for (int i = 0; i < _FrmCount; ++i)
	{
		tAnimFrm frm = {};
				
		frm.vSlice = Vec2(_vSliceSize.x / (float)_Atlas->GetWidth(), _vSliceSize.y / (float)_Atlas->GetHeight());

		frm.vLeftTop = Vec2(_vLeftTop.x / (float)_Atlas->GetWidth() + frm.vSlice.x * i, _vLeftTop.y / (float)_Atlas->GetHeight());

		frm.vOffset = Vec2(_vOffset.x / (float)_Atlas->GetWidth(), _vOffset.y / (float)_Atlas->GetHeight());
		//frm.Duration = 1.f / _FPS;

		frm.vBackground = Vec2(_vBackground.x / (float)_Atlas->GetWidth(), _vBackground.y / (float)_Atlas->GetHeight());
	

		m_vecFrm.push_back(frm);
	}
}

void CAnim::CreateFromFolder(CAnimator2D* _Animator, const wstring& _Path, float _Delay, Vec2 _offset)
{
	tAnimFrm frm = {};
	m_Animator = _Animator;
	m_Duration = _Delay;

	string s;
	s.assign(_Path.begin(),_Path.end());

	static const std::filesystem::path base_path = CPathMgr::GetContentPath();

	auto images = getImagesFromDirectory(s);

	for (int i = 0; i < images.size(); ++i)
	{
		tAnimFrm frm = {};

		std::filesystem::path relative_path = std::filesystem::relative(images[i], base_path);

		Ptr<CTexture> pFrameTex = CAssetMgr::GetInst()->Load<CTexture>(relative_path, relative_path);

		frm.pFrameTex = pFrameTex;

		frm.vSlice = Vec2(1.f, 1.f);
		frm.vLeftTop = Vec2(0.f,0.f);
		frm.vOffset = _offset;
		//frm.Duration = 1.f / _FPS;

		frm.vBackground = Vec2(1.f,1.f);

		m_vecFrm.push_back(frm);
	}
}

void CAnim::SaveToFile(FILE* _File)
{
	// 애니메이션 이름 저장
	SaveWString(GetName(), _File);
	
	// 모든 프레임 정보 저장
	size_t FrmSize = m_vecFrm.size();
	fwrite(&FrmSize, sizeof(size_t), 1, _File);
	fwrite(m_vecFrm.data(), sizeof(tAnimFrm), m_vecFrm.size(), _File);

	// 애니메이션이 참조하던 텍스쳐 정보 저장
	SaveAssetRef(m_AtlasTex, _File);
}

void CAnim::LoadFromFile(FILE* _File)
{
	// 애니메이션 이름 로드
	wstring strName;
	LoadWString(strName, _File);
	SetName(strName);
	
	// 모든 프레임 정보 로드
	size_t FrmSize = 0;
	fread(&FrmSize, sizeof(size_t), 1, _File);
	m_vecFrm.resize(FrmSize);
	fread(m_vecFrm.data(), sizeof(tAnimFrm), m_vecFrm.size(), _File);

	// 애니메이션이 참조하던 텍스쳐 정보 로드
	LoadAssetRef(m_AtlasTex, _File);
}
