/*
** Copyright (c) 2021, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

/*
This file contains framework classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_FRAME_H__
#define __GKC_FRAME_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_COLLB_H__
	#error GkcFrame.h requires GkcCollb.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//classes

// Command
//   bool(<parameters>)

// Property Notification

typedef delegate_base<void(uint)>  PropertyNotification;

class PropertyTrigger
{
public:
	PropertyTrigger() noexcept
	{
	}
	PropertyTrigger(const PropertyTrigger&) = delete;
	PropertyTrigger& operator=(const PropertyTrigger&) = delete;
	~PropertyTrigger() noexcept
	{
	}

	uintptr AddNotification(const RefPtr<PropertyNotification>& pn) noexcept
	{
		return m_arr.Add(pn);
	}
	void RemoveNotification(uintptr uCookie) noexcept
	{
		m_arr.Remove(uCookie);
	}

	void Fire(uint uID)
	{
		for( uintptr index = 0; index < m_arr.GetCount(); index ++ ) {
			RefPtr<PropertyNotification> pn;
			m_arr.GetObject(index, pn);
			if( !pn.IsNull() )
				pn.Deref()(uID);
		}
	}

private:
	SObjArray<RefPtr<PropertyNotification>> m_arr;
};

// State machine

typedef delegate_base<int(uint)>  StateFunc;

class StateManager
{
public:
	StateManager() noexcept : m_iCurrentState(0), m_map(MemoryHelper::GetCrtMemoryManager())
	{
	}
	StateManager(const StateManager&) = delete;
	StateManager& operator=(const StateManager&) = delete;
	~StateManager() noexcept
	{
	}

	void Add(int iState, const RefPtr<StateFunc>& sf)
	{
		m_map.Insert(iState, sf);  //may throw
	}
	void SetStartState(int iStartState) noexcept
	{
		m_iCurrentState = iStartState;
	}
	void Process(uint uEvent)
	{
		auto iter(m_map.Find(m_iCurrentState));
		if( iter != m_map.GetEnd() )
			m_iCurrentState = iter.get_Value().get_Second().Deref()(uEvent);  //may throw
	}
	int GetCurrentState() const noexcept
	{
		return m_iCurrentState;
	}

private:
	int m_iCurrentState;
	RBMap<int, RefPtr<StateFunc>> m_map;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
