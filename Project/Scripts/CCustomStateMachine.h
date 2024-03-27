#pragma once

class Coroutine;
#include "CPlayerScript.h"

template <class T>
class CCustomStateMachine
{
private:
	//멤버는 int변수 밖에없지만 
	//직접 접근을 방지하기위한(setter강제) innerclass

	//typedef void (T::* voidRetFunc)();
	//typedef int (T::* intRetFunc)();

private:
	class StateManager
	{
	private:
		int m_PrevState;
		int m_CurState;
		CCustomStateMachine* m_Outer;

	private:
		StateManager() : m_PrevState(-1), m_CurState(-1), m_Outer(nullptr) {}

	public:
		StateManager(CCustomStateMachine* _Outer) :StateManager() { m_Outer = _Outer; };
		~StateManager() {};
		int GetPrevStateInt() { return m_CurState; }
		int GetCurStateInt() { return m_CurState; }
		void SetCurState(int _ToState);
		void ForceSetCurState(int _ToState);
		T* GetOwner() { return m_Outer->m_pOwner;  }


		//bool operator==(const StateManager& other) const
		//{
		//	return m_PrevState == other.m_PrevState && m_CurState == other.m_CurState && m_Outer == other.m_Outer;
		//}

		//StateManager(int m_PrevState, int m_CurState, CustomStateMachine* m_Outer)
		//	: m_PrevState(m_PrevState), m_CurState(m_CurState), m_Outer(m_Outer)
		//{
		//}
	};
	StateManager m_StateMgr;
	T* m_pOwner;

public:
	bool m_bChangedStates;
	bool m_bLog;
	bool m_bLocked;

	vector<void (T::*)()> m_vecBegins;
	vector<int (T::*)()> m_vecUpdates;
	vector<void (T::*)()> m_vecEnds;
	vector<void (T::*)()> m_vecCoroutines;
	vector<string> m_vecStateStrings;
	Coroutine* currentCoroutine;

private:
	CCustomStateMachine() = delete;

public:
	//void SetCallbacks(int _State, string _StateName , intRetFunc _UpdateFunc, voidRetFunc _CoroutineFunc = NULL, voidRetFunc _BeginFunc = NULL, voidRetFunc _EndFunc = NULL);
	void SetCallbacks(int _State, string _StateName , int (T::* _Update)(), void (T::* CoRoutine)() = nullptr, void (T::* _Begin)() = nullptr, void (T::* _End)() = nullptr);
	void Update();
	void ForceState(int _ToState) { m_StateMgr.ForceSetCurState(_ToState); }
	void SetCurState(int _ToState) { m_StateMgr.SetCurState(_ToState); }
	int GetCurState() { return m_StateMgr.GetCurStateInt(); }
	int GetPrevState() { return m_StateMgr.GetPrevStateInt(); }

	//로깅용
	void ReflectState(CGameObject* from, int index, string name);
	void LogState(int index);
	void LogAllStates();


	//CCustomStateMachine(T* _pOwner, int maxStates = 1);
	CCustomStateMachine(T* _pOwner, int maxStates=1)
		:m_pOwner(_pOwner),
		m_StateMgr(StateManager(this)),
		m_bChangedStates(false),
		m_bLog(false),
		m_bLocked(false)
	{
		m_vecStateStrings.resize(maxStates);
		m_vecBegins.resize(maxStates, nullptr);
		m_vecUpdates.resize(maxStates, nullptr);
		m_vecEnds.resize(maxStates, nullptr);
	}

	~CCustomStateMachine() 
	{
	};

};


class Coroutine 
{
	Coroutine() {};
	~Coroutine() {};
};

