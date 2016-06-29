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

// --Only one trait class can be defined in a pair of .h and .cpp files.--

// --<header file>--

// DECLARE_PDA_TRAIT_CLASS(cls_name)
#define DECLARE_PDA_TRAIT_CLASS(cls_name)  \
	class cls_name { public:  \
	static const PDA_TABLE& GetTable() throw(); };

// --<.h end>--

// --<source file>--

// BEGIN_PDA_TRAIT_CLASS(cls_name)
#define BEGIN_PDA_TRAIT_CLASS(cls_name)

// PDA_BEGIN_STATE_TRANSITION(state_name)
//   state_name : Specify the state name or state enumerator.
#define PDA_BEGIN_STATE_TRANSITION(state_name)  \
	const PDA_TRANSITION_ITEM _ti##state_name[] = {

// PDA_STATE_TRANSITION_ENTRY(event_no, state_next)
//   event_no : Specify the event No.
//   state_next : Specify the next state.
#define PDA_STATE_TRANSITION_ENTRY(event_no, state_next)  \
	{event_no, event_no, state_next},

// PDA_STATE_TRANSITION_RANGE_ENTRY(event_first, event_last, state_next)
//   event_first : Specify the first event No.
//   event_last : Specify the last event No.
//   state_next : Specify the next state.
#define PDA_STATE_TRANSITION_RANGE_ENTRY(event_first, event_last, state_next)  \
	{event_first, event_last, state_next},

// PDA_END_STATE_TRANSITION()
#define PDA_END_STATE_TRANSITION()  \
	{PDA_LAST_EVENT_NO, PDA_LAST_EVENT_NO, 0} };

// PDA_BEGIN_STATE_SET()
#define PDA_BEGIN_STATE_SET()  \
	const PDA_STATE_ITEM _ti_state_map[] = {  \
	{NULL, PDA_STATE_ERROR}, {NULL, PDA_STATE_ERROR},

// PDA_STATE_SET_ENTRY(state_name)
//   state_name : Specify the state name or state enumerator.
// The first entry has the index whose value is PDA_STATE_START. This map has at least two entries.
#define PDA_STATE_SET_ENTRY(state_name)  \
	{_ti##state_name, PDA_STATE_ERROR},

// PDA_END_STATE_SET()
#define PDA_END_STATE_SET()  \
	{NULL, PDA_STATE_ERROR} };

// PDA_BEGIN_RULE()
#define PDA_BEGIN_RULE()  \
	const PDA_RULE_ITEM _ti_rule_map[] = {  \
	{0, 1},

// PDA_RULE_ENTRY(event, number)
//   This map has at least one entry. (S -> start_nonterminal $)
#define PDA_RULE_ENTRY(event, number)  \
	{event, number},

// PDA_END_RULE()
#define PDA_END_RULE()  \
	{0, 0} };

// END_PDA_TRAIT_CLASS(cls_name)
#define END_PDA_TRAIT_CLASS(cls_name)  \
const PDA_TABLE _ti_pda_table = {  \
_ti_state_map,  \
_ti_rule_map,  \
(int)(sizeof(_ti_state_map) / sizeof(PDA_STATE_ITEM)) - 2,  \
(int)(sizeof(_ti_rule_map) / sizeof(PDA_RULE_ITEM)) - 2  \
};  \
const PDA_TABLE& cls_name::GetTable() throw()  \
{ return _ti_pda_table; }

// --<.cpp end>--

//------------------------------------------------------------------------------
// PdaTable
//   PDA tables are defined by allocator

class PdaTable
{
public:
	explicit PdaTable(const RefPtr<IMemoryAllocatorRef32>& allocator) throw() : m_allocator(allocator)
	{
		Reset();
	}
	~PdaTable() throw()
	{
	}

	void Reset() throw()
	{
		_Init_Pda_Table(m_table);
	}

	//state: 2 ~ iMaxStateNo
	//transition: the count of arrTransitionNum is iMaxStateNo + 1
	//rule: 0 ~ iMaxRuleNo
	void Allocate(int iMaxStateNo, const ConstArray<int>& arrTransitionNum, int iMaxRuleNo)
	{
		assert( _Is_Pda_Size_Valid(iMaxStateNo, iMaxRuleNo) );
		assert( arrTransitionNum.GetCount() == (uintptr)(iMaxStateNo + 1) );
		assert( _Is_Pda_Table_Null(m_table) );
		//transition number
		int total_transition_num = 0;
		for( int i = 2; i <= iMaxStateNo; i ++ ) {
			int iNum = arrTransitionNum[i].get_Value();
			assert( iNum > 0 );
			iNum = SafeOperators::AddThrow(iNum, (int)1);  //may throw
			total_transition_num = SafeOperators::AddThrow(total_transition_num, iNum);  //may throw
		}
		//allocate
		uint uBytes, uBytes1, uBytes2;
		uBytes1 = SafeOperators::MultiplyThrow((uint)iMaxStateNo + 2, (uint)sizeof(PDA_STATE_ITEM));  //may throw
		uBytes2 = SafeOperators::MultiplyThrow((uint)iMaxRuleNo + 2, (uint)sizeof(PDA_RULE_ITEM));  //may throw
		uBytes  = SafeOperators::AddThrow(uBytes1, uBytes2);  //may throw
		uBytes1 = SafeOperators::MultiplyThrow((uint)total_transition_num, (uint)sizeof(PDA_TRANSITION_ITEM));  //may throw
		uBytes  = SafeOperators::AddThrow(uBytes, uBytes1);  //may throw
		uint uStart = m_allocator.Deref().Allocate(uBytes);
		if( uStart == 0 )
			throw OutOfMemoryException();
		//pointers
		PDA_STATE_ITEM* pState = (PDA_STATE_ITEM*)(m_allocator.Deref().ToPointer(uStart));
		pState[0].iDefaultState = 0;
		pState[0].pTransition = NULL;
		pState[1].iDefaultState = 0;
		pState[1].pTransition = NULL;
		pState[iMaxStateNo + 1].iDefaultState = 0;
		pState[iMaxStateNo + 1].pTransition = NULL;
		PDA_RULE_ITEM* pRule = (PDA_RULE_ITEM*)((byte*)pState + sizeof(PDA_STATE_ITEM) * (iMaxStateNo + 2));
		pRule[0].uLeftEventNo = 0;
		pRule[0].uRightSymbolNumber = 1;
		pRule[iMaxRuleNo + 1].uLeftEventNo = 0;
		pRule[iMaxRuleNo + 1].uRightSymbolNumber = 0;
		PDA_TRANSITION_ITEM* pItem = (PDA_TRANSITION_ITEM*)((byte*)pRule + sizeof(PDA_RULE_ITEM) * (iMaxRuleNo + 2));
		for( int i = 2; i <= iMaxStateNo; i ++ ) {
			pState[i].pTransition = pItem;
			pItem += (arrTransitionNum[i].get_Value());
			pItem->uEventFirstNo = PDA_LAST_EVENT_NO;
			pItem->uEventLastNo  = PDA_LAST_EVENT_NO;
			pItem->iNextState    = 0;
			pItem ++;
		}
		//parameters
		_Init_Pda_Table(m_table, iMaxStateNo, pState, iMaxRuleNo, pRule);
	}

	//set state
	void SetState(int iState, int iDefaultState) throw()
	{
		assert( iState > 1 && iState <= m_table.iMaxStateNo );
		assert( !_Is_Pda_Table_Null(m_table) );
		PDA_STATE_ITEM* pItem = const_cast<PDA_STATE_ITEM*>(m_table.pState);
		pItem[iState].iDefaultState = iDefaultState;
	}
	//set rule
	void SetRule(int iRule, uint uLeftEventNo, uint uRightSymbolNumber) throw()
	{
		assert( iRule > 0 && iRule <= m_table.iMaxRuleNo );
		assert( uLeftEventNo > 0 && uRightSymbolNumber > 0 );
		assert( !_Is_Pda_Table_Null(m_table) );
		PDA_RULE_ITEM* pItem = const_cast<PDA_RULE_ITEM*>(m_table.pRule);
		pItem[iRule].uLeftEventNo = uLeftEventNo;
		pItem[iRule].uRightSymbolNumber = uRightSymbolNumber;
	}
	//set transition
	void SetTransition(int iState, int iIndex, uint uEventFirstNo, uint uEventLastNo, int iNextState) throw()
	{
		assert( iState > 1 && iState <= m_table.iMaxStateNo );
		assert( !_Is_Pda_Table_Null(m_table) );
		PDA_TRANSITION_ITEM* pItem = const_cast<PDA_TRANSITION_ITEM*>(m_table.pState[iState].pTransition);
		assert( pItem != NULL );
		pItem[iIndex].uEventFirstNo = uEventFirstNo;
		pItem[iIndex].uEventLastNo  = uEventLastNo;
		pItem[iIndex].iNextState    = iNextState;
	}

	const PDA_TABLE& GetTable() const throw()
	{
		return m_table;
	}

private:
	RefPtr<IMemoryAllocatorRef32>  m_allocator;
	//tables
	PDA_TABLE  m_table;

private:
	//noncopyable
	PdaTable(const PdaTable&) throw();
	PdaTable& operator=(const PdaTable&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
