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

// PDA_TABLE
typedef struct _tagPDA_Table {
	const PDA_STATE_ITEM* pState;  //!< A pointer to state array with the size iMaxStateNo + 1.
	int iMaxStateNo;      //!< The maximum state No. It must not less than 2.
} PDA_TABLE;

#pragma pack(pop)

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

//------------------------------------------------------------------------------
// classes

// DefaultPdaTraits

class DefaultPdaTraits
{
public:
	// state map
	PDA_BEGIN_TRAITS_STATE_MAP(DefaultPdaTraits)
		//transitions
		PDA_BEGIN_STATE_TRANSITION(PDA_STATE_START)
			PDA_STATE_TRANSITION_ENTRY(1, PDA_STATE_ACCEPTED)
		PDA_END_STATE_TRANSITION()
		//state
		PDA_BEGIN_STATE_SET()
			PDA_STATE_SET_ENTRY(PDA_STATE_START)
		PDA_END_STATE_SET()
	PDA_END_TRAITS_STATE_MAP()
};

// PushDownAutomata<T>

template <typename T>
class PushDownAutomata
{
private:
	struct _SymItem
	{
	public:
		uint m_uEvent;  //input event
		int  m_iState;  //state after the event
		T m_t;  //user data
	};

public:
	PushDownAutomata() throw() : m_uCurrentEvent(PDA_NO_EVENT), m_iCurrentActState(PDA_STATE_START), m_stack(MemeoryHelper::GetCrtMemoryManager()),
								m_uRightSymbolNumber(0)
	{
		m_table.pState = NULL;
		m_table.iMaxStateNo = PDA_STATE_START;
	}
	~PushDownAutomata() throw()
	{
	}

	//set parameters
	void SetParameters(int iMaxStateNo, const PDA_STATE_ITEM* pState) throw()
	{
		assert( iMaxStateNo >= 2 && pState != NULL );
		m_table.iMaxStateNo = iMaxStateNo;
		m_table.pState      = pState;
	}

	//start
	void SetStartState()
	{
		m_stack.RemoveAll();
		m_uCurrentEvent = PDA_NO_EVENT;
		m_iCurrentActState = PDA_STATE_START;
		//push
		auto iter = m_stack.AddTail();  //may throw
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
	void ProcessEvent() throw()
	{
		//stack top
		_SymItem& item = get_stack_top();
		process_event(item.iState, m_uCurrentEvent);
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

	//actions
	T& Shift()
	{
		assert( m_iCurrentActState >= PDA_STATE_START );
		//push
		auto iter = m_stack.AddTail();  //may throw
		//state
		_SymItem& item = iter.get_Value();
		item.m_uEvent = m_uCurrentEvent;
		item.m_iState = m_iCurrentActState;
		//clear
		m_uCurrentEvent = PDA_NO_EVENT;
		return item.m_t;
	}

	// uRightSymbolNumber : the number of symbols in right part of a production rule
	// uLeftEventNo : the event no of left part of a production rule
	void BeginReduce(uint uRightSymbolNumber, uint uLeftEventNo)
	{
		assert( uRightSymbolNumber > 0 );
		//push event
		auto iter = m_stack.AddTail();  //may throw
		//left event
		_SymItem& item = iter.get_Value();
		item.m_uEvent = uLeftEventNo;
		m_uRightSymbolNumber = uRightSymbolNumber;
	}
	// uIndex : 0 --- left symbol, 1, 2, 3, ... --- right symbols
	T& GetReduceSymbolData(uint uIndex) throw()
	{
		if( uIndex == 0 )
			return get_stack_top().m_t;
		return get_stack_right_sym(uIndex).m_t;
	}
	void EndReduce() throw()
	{
		assert( m_stack.GetCount() > (m_uRightSymbolNumber + 1) );
		//remove
		auto iter = m_stack.GetTail();
		iter.MovePrev();
		while( m_uRightSymbolNumber > 0 ) {
			auto iter1(iter);
			iter.MovePrev();
			m_stack.RemoveAt(iter1);
			m_uRightSymbolNumber --;
		}
		//new state
		process_event(iter.get_Value().m_iState, get_stack_top().m_uEvent);
		assert( m_iCurrentActState >= PDA_STATE_START );  //it must not be an error
		get_stack_top().m_iState = m_iCurrentActState;
	}

	//error restore
	//  return : true --- continue, false --- should input a new event and call NextRevert repeatedly until returning true
	bool Revert() throw()
	{
		return revert_with_current_event(false);
	}
	//input the next event and try this method
	//  do not input PDA_END_OF_EVENT
	bool NextRevert() throw()
	{
		assert( m_uCurrentEvent != PDA_END_OF_EVENT );
		return revert_with_current_event(true);
	}

private:
	//process event
	void process_event(int iCurrentState, uint uEventNo) throw()
	{
		assert( m_table.pState != NULL );
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
	_SymItem& get_stack_right_sym(uint uIndex) throw()
	{
		assert( uIndex <= m_uRightSymbolNumber );
		uintptr uCount = m_stack.GetCount();
		assert( uCount > m_uRightSymbolNumber );
		return FindIndex(uCount - m_uRightSymbolNumber - 1 + uIndex - 1).get_Value();
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
	uint m_uCurrentEvent;    //current input event
	int m_iCurrentActState;  //current action & state
	PDA_TABLE m_table;
	List<_SymItem> m_stack;  //symbol stack
	//reduce
	uint m_uRightSymbolNumber;  //the number of symbols in right part of a production rule

private:
	//noncopyable
};

// PushDownMachineT<TTraits>
//   TTraits A class for PDA traits.
// In class TTraits, user must define enumerations of states (from 3 to iMaxStateNo),
//    events (from 1 to iMaxEventNo).
//    The state map should be defined.

template <class TTraits = DefaultPdaTraits>
class PushDownMachineT : public PushDownAutomata
{
public:
	PushDownMachineT() throw()
	{
		//init
		RestoreParameters();
	}
	void RestoreParameters() throw()
	{
		m_table.pState = TTraits::GetStateTable(m_table.iMaxStateNo);
		assert( m_table.iMaxStateNo >= PDA_STATE_START );
		assert( m_table.pState != NULL );
	}

private:
	//noncopyable
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
