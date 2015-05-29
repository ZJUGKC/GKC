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
This file contains FSA.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __FSA_H__
#define __FSA_H__
////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// define FSA common states

#define FSA_STATE_START  1
#define FSA_STATE_STOP   0

// event

#define FSA_END_OF_EVENT   ((uint)-1)

//------------------------------------------------------------------------------
// structures

// FSA_TRANSITION_ITEM
typedef struct _tagFSA_TransitionItem {
	uint uEventFirstNo;  //!< The first No. of event. (uint)-1 means the last transition item.
	uint uEventLastNo;   //!< The last No. of event.
	int  iNextState;     //!< The next state. If this value is smaller than 0, it is an invalid state. 0 means the stop state.
} FSA_TRANSITION_ITEM;

// FSA_STATE_ITEM
typedef struct _tagFSA_StateItem {
	int  iDefaultState;   //!< The default next state if no transitions are used.
	const FSA_TRANSITION_ITEM* pTransition;  //!< A pointer to transition array.
	int  iMatchIndex;     //!< Specify the match index. If this value is not larger than 0, its absolute value is the match index (the current state is a stop state). If this value is larger than 0, the current state is not a stop state.
} FSA_STATE_ITEM;

// FSA_MATCH_ITEM
typedef struct _tagFSA_MatchItem {
	int iMatch;  //!< The match No. 0 means no match. If this value is less than zero, the last event should be unput.
} FSA_MATCH_ITEM;

// FSA_TABLE
typedef struct _tagFSA_Table {
	int iMaxStateNo;      //!< The maximum state No. It must not less than 1.
	int iMaxMatchNo;      //!< The maximum match No. It must be not less than zero.
	const FSA_STATE_ITEM* pState;  //!< A pointer to state array with the size iMaxStateNo + 1.
	const FSA_MATCH_ITEM* pMatch;  //!< A pointer to match array with the size iMaxMatchNo + 1.
} FSA_TABLE;

//------------------------------------------------------------------------------
// for constant definition

// These macros are used for class of state machine traits

// BEGIN_FSA_TRAITS_STATE_MAP(_class)
#define BEGIN_FSA_TRAITS_STATE_MAP(_class) \
	typedef _class _TraitsGetStateMap; \
	BEGIN_NOINLINE  \
	static const FSA_STATE_ITEM* GetStateTable(int& iMaxStateNo) throw() \
	END_NOINLINE \
	{

// BEGIN_STATE_TRANSITION(state_name)
//   state_name : Specify the state name or state enumerator.
#define BEGIN_STATE_TRANSITION(state_name) \
	static const FSA_TRANSITION_ITEM _ti##state_name[] = {

// STATE_TRANSITION_ENTRY(event_no, state_next)
//   event_no : Specify the event No.
//   state_next : Specify the next state.
#define STATE_TRANSITION_ENTRY(event_no, state_next) \
	{event_no, event_no, state_next},


// STATE_TRANSITION_RANGE_ENTRY(event_first, event_last, state_next)
//   event_first : Specify the first event No.
//   event_last : Specify the last event No.
//   state_next : Specify the next state.
#define STATE_TRANSITION_RANGE_ENTRY(event_first, event_last, state_next) \
	{event_first, event_last, state_next},

// END_STATE_TRANSITION()
#define END_STATE_TRANSITION() \
	{FSA_END_OF_EVENT, FSA_END_OF_EVENT, 0} };

// BEGIN_STATE_SET()
#define BEGIN_STATE_SET() \
	static const FSA_STATE_ITEM map[] = { \
	{ 0, NULL, 0 },

// STATE_SET_ENTRY(default_state, state_name, match_index)
//   default_state : The default state.
//   state_name : Specify the state name or state enumerator.
//   match_index : Specify the match index.
// The first entry has the index whose value is FSA_STATE_START. This map has at least one entry.
#define STATE_SET_ENTRY(default_state, state_name, match_index) \
	{default_state, _ti##state_name, match_index},

// END_STATE_SET()
#define END_STATE_SET() \
	{0, NULL, 0} };

// END_FSA_TRAITS_STATE_MAP()
#define END_FSA_TRAITS_STATE_MAP() \
	iMaxStateNo = (int)(sizeof(map) / sizeof(FSA_STATE_ITEM)) - 2; return map; \
	}

// BEGIN_FSA_TRAITS_MATCH_MAP(_class)
#define BEGIN_FSA_TRAITS_MATCH_MAP(_class) \
	typedef _class _TraitsGetMatchMap; \
	BEGIN_NOINLINE  \
	static const FSA_MATCH_ITEM* GetMatchTable(int& iMaxMatchNo) throw() \
	END_NOINLINE  \
	{ \
		static const FSA_MATCH_ITEM map[] = {

// STATE_MATCH_ENTRY(match)
//   match : Specify the match No. 0 means no match. If this value is less than zero, the last event should be unput. This map has at least one entry.
#define STATE_MATCH_ENTRY(match) \
	{match},

// END_FSA_TRAITS_MATCH_MAP()
#define END_FSA_TRAITS_MATCH_MAP() \
	{ 0 } }; iMaxMatchNo = (int)(sizeof(map) / sizeof(FSA_MATCH_ITEM)) - 2; return map; \
	}

//------------------------------------------------------------------------------
// classes

// DefaultFsaTraits

class DefaultFsaTraits
{
public:
	// state map
	BEGIN_FSA_TRAITS_STATE_MAP(DefaultFsaTraits)
		//transitions
		BEGIN_STATE_TRANSITION(FSA_STATE_START)
			STATE_TRANSITION_ENTRY(0, FSA_STATE_STOP)
		END_STATE_TRANSITION()
		//state
		BEGIN_STATE_SET()
			STATE_SET_ENTRY(FSA_STATE_STOP, FSA_STATE_START, FSA_STATE_STOP)
		END_STATE_SET()
	END_FSA_TRAITS_STATE_MAP()

	// match map
	BEGIN_FSA_TRAITS_MATCH_MAP(DefaultFsaTraits)
		STATE_MATCH_ENTRY(0)
	END_FSA_TRAITS_MATCH_MAP()
};

// FiniteStateAutomata

class FiniteStateAutomata
{
public:
	FiniteStateAutomata() throw() : m_iCurrentState(FSA_STATE_START), m_iPrevState(FSA_STATE_START), m_iLastStopState(1), m_uBackEventNum(0), m_iMatch(0), m_uLastEventNo(0)
	{
		//init
		m_table.pState = NULL;
		m_table.iMaxStateNo = 1;
		m_table.pMatch = NULL;
		m_table.iMaxMatchNo = 1;
	}
	//set parameters
	// iMaxStateNo : Specify the maximum state No.
	// pState : A pointer to state array.
	// iMaxMatchNo : Specify the maximum match No.
	// pMatch : A pointer to match array.
	void SetParameters(int iMaxStateNo, const FSA_STATE_ITEM* pState,
						int iMaxMatchNo, const FSA_MATCH_ITEM* pMatch) throw()
	{
		assert( iMaxStateNo >= 1 && iMaxMatchNo >= 0 && pState != NULL && pMatch != NULL );
		m_table.iMaxStateNo = iMaxStateNo;
		m_table.pState      = pState;
		m_table.iMaxMatchNo = iMaxMatchNo;
		m_table.pMatch      = pMatch;
	}
	//set start state
	void SetStartState() throw()
	{
		m_iCurrentState  = FSA_STATE_START;
		m_iPrevState     = FSA_STATE_START;
		m_iLastStopState = 1;
		m_uBackEventNum  = 0;
		m_iMatch         = 0;
		m_uLastEventNo   = 0;
	}
	//back to the previous state
	// User should not call this method repeatedly.
	void BackState() throw()
	{
		m_iCurrentState  = m_iPrevState;
		m_iLastStopState = 1;
		m_uBackEventNum  = 0;
		m_iMatch         = 0;
		m_uLastEventNo   = 0;
	}
	//Process state according to current event
	// uEventNo : The input event No.
	void ProcessState(uint uEventNo) throw()
	{
		assert( m_iCurrentState > 0 && m_iCurrentState <= m_table.iMaxStateNo );
		m_iPrevState = m_iCurrentState;
		m_uLastEventNo = uEventNo;
		//look table
		const FSA_TRANSITION_ITEM* pItem = m_table.pState[m_iCurrentState].pTransition;
		if( pItem == NULL ) {
			m_iCurrentState = m_table.pState[m_iCurrentState].iDefaultState;
			_post_process_current_state(uEventNo);
			return ;
		}
		while( pItem->uEventFirstNo != FSA_END_OF_EVENT ) {
			if( uEventNo >= pItem->uEventFirstNo && uEventNo <= pItem->uEventLastNo ) {
				m_iCurrentState = pItem->iNextState;
				_post_process_current_state(uEventNo);
				return ;
			}
			pItem ++;
		} //end while
		m_iCurrentState = m_table.pState[m_iCurrentState].iDefaultState;
		_post_process_current_state(uEventNo);
	}
	//Get current state
	int GetCurrentState() const throw()
	{
		return m_iCurrentState;
	}
	//Check if the current state is stopped
	bool IsStopped() const throw()
	{
		return m_iCurrentState <= 0;
	}
	//Get last stop state
	int GetLastStopState() const throw()
	{
		return m_iLastStopState;
	}
	//Get match No. of current stopped state
	//  uBackEventNum [out] Receive the the number of events needing to be backed.
	//  return: The match No. It is not less than zero. 0 means no match (error).
	int GetMatch(uintptr& uBackEventNum) const throw()
	{
		assert( m_iCurrentState <= 0 );
		uBackEventNum = m_uBackEventNum;
		return m_iMatch;
	}
	//check EOE (End Of Event)
	bool CheckLastEvent_EndOfEvent() const throw()
	{
		assert( IsStopped() );
		return m_uLastEventNo == FSA_END_OF_EVENT;
	}
	//can restart (check error of last token)
	bool CanRestart() const throw()
	{
		assert( IsStopped() );
		return (m_uLastEventNo != FSA_END_OF_EVENT)
				|| (m_iLastStopState <= 0 && m_uBackEventNum != 0);
	}

protected:
	int  m_iCurrentState;         //!< The current state.
	int  m_iPrevState;            //!< The previous state.
	FSA_TABLE m_table;            //!< A state machine table.
	int  m_iLastStopState;        //!< The last stop state.
	uintptr m_uBackEventNum;      //!< The number of events should be backed.
	int  m_iMatch;                //!< The match No. 0 means no match (error). It must be not less than 0.
	uint m_uLastEventNo;

private:
	//get match
	int get_match(int iMatchIndex, bool& bUnput) const throw()
	{
		assert( iMatchIndex <= 0 );
		bUnput = true;
		if( -iMatchIndex > m_table.iMaxMatchNo ) {
			//mismatch
			bUnput = false;
			return 0;
		}
		int iMatch = m_table.pMatch[-iMatchIndex].iMatch;
		//0 : no match(ERROR), discard the current input event.
		if( iMatch >= 0 )
			bUnput = false;
		else
			iMatch = -iMatch;
		return iMatch;
	}
	//post process
	void _post_process_current_state(uint uEventNo) throw()
	{
		if( uEventNo != FSA_END_OF_EVENT )
			++ m_uBackEventNum;
		if( m_iCurrentState > 0 ) {
			assert( uEventNo != FSA_END_OF_EVENT );
			int iMatchIndex = m_table.pState[m_iCurrentState].iMatchIndex;
			if( iMatchIndex <= 0 ) {
				//matched stop state
				bool bUnput;
				m_iMatch = get_match(iMatchIndex, bUnput);
				//stop state
				m_iLastStopState = iMatchIndex;
				m_uBackEventNum  = bUnput ? 1 : 0;
			}
		}
		else {
			//mismatch
			if( m_iLastStopState > 0 ) {
				//invalid
				m_iMatch = 0;
				m_uBackEventNum = 0;
			}
			//otherwise, stop
		} //end if
	}

private:
	//noncopyable
};

// FiniteStateMachineT<TTraits>
//   TTraits A class for state machine traits.
// In class TTraits, user must define enumerations of states (from 2 to iMaxStateNo),
//    stop states (0 or the match index of states must be <= 0), events (from 0 to iMaxEventNo) and matches (from 1 to iMaxMatchNo).
//    The state map and match map should also be defined.

template <class TTraits = DefaultFsaTraits>
class FiniteStateMachineT : public FiniteStateAutomata
{
public:
	FiniteStateMachineT() throw()
	{
		//init
		RestoreParameters();
	}
	void RestoreParameters() throw()
	{
		m_table.pState = TTraits::GetStateTable(m_table.iMaxStateNo);
		m_table.pMatch = TTraits::GetMatchTable(m_table.iMaxMatchNo);
		assert( m_table.iMaxStateNo >= 1 );
		assert( m_table.iMaxMatchNo >= 0 );
		assert( m_table.pState != NULL );
		assert( m_table.pMatch != NULL );
	}

private:
	//noncopyable
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
