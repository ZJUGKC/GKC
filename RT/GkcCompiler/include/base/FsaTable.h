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

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// for constant definition

// These macros are used for class of state machine traits

// --Only one trait class can be defined in a pair of .h and .cpp files.--

// --<header file>--

// DECLARE_FSA_TRAIT_CLASS(cls_name)
#define DECLARE_FSA_TRAIT_CLASS(cls_name)  \
	class cls_name { public:  \
	static const FSA_TABLE& GetTable() throw(); };

// --<.h end>--

// --<source file>--

// BEGIN_FSA_TRAIT_CLASS(cls_name)
#define BEGIN_FSA_TRAIT_CLASS(cls_name)

// FSA_BEGIN_STATE_TRANSITION(state_name)
//   state_name : Specify the state name or state enumerator.
#define FSA_BEGIN_STATE_TRANSITION(state_name)  \
	const FSA_TRANSITION_ITEM _ti##state_name[] = {

// FSA_STATE_TRANSITION_ENTRY(event_no, state_next)
//   event_no : Specify the event No.
//   state_next : Specify the next state.
#define FSA_STATE_TRANSITION_ENTRY(event_no, state_next)  \
	{event_no, event_no, state_next},

// FSA_STATE_TRANSITION_RANGE_ENTRY(event_first, event_last, state_next)
//   event_first : Specify the first event No.
//   event_last : Specify the last event No.
//   state_next : Specify the next state.
#define FSA_STATE_TRANSITION_RANGE_ENTRY(event_first, event_last, state_next)  \
	{event_first, event_last, state_next},

// FSA_END_STATE_TRANSITION()
#define FSA_END_STATE_TRANSITION()  \
	{FSA_LAST_EVENT_NO, FSA_LAST_EVENT_NO, 0} };

// FSA_BEGIN_STATE_SET()
#define FSA_BEGIN_STATE_SET()  \
	const FSA_STATE_ITEM _ti_state_map[] = {  \
	{NULL, 0, 0},

// FSA_STATE_SET_ENTRY(default_state, state_name, match_index)
//   default_state : The default state.
//   state_name : Specify the state name or state enumerator.
//   match_index : Specify the match index.
// The first entry has the index whose value is FSA_STATE_START. This map has at least one entry.
#define FSA_STATE_SET_ENTRY(default_state, state_name, match_index)  \
	{_ti##state_name, default_state, match_index},

// FSA_END_STATE_SET()
#define FSA_END_STATE_SET()  \
	{NULL, 0, 0} };

// FSA_BEGIN_MATCH()
#define FSA_BEGIN_MATCH()  \
	const FSA_MATCH_ITEM _ti_match_map[] = {  \
	{0},

// FSA_MATCH_ENTRY(match)
//   match : Specify the match No. 0 means no match. If this value is less than zero, the last event should be unput. This map has at least one entry.
#define FSA_MATCH_ENTRY(match)  \
	{match},

// FSA_END_MATCH()
#define FSA_END_MATCH()  \
	{0} };

// END_FSA_TRAIT_CLASS(cls_name)
#define END_FSA_TRAIT_CLASS(cls_name)  \
const FSA_TABLE _ti_fsa_table = {  \
(int)(sizeof(_ti_state_map) / sizeof(FSA_STATE_ITEM)) - 2,  \
(int)(sizeof(_ti_match_map) / sizeof(FSA_MATCH_ITEM)) - 2,  \
_ti_state_map,  \
_ti_match_map };  \
const FSA_TABLE& cls_name::GetTable() throw()  \
{ return _ti_fsa_table; }

// --<.cpp end>--

//------------------------------------------------------------------------------
// FsaTable
//   FSA tables are defined by allocator

class FsaTable
{
public:
	explicit FsaTable(const RefPtr<IMemoryAllocatorRef32>& allocator) throw() : m_allocator(allocator)
	{
		Reset();
	}
	~FsaTable() throw()
	{
	}

	void Reset() throw()
	{
		_Init_Fsa_Table(m_table);
	}

	//state: 1 ~ iMaxStateNo
	//transition: the count of arrTransitionNum is iMaxStateNo + 1
	//match: 0 ~ iMaxMatchNo
	void Allocate(int iMaxStateNo, const ConstArray<int>& arrTransitionNum, int iMaxMatchNo)
	{
		assert( _Is_Fsa_Size_Valid(iMaxStateNo, iMaxMatchNo) );
		assert( arrTransitionNum.GetCount() == (uintptr)(iMaxStateNo + 1) );
		assert( _Is_Fsa_Table_Null(m_table) );
		//transition number
		int total_transition_num = 0;
		for( int i = 1; i <= iMaxStateNo; i ++ ) {
			int iNum = arrTransitionNum[i].get_Value();
			assert( iNum > 0 );
			iNum = SafeOperators::AddThrow(iNum, (int)1);  //may throw
			total_transition_num = SafeOperators::AddThrow(total_transition_num, iNum);  //may throw
		}
		//allocate
		uint uBytes, uBytes1, uBytes2;
		uBytes1 = SafeOperators::MultiplyThrow((uint)iMaxStateNo + 2, (uint)sizeof(FSA_STATE_ITEM));  //may throw
		uBytes2 = SafeOperators::MultiplyThrow((uint)iMaxMatchNo + 2, (uint)sizeof(FSA_MATCH_ITEM));  //may throw
		uBytes  = SafeOperators::AddThrow(uBytes1, uBytes2);  //may throw
		uBytes1 = SafeOperators::MultiplyThrow((uint)total_transition_num, (uint)sizeof(FSA_TRANSITION_ITEM));  //may throw
		uBytes  = SafeOperators::AddThrow(uBytes, uBytes1);  //may throw
		uint uStart = m_allocator.Deref().Allocate(uBytes);
		if( uStart == 0 )
			throw OutOfMemoryException();
		//pointers
		FSA_STATE_ITEM* pState = (FSA_STATE_ITEM*)(m_allocator.Deref().ToPointer(uStart));
		pState[0].iDefaultState = 0;
		pState[0].pTransition = NULL;
		pState[0].iMatchIndex = 0;
		pState[iMaxStateNo + 1].iDefaultState = 0;
		pState[iMaxStateNo + 1].pTransition = NULL;
		pState[iMaxStateNo + 1].iMatchIndex = 0;
		FSA_MATCH_ITEM* pMatch = (FSA_MATCH_ITEM*)((byte*)pState + sizeof(FSA_STATE_ITEM) * (iMaxStateNo + 2));
		pMatch[0].iMatch = 0;
		pMatch[iMaxMatchNo + 1].iMatch = 0;
		FSA_TRANSITION_ITEM* pItem = (FSA_TRANSITION_ITEM*)((byte*)pMatch + sizeof(FSA_MATCH_ITEM) * (iMaxMatchNo + 2));
		for( int i = 1; i <= iMaxStateNo; i ++ ) {
			pState[i].pTransition = pItem;
			pItem += (arrTransitionNum[i].get_Value());
			pItem->uEventFirstNo = FSA_LAST_EVENT_NO;
			pItem->uEventLastNo  = FSA_LAST_EVENT_NO;
			pItem->iNextState    = 0;
			pItem ++;
		}
		//parameters
		_Init_Fsa_Table(m_table, iMaxStateNo, pState, iMaxMatchNo, pMatch);
	}

	//set state
	void SetState(int iState, int iDefaultState, int iMatchIndex) throw()
	{
		assert( iState > 0 && iState <= m_table.iMaxStateNo );
		assert( !_Is_Fsa_Table_Null(m_table) );
		FSA_STATE_ITEM* pItem = const_cast<FSA_STATE_ITEM*>(m_table.pState);
		pItem[iState].iDefaultState = iDefaultState;
		pItem[iState].iMatchIndex = iMatchIndex;
	}
	//set match
	void SetMatch(int iMatchIndex, int iMatch) throw()
	{
		assert( iMatchIndex > 0 && iMatchIndex <= m_table.iMaxMatchNo );
		assert( !_Is_Fsa_Table_Null(m_table) );
		FSA_MATCH_ITEM* pItem = const_cast<FSA_MATCH_ITEM*>(m_table.pMatch);
		pItem[iMatchIndex].iMatch = iMatch;
	}
	//set transition
	void SetTransition(int iState, int iIndex, uint uEventFirstNo, uint uEventLastNo, int iNextState) throw()
	{
		assert( iState > 0 && iState <= m_table.iMaxStateNo );
		assert( !_Is_Fsa_Table_Null(m_table) );
		FSA_TRANSITION_ITEM* pItem = const_cast<FSA_TRANSITION_ITEM*>(m_table.pState[iState].pTransition);
		assert( pItem != NULL );
		pItem[iIndex].uEventFirstNo = uEventFirstNo;
		pItem[iIndex].uEventLastNo  = uEventLastNo;
		pItem[iIndex].iNextState    = iNextState;
	}

	const FSA_TABLE& GetTable() const throw()
	{
		return m_table;
	}

private:
	RefPtr<IMemoryAllocatorRef32>  m_allocator;
	//tables
	FSA_TABLE  m_table;

private:
	//noncopyable
	FsaTable(const FsaTable&) throw();
	FsaTable& operator=(const FsaTable&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
