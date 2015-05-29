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

////////////////////////////////////////////////////////////////////////////////
#ifndef __PARSER_H__
#define __PARSER_H__
////////////////////////////////////////////////////////////////////////////////

#include "fsa.h"
#include "pool.h"
#include "CharStream.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//token
#define TK_ERROR   ((uint)-2)     //error
#define TK_EOF     ((uint)-1)     //EOF
#define TK_NULL    0              //noop or start token
#define TK_FIRST   1              //the first user token

// TokenTable

class TokenTable
{
public:
	TokenTable() throw() : m_symbol_pool(m_string_pool), m_uLevelHead(0)
	{
	}
	~TokenTable() throw()
	{
	}

	void InsertToken(const ConstStringA& strToken, uint uID)
	{
		assert( uID >= TK_FIRST );
		assert( m_symbol_pool.Find(strToken).IsNull() );
		Iterator iter(m_symbol_pool.CreateNode(strToken, sizeof(uint), 0, 0, m_uLevelHead));  //may throw
		iter.GetData<uint>() = _cpl_process_byte_order(uID);
		m_symbol_pool.SetZeroLevelHead(m_uLevelHead);
	}

	//properties
	uint GetTokenID(const ConstStringA& strToken) const throw()
	{
		Iterator iter(m_symbol_pool.Find(strToken));
		assert( !iter.IsNull() );
		return _cpl_process_byte_order(iter.GetData<uint>());
	}

private:
	SymbolPool m_symbol_pool;
	StringPool m_string_pool;
	uint m_uLevelHead;  //not used in this class

private:
	//noncopyable
};

//------------------------------------------------------------------------------
// Lexer

#pragma pack(push, 1)

// LEXER_CHAR_INFO
typedef struct _LexerCharInfo
{
	uint uRow;
	uint uCol;
	uint uCharIndex;
} LEXER_CHAR_INFO;

#pragma pack(pop)

// LexerTokenInfo

class LexerTokenInfo
{
public:
	LexerTokenInfo() throw()
	{
	}
	~LexerTokenInfo() throw()
	{
	}

	void Init()
	{
		m_startInfo.uRow = m_startInfo.uCol = m_startInfo.uCharIndex = 0;
		m_endInfo.uRow = m_endInfo.uCol = m_endInfo.uCharIndex = 0;
		if( SharedArrayHelper::GetBlockPointer(m_strBuffer) == NULL )
			m_strBuffer = StringUtilHelper::MakeEmptyString(MemoryHelper::GetCrtMemoryManager());  //may throw
	}
	//reset for parsing new token
	void Reset() throw()
	{
		m_strBuffer.Clear();
		m_info.startInfo = m_info.endInfo;
		m_uID = TK_NULL;
	}
	//append a character
	void Append(const Tchar& ch)
	{
		StringUtilHelper::Append(ch, m_strBuffer);  //may throw
		//coordinates
		m_info.endInfo.uCol ++;
		m_info.endInfo.uCharIndex ++;
	}
	//back characters
	void BackChar(uint uBackNum)
	{
		uintptr uCount = m_strBuffer.GetLength();
		assert( (uintptr)uBackNum <= uCount );
		m_strBuffer.SetLength(uCount - (uintptr)uBackNum);  //may throw
		//only in current line
		assert( m_info.endInfo.uCol >= uBackNum );
		m_info.endInfo.uCol -= uBackNum;
	}

	//properties
	StringA& GetBuffer() throw()
	{
		return m_strBuffer;
	}
	uint GetID() const throw()
	{
		return m_uID;
	}
	void SetID(uint uID) throw()
	{
		m_uID = uID;
	}
	LEXER_CHAR_INFO& GetCharStart() throw()
	{
		return m_startInfo;
	}
	LEXER_CHAR_INFO& GetCharEnd() throw()
	{
		return m_endInfo;
	}

private:
	StringA m_strBuffer;  //token string
	LEXER_CHAR_INFO m_startInfo;
	LEXER_CHAR_INFO m_endInfo;
	uint m_uID;  //token id

private:
	//noncopyable
};

// LexerTable

class LexerTable
{
public:
	LexerTable(const RefPtr<FiniteStateAutomata>& fsa, const RefPtr<TokenTable>& tk) throw() : m_fsa(fsa), m_token_table(tk)
	{
	}
	~LexerTable() throw()
	{
	}

	//properties
	FiniteStateAutomata& GetFSA() throw()
	{
		return m_fsa.Deref();
	}
	const TokenTable& GetTokenTable() const throw()
	{
		return m_token_table.Deref();
	}

private:
	RefPtr<FiniteStateAutomata>  m_fsa;
	RefPtr<TokenTable>           m_token_table;

private:
	//noncopyable
};

// ILexerAction
class NOVTABLE ILexerAction
{
public:
	// info : all fields can be revised
	virtual void DoAction(INOUT CharStream& stream, INOUT LexerTokenInfo& info) = 0;
};

// LexerParser

class LexerParser
{
public:
	LexerParser() throw()
	{
	}
	~LexerParser() throw()
	{
	}

	//settings
	void SetLexerTable(const RefPtr<LexerTable>& table) throw()
	{
		m_table = table;
	}
	void SetAction(const ConstStringA& strToken, const RefPtr<ILexerAction>& pAction)
	{
		if( SharedArrayHelper::GetBlockPointer(m_arrAction) == NULL )
			m_arrAction = SharedArrayHelper::MakeSharedArray<RefPtr<ILexerAction>>(MemoryHelper::GetCrtMemoryManager());  //may throw
		//find id
		uint uID = m_table.Deref().GetTokenTable().GetTokenID(strToken);
		assert( uID > 0 );
		//fill
		if( m_arrAction.GetCount() < (uintptr)(uID + 1) )
			m_arrAction.SetCount(uID + 1);  //may throw
		m_arrAction.SetAt(uID, pAction);
	}
	void SetStream(INOUT const RefPtr<CharStream>& stream) throw()
	{
		m_stream = stream;
	}

	//start parsing
	void Start(OUT LexerTokenInfo& info)
	{
		m_table.GetFSA().SetStartState();
		info.Init();
	}

	// called repeatedly.
	// return : Failed
	//          SystemCallResults::S_False, the end of stream.
	//          OK, the token id may be TK_ERROR.
	CallResult Parse(INOUT LexerTokenInfo& info)
	{
		CallResult cr;

		//last event is EOE
		if( m_table.GetFSA().CheckLastEvent_EndOfEvent() ) {
			cr.SetResult(SystemCallResults::S_False);
			return cr;
		}
		//can restart (last token is error)
		if( !m_table.GetFSA().CanRestart() ) {
			cr.SetResult(SystemCallResults::Fail);
			return cr;
		}

		//start state
		m_table.GetFSA().SetStartState();
		info.Reset();
		//loop
		do {
			uint uEvent;
			//get char
			byte ch;
			cr = m_stream.Deref().GetChar(ch);
			if( cr.IsFailed() )
				return cr;
			if( cr.GetResult() == SystemCallResults::S_EOF ) {
				uEvent = FSA_END_OF_EVENT;
				cr.SetResult(SystemCallResults::OK);
			}
			else {
				uEvent = ch;
				info.Append(ch);  //may throw
			}
			//state
			m_table.GetFSA().ProcessState(uEvent);
			//stopped
			if( m_table.GetFSA().IsStopped() ) {
				//match
				uintptr uBackNum;
				int iMatch = m_table.GetFSA().GetMatch(uBackNum);
				assert( iMatch >= 0 );
				if( iMatch == 0 ) {
					//error
					info.SetID(TK_ERROR);
					break;
				}
				//iMatch is the same as token id (from TK_FIRST)
				info.SetID(iMatch);
				//back
				assert( uBackNum <= (uintptr)Limits<uint>::Max && uBackNum > 0 );
				cr = m_stream.Deref().UngetChar((uint)uBackNum);
				assert( cr.IsOK() );
				info.BackChar((uint)uBackNum);  //may throw
				//action
				if( !m_arrAction.IsNull() && !m_arrAction[info.GetID()].get_Value().IsNull() ) {
					m_arrAction[info.GetID()].get_Value().Deref().DoAction(m_stream.Deref(), info);  //may throw
				}
				break;
			} //end if
		} while( true );

		return cr;
	}

private:
	//settings
	RefPtr<LexerTable>  m_table;
	SharedArray<RefPtr<ILexerAction>>  m_arrAction;
	RefPtr<CharStream>  m_stream;

private:
	//noncopyable
};

//------------------------------------------------------------------------------
// Grammar


////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__PARSER_H__
////////////////////////////////////////////////////////////////////////////////
