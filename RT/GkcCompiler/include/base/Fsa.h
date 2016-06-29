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
	int iMaxStateNo;      //!< The maximum state No. It must not be less than 1.
	int iMaxMatchNo;      //!< The maximum match No. It must not be less than zero.
	const FSA_STATE_ITEM* pState;  //!< A pointer to state array with the size iMaxStateNo + 1.
	const FSA_MATCH_ITEM* pMatch;  //!< A pointer to match array with the size iMaxMatchNo + 1.
} FSA_TABLE;

#pragma pack(pop)

inline void _Init_Fsa_Table(FSA_TABLE& table,
							int iMaxStateNo = 1, const FSA_STATE_ITEM* pState = NULL,
							int iMaxMatchNo = 0, const FSA_MATCH_ITEM* pMatch = NULL) throw()
{
	table.iMaxStateNo = iMaxStateNo;
	table.iMaxMatchNo = iMaxMatchNo;
	table.pState = pState;
	table.pMatch = pMatch;
}
inline void _Copy_Fsa_Table(const FSA_TABLE& src, FSA_TABLE& dest) throw()
{
	dest.iMaxStateNo = src.iMaxStateNo;
	dest.iMaxMatchNo = src.iMaxMatchNo;
	dest.pState = src.pState;
	dest.pMatch = src.pMatch;
}
inline bool _Is_Fsa_Table_Null(const FSA_TABLE& table) throw()
{
	return table.pState == NULL || table.pMatch == NULL;
}
inline bool _Is_Fsa_Size_Valid(int iMaxStateNo, int iMaxMatchNo) throw()
{
	return iMaxStateNo >= 1 && iMaxStateNo < Limits<int>::Max - 1
		&& iMaxMatchNo >= 0 && iMaxMatchNo < Limits<int>::Max - 1;  //with last NULL item
}
inline bool _Is_Fsa_Table_Valid(const FSA_TABLE& table) throw()
{
	return !_Is_Fsa_Table_Null(table) && _Is_Fsa_Size_Valid(table.iMaxStateNo, table.iMaxMatchNo);
}

//------------------------------------------------------------------------------
// classes

// FiniteStateAutomata

class FiniteStateAutomata
{
public:
	FiniteStateAutomata() throw() : m_iCurrentState(FSA_STATE_START), m_iPrevState(FSA_STATE_START), m_iLastStopState(1), m_uBackEventNum(0), m_iMatch(0), m_uLastEventNo(0)
	{
		//init
		_Init_Fsa_Table(m_table);
	}
	~FiniteStateAutomata() throw()
	{
	}

	//set table
	void SetTable(const FSA_TABLE& table) throw()
	{
		assert( _Is_Fsa_Table_Valid(table) );
		_Copy_Fsa_Table(table, m_table);
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
		assert( !_Is_Fsa_Table_Null(m_table) );
		assert( m_iCurrentState > 0 && m_iCurrentState <= m_table.iMaxStateNo );
		m_iPrevState = m_iCurrentState;
		m_uLastEventNo = uEventNo;
		//look table
		const FSA_TRANSITION_ITEM* pItem = m_table.pState[m_iCurrentState].pTransition;
		if( pItem == NULL ) {
			m_iCurrentState = m_table.pState[m_iCurrentState].iDefaultState;
			post_process_current_state(uEventNo);
			return ;
		}
		while( pItem->uEventFirstNo != FSA_LAST_EVENT_NO ) {
			if( uEventNo >= pItem->uEventFirstNo && uEventNo <= pItem->uEventLastNo ) {
				m_iCurrentState = pItem->iNextState;
				post_process_current_state(uEventNo);
				return ;
			}
			pItem ++;
		} //end while
		m_iCurrentState = m_table.pState[m_iCurrentState].iDefaultState;
		post_process_current_state(uEventNo);
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
	int GetMatch(uint& uBackEventNum) const throw()
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
	FSA_TABLE m_table;            //!< A state machine table.
	int  m_iCurrentState;         //!< The current state.
	int  m_iPrevState;            //!< The previous state.
	int  m_iLastStopState;        //!< The last stop state.
	uint m_uBackEventNum;         //!< The number of events should be backed.
	int  m_iMatch;                //!< The match No. 0 means no match (error). It must not be less than 0.
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
	void post_process_current_state(uint uEventNo) throw()
	{
		if( uEventNo != FSA_END_OF_EVENT ) {
			CallResult cr = SafeOperators::Add(m_uBackEventNum, (uint)1, m_uBackEventNum);
			if( cr.IsFailed() ) {
				//overflow
				m_iCurrentState  = 0;
				m_iLastStopState = 1;
			}
		}
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
	FiniteStateAutomata(const FiniteStateAutomata&) throw();
	FiniteStateAutomata& operator=(const FiniteStateAutomata&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
