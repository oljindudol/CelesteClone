#include "pch.h"
#include "CCustomStateMachine.h"

//template <class T>
//CCustomStateMachine<T>::CCustomStateMachine(T* _pOwner,int maxStates)
//	:m_pOwner(_pOwner),
//	m_StateMgr(StateManager(this)),
//	m_bChangedStates(false),
//	m_bLog(false),
//	m_bLocked(false)
//{
//	m_vecStateStrings.resize(maxStates, NULL);
//	m_vecBegins.resize(maxStates, NULL);
//	m_vecUpdates.resize(maxStates, NULL);
//	m_vecEnds.resize(maxStates, NULL);
//}

//template <class T>
//CCustomStateMachine<T>::~CCustomStateMachine()
//{
//}


//template<class T>
//void CustomStateMachine<T>::SetCallbacks(int _State, string _StateName, intRetFunc _UpdateFunc, voidRetFunc _CoroutineFunc, voidRetFunc _BeginFunc, voidRetFunc _EndFunc)
//{
//	m_vecStateStrings[_State] = _StateName;
//	m_vecUpdates[_State] = _UpdateFunc;
//	m_vecBegins[_State] = _BeginFunc;
//	m_vecEnds[_State] = _EndFunc;
//	m_vecCoroutines[_State] = _CoroutineFunc;
//}

//template<class T>
//void CCustomStateMachine<T>::SetCallbacks(int _State, string _StateName, int(T::* _Update)(), void(T::* _Begin)(), void(T::* _End)(), void(T::* CoRoutine)())
//{
//	m_vecStateStrings[_State] = _StateName;
//	m_vecUpdates[_State] = _Update;
//	m_vecBegins[_State] = _Begin;
//	m_vecEnds[_State] = _End;
//	m_vecCoroutines[_State] = CoRoutine;
//}

//template <class T>
//void CCustomStateMachine<T>::Update()
//{
//	auto state = GetCurState();
//	
//	m_bChangedStates = false;
//	if (m_vecUpdates[state] != NULL)
//	{
//		int _toState = (m_pOwner->*m_vecUpdates[state])();
//		SetCurState(_toState);
//	}
//	if (currentCoroutine.Active)
//	{
//		currentCoroutine->Update();
//#pragma region Coroutine Finish Log
//		//if (!m_bChangedStates && Log && currentCoroutine->Finished)
//		//{
//		//	Calc.Log(new object[]
//		//		{
//		//			"Finished Coroutine " + state
//		//		});
//		//}
//#pragma endregion
//	}
//}


template <class T>
void CCustomStateMachine<T>::ReflectState(CGameObject* from, int index, string name)
{
}

template <class T>
void CCustomStateMachine<T>::LogState(int index)
{
}

template <class T>
void CCustomStateMachine<T>::LogAllStates()
{
}


//template <class T>
//void CCustomStateMachine<T>::StateManager::SetCurState(int _ToState)
//{
//	T* Ower = m_Outer->m_pOwner;
//
//
//	if (!m_Outer->m_bLocked && m_CurState != _ToState)
//	{
//#pragma region Enter Leaving State Log
//		//if (Log)
//		//{
//		//	Calc.Log(new object[]
//		//		{
//		//			string.Concat(new object[]
//		//			{
//		//				"Enter m_CurState ",
//		//				_CurState,
//		//				" (leaving ",
//		//				m_CurState,
//		//				")"
//		//			})
//		//		});
//		//}
//#pragma endregion
//		m_Outer->m_bChangedStates = true;
//		m_PrevState = m_CurState;
//		m_CurState = _ToState;
//		if (m_PrevState != -1 && m_Outer->m_vecEnds[m_PrevState] != nullptr)
//		{
//#pragma region Calling End Log
//			//if (Log)
//			//{
//			//	Calc.Log(new object[]
//			//		{
//			//			"Calling End " + PreviousState
//			//		});
//			//}
//#pragma endregion
//			(m_pOwner->*(m_Outer->m_vecEnds[m_PrevState]))() ;
//		}
//		if (m_Outer->m_vecBegins[m_CurState] != nullptr)
//		{
//#pragma region Calling Begin Log
//			//if (Log)
//			//{
//			//	Calc.Log(new object[]
//			//		{
//			//			"Calling Begin " + m_CurState
//			//		});
//			//}
//#pragma endregion
//			(m_pOwner->*(m_Outer->m_vecBegins[m_CurState]))();
//		}
//		if (m_Outer->m_vecCoroutines[m_CurState] != nullptr)
//		{
//#pragma region Calling Coroutine Log
//			//if (Log)
//			//{
//			//	Calc.Log(new object[]
//			//		{
//			//			"Starting Coroutine " + m_CurState
//			//		});
//			//}
//#pragma endregion
//			m_Outer->currentCoroutine->Replace(m_Outer->m_vecCoroutines[m_CurState]());
//			return;
//		}
//		m_Outer->currentCoroutine->Cancel();
//	}
//}

//template<class T>
//void CustomStateMachine<T>::StateManager<T>::SetCurState(int _ToState)
//{
//}

template <class T>
void CCustomStateMachine<T>::StateManager::ForceSetCurState(int _ToState)
{
	if (m_CurState != _ToState)
	{
		m_CurState = _ToState;
		return;
	}
#pragma region Enter&Leaving Log
	//if (Log)
	//{
	//	Calc.Log(new object[]
	//		{
	//			string.Concat(new object[]
	//			{
	//				"Enter State ",
	//				toState,
	//				" (leaving ",
	//				state,
	//				")"
	//			})
	//		});
	//}
#pragma endregion
	m_Outer->m_bChangedStates = true;
	m_PrevState = m_CurState;
	m_CurState = _ToState;
	if (m_PrevState != -1 && m_Outer->m_vecEnds[m_PrevState] != nullptr)
	{
#pragma region End State Log
		//if (Log)
		//{
		//	Calc.Log(new object[]
		//		{
		//			"Calling End " + state
		//		});
		//}
#pragma endregion
		(m_Outer->m_vecEnds[m_PrevState])();
	}
	if (m_Outer->m_vecBegins[m_CurState] != nullptr)
	{
#pragma region Begin State Log
		//if (Log)
		//{
		//	Calc.Log(new object[]
		//		{
		//			"Calling Begin " + state
		//		});
		//}
#pragma endregion
		m_Outer->m_vecBegins[m_CurState]();
	}
	if (m_Outer->m_vecCoroutines[m_CurState] != nullptr)
	{
#pragma region Corutine Log
		//if (Log)
		//{
		//	Calc.Log(new object[]
		//		{
		//			"Starting Coroutine " + state
		//		});
		//}
#pragma endregion
		m_Outer->currentCoroutine->Replace(m_Outer->m_vecCoroutines[m_CurState]());
		return;
	}
	m_Outer->currentCoroutine->Cancel();

}


