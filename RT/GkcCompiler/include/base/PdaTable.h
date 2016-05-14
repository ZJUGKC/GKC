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
This file contains PDA Table classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __PDA_TABLE_H__
#define __PDA_TABLE_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// for constant definition

// These macros are used for class of PDA traits

// PDA_BEGIN_TRAITS_STATE_MAP(_class)
#define PDA_BEGIN_TRAITS_STATE_MAP(_class) \
	typedef _class _TraitsGetStateMap; \
	BEGIN_NOINLINE  \
	static const PDA_STATE_ITEM* GetStateTable(int& iMaxStateNo) throw() \
	END_NOINLINE \
	{

// PDA_BEGIN_STATE_TRANSITION(state_name)
//   state_name : Specify the state name or state enumerator.
#define PDA_BEGIN_STATE_TRANSITION(state_name) \
	static const PDA_TRANSITION_ITEM _ti##state_name[] = {

// PDA_STATE_TRANSITION_ENTRY(event_no, state_next)
//   event_no : Specify the event No.
//   state_next : Specify the next state.
#define PDA_STATE_TRANSITION_ENTRY(event_no, state_next) \
	{event_no, event_no, state_next},

// PDA_STATE_TRANSITION_RANGE_ENTRY(event_first, event_last, state_next)
//   event_first : Specify the first event No.
//   event_last : Specify the last event No.
//   state_next : Specify the next state.
#define PDA_STATE_TRANSITION_RANGE_ENTRY(event_first, event_last, state_next) \
	{event_first, event_last, state_next},

// PDA_END_STATE_TRANSITION()
#define PDA_END_STATE_TRANSITION() \
	{PDA_LAST_EVENT_NO, PDA_LAST_EVENT_NO, 0} };

// PDA_BEGIN_STATE_SET()
#define PDA_BEGIN_STATE_SET() \
	static const PDA_STATE_ITEM map[] = { \
	{ NULL, PDA_STATE_ERROR }, { NULL, PDA_STATE_ERROR },

// PDA_STATE_SET_ENTRY(state_name)
//   state_name : Specify the state name or state enumerator.
// The first entry has the index whose value is PDA_STATE_START. This map has at least two entries.
#define PDA_STATE_SET_ENTRY(state_name) \
	{_ti##state_name, PDA_STATE_ERROR},

// PDA_END_STATE_SET()
#define PDA_END_STATE_SET() \
	{NULL, PDA_STATE_ERROR} };

// PDA_END_TRAITS_STATE_MAP()
#define PDA_END_TRAITS_STATE_MAP() \
	iMaxStateNo = (int)(sizeof(map) / sizeof(PDA_STATE_ITEM)) - 2; return map; \
	}

// PDA_BEGIN_TRAITS_RULE_MAP(_class)
#define PDA_BEGIN_TRAITS_RULE_MAP(_class) \
	typedef _class _TraitsGetRuleMap; \
	BEGIN_NOINLINE  \
	static const PDA_RULE_ITEM* GetRuleTable(int& iMaxRuleNo) throw() \
	END_NOINLINE  \
	{ \
		static const PDA_RULE_ITEM map[] = {

// PDA_RULE_ENTRY(event, number)
//   This map has at least one entry. (S -> start_nonterminal $)
#define PDA_RULE_ENTRY(event, number) \
	{event, number},

// PDA_END_TRAITS_RULE_MAP()
#define PDA_END_TRAITS_RULE_MAP() \
	{ 0, 0 } }; iMaxRuleNo = (int)(sizeof(map) / sizeof(PDA_RULE_ITEM)) - 2; return map; \
	}

//------------------------------------------------------------------------------
// PdaTableInPool
//   PDA tables are defined in a pool

class PdaTableInPool
{
public:
	PdaTableInPool() throw() : m_pState(NULL), m_iMaxState(0), m_pRule(NULL), m_iMaxRule(0)
	{
	}
	~PdaTableInPool() throw()
	{
	}

	//state: 2---iMaxState
	//transition: the count of arrTransitionNum is iMaxState + 1
	//rule: 0---iMaxRule
	void Allocate(int iMaxState, const int* arrTransitionNum, int iMaxRule)
	{
		assert( iMaxState > 2 && iMaxRule > 0 );
		assert( m_pState == NULL && m_pRule == NULL );
		//transition number
		int total_transition_num = 0;
		for( int i = 2; i <= iMaxState; i ++ ) {
			assert( arrTransitionNum[i] > 0 );
			total_transition_num += arrTransitionNum[i];
		}
		//allocate
		uint uBytes, uBytes1, uBytes2;
		uBytes1 = SafeOperators::MultiplyThrow((uint)iMaxState + 1, (uint)sizeof(PDA_STATE_ITEM));  //may throw
		uBytes2 = SafeOperators::MultiplyThrow((uint)iMaxRule + 1, (uint)sizeof(PDA_RULE_ITEM));  //may throw
		uBytes  = SafeOperators::AddThrow(uBytes1, uBytes2);  //may throw
		uBytes1 = SafeOperators::MultiplyThrow((uint)total_transition_num, (uint)sizeof(PDA_TRANSITION_ITEM));  //may throw
		uBytes  = SafeOperators::AddThrow(uBytes, uBytes1);  //may throw
		uint uStart = m_alloc.Allocate(uBytes);  //may throw
		//pointers
		m_pState = (PDA_STATE_ITEM*)m_alloc.ToPtr(uStart);
		m_pState[0].iDefaultState = 0;
		m_pState[0].pTransition = NULL;
		m_pState[1].iDefaultState = 0;
		m_pState[1].pTransition = NULL;
		m_pRule = (PDA_RULE_ITEM*)((byte*)m_pState + sizeof(PDA_STATE_ITEM) * (iMaxState + 1));
		m_pRule[0].uLeftEventNo = 0;
		m_pRule[0].uRightSymbolNumber = 0;
		PDA_TRANSITION_ITEM* pItem = (PDA_TRANSITION_ITEM*)((byte*)m_pRule + sizeof(PDA_RULE_ITEM) * (iMaxRule + 1));
		for( int i = 2; i <= iMaxState; i ++ ) {
			m_pState[i].pTransition = pItem;
			pItem += arrTransitionNum[i];
		}
		//parameters
		m_iMaxState = iMaxState;
		m_iMaxRule  = iMaxRule;
	}

	//set state
	void SetState(int iState, int iDefaultState) throw()
	{
		assert( iState > 1 && iState <= m_iMaxState );
		assert( m_pState != NULL );
		m_pState[iState].iDefaultState = iDefaultState;
	}
	//set rule
	void SetRule(int iRule, uint uLeftEventNo, uint uRightSymbolNumber) throw()
	{
		assert( iRule > 0 && iRule <= m_iMaxRule );
		assert( m_pRule != NULL );
		m_pRule[iRule].uLeftEventNo = uLeftEventNo;
		m_pRule[iRule].uRightSymbolNumber = uRightSymbolNumber;
	}

	const PDA_STATE_ITEM* GetStateTable(int& iMaxStateNo) const throw()
	{
		iMaxStateNo = m_iMaxState;
		return m_pState;
	}
	const PDA_RULE_ITEM* GetRuleTable(int& iMaxRuleNo) const throw()
	{
		iMaxRuleNo = m_iMaxRule;
		return m_pRule;
	}

private:
	DataPoolAllocator m_alloc;  //pool
	//tables
	PDA_STATE_ITEM* m_pState;
	int m_iMaxState;
	PDA_RULE_ITEM*  m_pRule;
	int m_iMaxRule;

private:
	//noncopyable
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
