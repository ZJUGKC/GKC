/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
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
This file contains FSA Table classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __FSA_TABLE_H__
#define __FSA_TABLE_H__
////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// for constant definition

// These macros are used for class of state machine traits

// FSA_BEGIN_TRAITS_STATE_MAP(_class)
#define FSA_BEGIN_TRAITS_STATE_MAP(_class) \
	typedef _class _TraitsGetStateMap; \
	BEGIN_NOINLINE  \
	static const FSA_STATE_ITEM* GetStateTable(int& iMaxStateNo) throw() \
	END_NOINLINE \
	{

// FSA_BEGIN_STATE_TRANSITION(state_name)
//   state_name : Specify the state name or state enumerator.
#define FSA_BEGIN_STATE_TRANSITION(state_name) \
	static const FSA_TRANSITION_ITEM _ti##state_name[] = {

// FSA_STATE_TRANSITION_ENTRY(event_no, state_next)
//   event_no : Specify the event No.
//   state_next : Specify the next state.
#define FSA_STATE_TRANSITION_ENTRY(event_no, state_next) \
	{event_no, event_no, state_next},

// FSA_STATE_TRANSITION_RANGE_ENTRY(event_first, event_last, state_next)
//   event_first : Specify the first event No.
//   event_last : Specify the last event No.
//   state_next : Specify the next state.
#define FSA_STATE_TRANSITION_RANGE_ENTRY(event_first, event_last, state_next) \
	{event_first, event_last, state_next},

// FSA_END_STATE_TRANSITION()
#define FSA_END_STATE_TRANSITION() \
	{FSA_LAST_EVENT_NO, FSA_LAST_EVENT_NO, 0} };

// FSA_BEGIN_STATE_SET()
#define FSA_BEGIN_STATE_SET() \
	static const FSA_STATE_ITEM map[] = { \
	{ NULL, 0, 0 },

// FSA_STATE_SET_ENTRY(default_state, state_name, match_index)
//   default_state : The default state.
//   state_name : Specify the state name or state enumerator.
//   match_index : Specify the match index.
// The first entry has the index whose value is FSA_STATE_START. This map has at least one entry.
#define FSA_STATE_SET_ENTRY(default_state, state_name, match_index) \
	{_ti##state_name, default_state, match_index},

// FSA_END_STATE_SET()
#define FSA_END_STATE_SET() \
	{NULL, 0, 0} };

// FSA_END_TRAITS_STATE_MAP()
#define FSA_END_TRAITS_STATE_MAP() \
	iMaxStateNo = (int)(sizeof(map) / sizeof(FSA_STATE_ITEM)) - 2; return map; \
	}

// FSA_BEGIN_TRAITS_MATCH_MAP(_class)
#define FSA_BEGIN_TRAITS_MATCH_MAP(_class) \
	typedef _class _TraitsGetMatchMap; \
	BEGIN_NOINLINE  \
	static const FSA_MATCH_ITEM* GetMatchTable(int& iMaxMatchNo) throw() \
	END_NOINLINE  \
	{ \
		static const FSA_MATCH_ITEM map[] = {

// FSA_STATE_MATCH_ENTRY(match)
//   match : Specify the match No. 0 means no match. If this value is less than zero, the last event should be unput. This map has at least one entry.
#define FSA_STATE_MATCH_ENTRY(match) \
	{match},

// FSA_END_TRAITS_MATCH_MAP()
#define FSA_END_TRAITS_MATCH_MAP() \
	{ 0 } }; iMaxMatchNo = (int)(sizeof(map) / sizeof(FSA_MATCH_ITEM)) - 2; return map; \
	}

//------------------------------------------------------------------------------
// FsaTableInPool
//   FSA tables are defined in a pool

class FsaTableInPool
{
public:
	FsaTableInPool() throw() : m_pState(NULL), m_iMaxState(0), m_pMatch(NULL), m_iMaxMatch(0)
	{
	}
	~FsaTableInPool() throw()
	{
	}

	//state: 1---iMaxState
	//transition: the count of arrTransitionNum is iMaxState + 1
	//match: 0---iMaxMatch
	void Allocate(int iMaxState, const int* arrTransitionNum, int iMaxMatch)
	{
		assert( iMaxState > 1 && iMaxMatch > 0 );
		assert( m_pState == NULL && m_pMatch == NULL );
		//transition number
		int total_transition_num = 0;
		for( int i = 1; i <= iMaxState; i ++ ) {
			assert( arrTransitionNum[i] > 0 );
			total_transition_num += arrTransitionNum[i];
		}
		//allocate
		uint uBytes, uBytes1, uBytes2;
		uBytes1 = SafeOperators::MultiplyThrow((uint)iMaxState + 1, (uint)sizeof(FSA_STATE_ITEM));  //may throw
		uBytes2 = SafeOperators::MultiplyThrow((uint)iMaxMatch + 1, (uint)sizeof(FSA_MATCH_ITEM));  //may throw
		uBytes  = SafeOperators::AddThrow(uBytes1, uBytes2);  //may throw
		uBytes1 = SafeOperators::MultiplyThrow((uint)total_transition_num, (uint)sizeof(FSA_TRANSITION_ITEM));  //may throw
		uBytes  = SafeOperators::AddThrow(uBytes, uBytes1);  //may throw
		uint uStart = m_alloc.Allocate(uBytes);  //may throw
		//pointers
		m_pState = (FSA_STATE_ITEM*)m_alloc.ToPtr(uStart);
		m_pState[0].iDefaultState = 0;
		m_pState[0].pTransition = NULL;
		m_pState[0].iMatchIndex = 0;
		m_pMatch = (FSA_MATCH_ITEM*)((byte*)m_pState + sizeof(FSA_STATE_ITEM) * (iMaxState + 1));
		m_pMatch[0].iMatch = 0;
		FSA_TRANSITION_ITEM* pItem = (FSA_TRANSITION_ITEM*)((byte*)m_pMatch + sizeof(FSA_MATCH_ITEM) * (iMaxMatch + 1));
		for( int i = 1; i <= iMaxState; i ++ ) {
			m_pState[i].pTransition = pItem;
			pItem += arrTransitionNum[i];
		}
		//parameters
		m_iMaxState = iMaxState;
		m_iMaxMatch = iMaxMatch;
	}
	//set state
	void SetState(int iState, int iDefaultState, int iMatchIndex) throw()
	{
		assert( iState > 0 && iState <= m_iMaxState );
		assert( m_pState != NULL );
		m_pState[iState].iDefaultState = iDefaultState;
		m_pState[iState].iMatchIndex = iMatchIndex;
	}
	//set match table
	void SetMatch(int iMatchIndex, int iMatch) throw()
	{
		assert( iMatchIndex > 0 && iMatchIndex <= m_iMaxMatch );
		assert( m_pMatch != NULL );
		m_pMatch[iMatchIndex].iMatch = iMatch;
	}

	const FSA_STATE_ITEM* GetStateTable(int& iMaxStateNo) const throw()
	{
		iMaxStateNo = m_iMaxState;
		return m_pState;
	}
	const FSA_MATCH_ITEM* GetMatchTable(int& iMaxMatchNo) const throw()
	{
		iMaxMatchNo = m_iMaxMatch;
		return m_pMatch;
	}

private:
	DataPoolAllocator m_alloc;  //pool
	//tables
	FSA_STATE_ITEM* m_pState;
	int m_iMaxState;
	FSA_MATCH_ITEM* m_pMatch;
	int m_iMaxMatch;

private:
	//noncopyable
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
