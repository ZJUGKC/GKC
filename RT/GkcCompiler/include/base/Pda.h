/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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
This file contains PDA.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __PDA_H__
#define __PDA_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// state

#define PDA_STATE_ERROR      (0)
#define PDA_STATE_ACCEPTED   (1)
#define PDA_STATE_START      (2)     //S0

// event

#define PDA_NO_EVENT        (0)
#define PDA_END_OF_EVENT    ((uint)-1)
#define PDA_LAST_EVENT_NO   ((uint)-2)

//------------------------------------------------------------------------------
// structures

#pragma pack(push, 1)

// PDA_TRANSITION_ITEM
typedef struct _tagPDA_TransitionItem {
	uint uEventFirstNo;  //!< The first No. of event. (uint)-2 means the last transition item.
	uint uEventLastNo;   //!< The last No. of event.
	int  iNextState;     //!< The next state. If this value is smaller than 0, it is a reduction operation, the absolute value is production rule index based on 1. 0 means the error state. The positive value means the shift operation and the next state.
} PDA_TRANSITION_ITEM;

// PDA_STATE_ITEM
typedef struct _tagPDA_StateItem {
	const PDA_TRANSITION_ITEM* pTransition;  //!< A pointer to transition array.
	int  iDefaultState;   //!< The default next state if no transitions are used. (PDA_STATE_ERROR)
} PDA_STATE_ITEM;

// PDA_RULE_ITEM
typedef struct _tagPDA_RuleItem {
	uint uLeftEventNo;        //!< The event no. of left part of a production rule.
	uint uRightSymbolNumber;  //!< The number of symbols in right part of a production rule.
} PDA_RULE_ITEM;

// PDA_TABLE
typedef struct _tagPDA_Table {
	const PDA_STATE_ITEM* pState;  //!< A pointer to state array with the size iMaxStateNo + 1.
	const PDA_RULE_ITEM*  pRule;   //!< A pointer to rule array with the size iMaxRuleNo + 1.
	int iMaxStateNo;      //!< The maximum state No. It must not be less than 2.
	int iMaxRuleNo;       //!< The maximum rule No. It must not be less than 0.
} PDA_TABLE;

#pragma pack(pop)

inline void _Init_Pda_Table(PDA_TABLE& table,
							int iMaxStateNo = 2, PDA_STATE_ITEM* pState = NULL,
							int iMaxRuleNo = 0, PDA_RULE_ITEM* pRule = NULL) throw()
{
	table.iMaxStateNo = iMaxStateNo;
	table.iMaxRuleNo  = iMaxRuleNo;
	table.pState = pState;
	table.pRule  = pRule;
}
inline void _Copy_Pda_Table(const PDA_TABLE& src, PDA_TABLE& dest) throw()
{
	dest.iMaxStateNo = src.iMaxStateNo;
	dest.iMaxRuleNo  = src.iMaxRuleNo;
	dest.pState = src.pState;
	dest.pRule  = src.pRule;
}
inline bool _Is_Pda_Table_Null(const PDA_TABLE& table) throw()
{
	return table.pState == NULL || table.pRule == NULL;
}
inline bool _Is_Pda_Size_Valid(int iMaxStateNo, int iMaxRuleNo) throw()
{
	return iMaxStateNo >= 2 && iMaxStateNo < Limits<int>::Max - 1
		&& iMaxRuleNo >= 0 && iMaxRuleNo < Limits<int>::Max - 1;  //with last NULL item
}
inline bool _Is_Pda_Table_Valid(const PDA_TABLE& table) throw()
{
	return !_Is_Pda_Table_Null(table) && _Is_Pda_Size_Valid(table.iMaxStateNo, table.iMaxRuleNo);
}

//------------------------------------------------------------------------------
// classes

// PushDownAutomata

class PushDownAutomata
{
private:
	struct _SymItem
	{
	public:
		uint m_uEvent;  //input event
		int  m_iState;  //state after the event
	};

public:
	PushDownAutomata() throw() : m_uCurrentEvent(PDA_NO_EVENT), m_iCurrentActState(PDA_STATE_START), m_stack(MemoryHelper::GetCrtMemoryManager())
	{
		_Init_Pda_Table(m_table);
	}
	~PushDownAutomata() throw()
	{
	}

	//set table
	void SetTable(const PDA_TABLE& table) throw()
	{
		assert( _Is_Pda_Table_Valid(table) );
		_Copy_Pda_Table(table, m_table);
	}

	//start
	void SetStartState()
	{
		m_stack.RemoveAll();
		m_uCurrentEvent = PDA_NO_EVENT;
		m_iCurrentActState = PDA_STATE_START;
		//push
		auto iter(m_stack.AddTail());  //may throw
		//S0
		_SymItem& item = iter.get_Value();
		item.m_uEvent = PDA_NO_EVENT;
		item.m_iState = PDA_STATE_START;
	}
	//check event
	bool IsNoEvent() const throw()
	{
		return m_uCurrentEvent == PDA_NO_EVENT;
	}
	// the event No. can be PDA_END_OF_EVENT
	void InputEvent(uint uEvent) throw()
	{
		assert( uEvent != PDA_NO_EVENT );
		m_uCurrentEvent = uEvent;
	}
	void ClearEvent() throw()
	{
		m_uCurrentEvent = PDA_NO_EVENT;
	}
	void ProcessEvent() throw()
	{
		//stack top
		process_event(get_stack_top().m_iState, m_uCurrentEvent);
	}
	uint GetCurrentEvent() const throw()
	{
		return m_uCurrentEvent;
	}

	//check states
	bool IsError() const throw()
	{
		return m_iCurrentActState == PDA_STATE_ERROR;
	}
	bool IsAccepted() const throw()
	{
		return m_iCurrentActState == PDA_STATE_ACCEPTED;
	}
	// iRule based 1
	bool IsReduction(int& iRule) const throw()
	{
		if( m_iCurrentActState < 0 ) {
			iRule = -m_iCurrentActState;
			return true;
		}
		return false;
	}

	// check only a start state S0 in stack
	bool CheckStackStart() const throw()
	{
		return m_stack.GetCount() == 1 && m_stack.GetHead().get_Value().m_iState == PDA_STATE_START;
	}

//actions

	// return : event no.
	uint Shift()
	{
		assert( m_iCurrentActState >= PDA_STATE_START );
		//push
		auto iter(m_stack.AddTail());  //may throw
		//state
		_SymItem& item = iter.get_Value();
		item.m_uEvent = m_uCurrentEvent;
		item.m_iState = m_iCurrentActState;
		//clear
		m_uCurrentEvent = PDA_NO_EVENT;
		return item.m_uEvent;
	}

	// return : the number of symbols in right part of a production rule
	uint BeginReduce(int iRule, uint& uLeftEventNo)
	{
		assert( !_Is_Pda_Table_Null(m_table) );
		assert( iRule > 0 );
		//rule
		uLeftEventNo = m_table.pRule[iRule].uLeftEventNo;
		uint uRightSymbolNumber = m_table.pRule[iRule].uRightSymbolNumber;
		assert( uRightSymbolNumber > 0 );
		//push event
		auto iter(m_stack.AddTail());  //may throw
		//left event
		_SymItem& item = iter.get_Value();
		item.m_uEvent = uLeftEventNo;
		return uRightSymbolNumber;
	}
	void EndReduce(uint uRightSymbolNumber) throw()
	{
		assert( m_stack.GetCount() > (uintptr)(uRightSymbolNumber + 1) );
		//remove
		auto iter(m_stack.GetTail());
		iter.MovePrev();
		while( uRightSymbolNumber > 0 ) {
			auto iter1(iter);
			iter.MovePrev();
			m_stack.RemoveAt(iter1);
			uRightSymbolNumber --;
		}
		//new state
		process_event(iter.get_Value().m_iState, get_stack_top().m_uEvent);
		assert( m_iCurrentActState >= PDA_STATE_START );  //it must not be an error
		get_stack_top().m_iState = m_iCurrentActState;
	}

	//error restore
	//  return : true --- success, false --- should input a new event and call RevertAgain repeatedly until returning true
	bool Revert() throw()
	{
		return revert_with_current_event(false);
	}
	//input the next event and try this method
	//  do not input PDA_END_OF_EVENT
	bool RevertAgain() throw()
	{
		assert( m_uCurrentEvent != PDA_END_OF_EVENT );
		return revert_with_current_event(true);
	}

private:
	//process event
	void process_event(int iCurrentState, uint uEventNo) throw()
	{
		assert( !_Is_Pda_Table_Null(m_table) );
		assert( iCurrentState >= PDA_STATE_START );
		//look table
		const PDA_TRANSITION_ITEM* pItem = m_table.pState[iCurrentState].pTransition;
		if( pItem == NULL ) {
			m_iCurrentActState = m_table.pState[iCurrentState].iDefaultState;
			return ;
		}
		while( pItem->uEventFirstNo != PDA_LAST_EVENT_NO ) {
			if( uEventNo >= pItem->uEventFirstNo && uEventNo <= pItem->uEventLastNo ) {
				m_iCurrentActState = pItem->iNextState;
				return ;
			}
			pItem ++;
		} //end while
		m_iCurrentActState = m_table.pState[iCurrentState].iDefaultState;
	}
	//stack
	_SymItem& get_stack_top() throw()
	{
		assert( m_stack.GetCount() > 0 );
		return m_stack.GetTail().get_Value();
	}
	//revert
	bool revert_with_current_event(bool bFromTop) throw()
	{
		assert( m_uCurrentEvent != PDA_NO_EVENT );
		//from top to bottom
		auto iter(m_stack.GetTail());
		if( !bFromTop )
			iter.MovePrev();
		while( iter != m_stack.GetEnd() ) {
			//try
			process_event(iter.get_Value().m_iState, m_uCurrentEvent);
			if( !IsError() ) {
				//remove
				iter.MoveNext();
				while( iter != m_stack.GetEnd() ) {
					auto iter1(iter);
					iter.MoveNext();
					m_stack.RemoveAt(iter1);
				}
				return true;
			}
			iter.MovePrev();
		} //end while
		//clear
		m_uCurrentEvent = PDA_NO_EVENT;
		return false;
	}

protected:
	PDA_TABLE m_table;
	uint m_uCurrentEvent;     //current input event
	int  m_iCurrentActState;  //current action & state
	List<_SymItem> m_stack;   //symbol stack

private:
	//noncopyable
	PushDownAutomata(const PushDownAutomata&) throw();
	PushDownAutomata& operator=(const PushDownAutomata&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
