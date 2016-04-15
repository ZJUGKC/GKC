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

#define FSA_STATE_START  (1)
#define FSA_STATE_STOP   (0)

// event

#define FSA_END_OF_EVENT    ((uint)-1)
#define FSA_LAST_EVENT_NO   ((uint)-2)

//------------------------------------------------------------------------------
// structures

#pragma pack(push, 1)

// FSA_TRANSITION_ITEM
typedef struct _tagFSA_TransitionItem {
	uint uEventFirstNo;  //!< The first No. of event. (uint)-2 means the last transition item.
	uint uEventLastNo;   //!< The last No. of event.
	int  iNextState;     //!< The next state. If this value is smaller than 0, it is an invalid state. 0 means the stop state.
} FSA_TRANSITION_ITEM;

// FSA_STATE_ITEM
typedef struct _tagFSA_StateItem {
	const FSA_TRANSITION_ITEM* pTransition;  //!< A pointer to transition array.
	int  iDefaultState;   //!< The default next state if no transitions are used.
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

#pragma pack(pop)

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
// classes

// DefaultFsaTraits

class DefaultFsaTraits
{
public:
	// state map
	FSA_BEGIN_TRAITS_STATE_MAP(DefaultFsaTraits)
		//transitions
		FSA_BEGIN_STATE_TRANSITION(FSA_STATE_START)
			FSA_STATE_TRANSITION_ENTRY(0, FSA_STATE_STOP)
		FSA_END_STATE_TRANSITION()
		//state
		FSA_BEGIN_STATE_SET()
			FSA_STATE_SET_ENTRY(FSA_STATE_STOP, FSA_STATE_START, FSA_STATE_STOP)
		FSA_END_STATE_SET()
	FSA_END_TRAITS_STATE_MAP()

	// match map
	FSA_BEGIN_TRAITS_MATCH_MAP(DefaultFsaTraits)
		FSA_STATE_MATCH_ENTRY(0)
	FSA_END_TRAITS_MATCH_MAP()
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
	~FiniteStateAutomata() throw()
	{
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
	//**special method**
	void InitializeAsStopState() throw()
	{
		m_iCurrentState  = 0;
		m_iPrevState     = 0;
		m_iLastStopState = 0;
		m_uBackEventNum  = 1;
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
		assert( m_table.pState != NULL && m_table.pMatch != NULL );
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
		while( pItem->uEventFirstNo != FSA_LAST_EVENT_NO ) {
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
	//can restart
	//  bErrorOrEnd: true for error of last token, false for end of input
	bool CanRestart(bool& bErrorOrEnd) const throw()
	{
		assert( IsStopped() );
		bErrorOrEnd = true;
		bool bRet = true;
		if( m_uLastEventNo == FSA_END_OF_EVENT ) {
			//EOE
			if( m_iLastStopState <= 0 ) {
				//stop state
				if( m_uBackEventNum == 0 ) {
					bErrorOrEnd = false;
					bRet = false;
				}
			}
			else {
				//error
				assert( m_uBackEventNum == 0 );
				bRet = false;
			}
		}
		else {
			if( m_iLastStopState <= 0 ) {
				//stop state
				assert( m_uBackEventNum > 0 );
			}
			else {
				//error
				assert( m_uBackEventNum == 0 );
				bRet = false;
			}
		} //end if
		return bRet;
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
