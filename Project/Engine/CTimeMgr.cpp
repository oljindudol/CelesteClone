#include "pch.h"
#include "CTimeMgr.h"

#include "CEngine.h"
#include "CKeyMgr.h"
#include "CFontMgr.h"

CTimeMgr::CTimeMgr()
	: m_Frequency{}
	, m_PrevCount{}
	, m_CurCount{}
	, m_Time(0.f)
	, m_bLock(true)
	, m_szText{}
{	

}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::init()
{
	// 초당 빈도
	QueryPerformanceFrequency(&m_Frequency);

	QueryPerformanceCounter(&m_PrevCount);

	m_FreezeTime = 0.f;
}

void CTimeMgr::tick()
{
	QueryPerformanceCounter(&m_CurCount);

	m_EngineDeltaTime = m_DeltaTime = double(m_CurCount.QuadPart - m_PrevCount.QuadPart) / double(m_Frequency.QuadPart);

	if (m_FreezeTime>0.f)
	{
		m_FreezeTime -= m_DeltaTime;
		m_DeltaTime = 0.f;
	}


	if (m_bLock)
	{
		m_DeltaTime = 0.f;
	}

	m_PrevCount = m_CurCount;

	// DT 보정
	if ((1.f / 60.f) < m_DeltaTime)
		m_DeltaTime = (1. / 60.);

	// 시간 누적 ==> 1초마다 if 구문 실행
	m_Time += m_EngineDeltaTime;
	m_szText[50] = {};
	if (1.f <= m_Time)
	{		
		swprintf_s(m_szText, 50, L"DeltaTime : %f, FPS : %d", m_DeltaTime, m_iCall);
		SetWindowText(CEngine::GetInst()->GetMainWind(), m_szText);
		m_iCall = 0;
		m_Time = 0.f;
	}

	++m_iCall;	
	g_global.g_time += (float)m_DeltaTime;	

	//if(KEY_PRESSED(F))
	//{
	//	Freeze(.05f);
	//}
	
	//m_DeltaTime *= 0.1f;

}

void CTimeMgr::render()
{
	// 폰트 출력
	//CFontMgr::GetInst()->DrawFont(m_szText, 10.f, 30.f, 16, FONT_RGBA(255, 30, 30, 255));
}