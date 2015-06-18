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

//error string
#define GKC_CPL_MAX_ERROR_LENGTH  (8*1024)

// CplErrorBuffer

typedef FixedStringT<CharS, GKC_CPL_MAX_ERROR_LENGTH>  CplErrorBuffer;

//token
#define TK_ERROR   ((uint)-2)     //error
#define TK_EOF     ((uint)-1)     //EOF
#define TK_NULL    (0)            //noop or start token (S0)
#define TK_FIRST   (1)            //the first user token

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
		auto iter(m_symbol_pool.CreateNode(strToken, sizeof(uint), 0, 0, m_uLevelHead));  //may throw
		iter.GetData<uint>() = _cpl_process_byte_order(uID);
		m_symbol_pool.SetZeroLevelHead(m_uLevelHead);
	}

	//properties
	uint GetTokenID(const ConstStringA& strToken) const throw()
	{
		auto iter(m_symbol_pool.Find(strToken));
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
			m_strBuffer = StringUtilHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager());  //may throw
		if( SharedArrayHelper::GetBlockPointer(m_strData) == NULL )
			m_strData = StringUtilHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager());  //may throw
	}
	//reset for parsing new token
	void Reset()
	{
		m_strBuffer.SetLength(0);  //may throw
		m_strData.SetLength(0);    //may throw
		m_eBuffer.SetLength(0);
		m_startInfo = m_endInfo;
		m_uID = TK_NULL;
	}
	//append a character
	void Append(const byte& ch)
	{
		StringUtilHelper::Append((CharA)ch, m_strBuffer);  //may throw
		//coordinates
		m_endInfo.uCol ++;
		m_endInfo.uCharIndex ++;
	}
	//back characters
	void BackChar(uint uBackNum)
	{
		uintptr uCount = m_strBuffer.GetLength();
		assert( (uintptr)uBackNum <= uCount );
		m_strBuffer.SetLength(uCount - (uintptr)uBackNum);  //may throw
		//only in current line
		assert( m_endInfo.uCol >= uBackNum );
		m_endInfo.uCol -= uBackNum;
		m_endInfo.uCharIndex -= uBackNum;
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
	const CplErrorBuffer& GetErrorString() const throw()
	{
		return m_eBuffer;
	}
	void SetErrorString(const ConstStringS& str) throw()
	{
		StringUtilHelper::MakeString(str, m_eBuffer);
	}
	StringA& GetData() throw()
	{
		return m_strData;
	}

private:
	StringA m_strBuffer;  //token string
	LEXER_CHAR_INFO m_startInfo;
	LEXER_CHAR_INFO m_endInfo;
	uint m_uID;  //token id
	//additional error string
	CplErrorBuffer m_eBuffer;
	//additional data
	StringA m_strData;

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
	virtual void DoAction(INOUT RefPtr<ICharStream>& stream, INOUT LexerTokenInfo& info) = 0;
};

// common actions

// CommentStartAction
class CommentStartAction : public ILexerAction
{
public:
	virtual void DoAction(INOUT RefPtr<ICharStream>& stream, INOUT LexerTokenInfo& info)
	{
		CallResult cr;
		byte ch;
		//next char
		cr = _get_next_char(stream, info, ch);
		if( cr.IsFailed() )
			return ;
		do {
			//CR
			if( ch == '\r' ) {
				(info.GetCharEnd().uRow) ++;
				info.GetCharEnd().uCol = 0;
				//next char
				cr = _get_next_char(stream, info, ch);
				if( cr.IsFailed() )
					break;
				if( ch != '\n' ) {
					continue;
				}
			}
			//LN
			else if( ch == '\n' ) {
				(info.GetCharEnd().uRow) ++;
				info.GetCharEnd().uCol = 0;
			}
			//*
			else if( ch == '*' ) {
				(info.GetCharEnd().uCol) ++;
				//next char
				cr = _get_next_char(stream, info, ch);
				if( cr.IsFailed() )
					break;
				if( ch == '/' ) {
					(info.GetCharEnd().uCol) ++;
					info.SetID(TK_NULL);
					break;
				}
				continue;
			}
			else {
				(info.GetCharEnd().uCol) ++;
			} //end if
			//next char
			cr = _get_next_char(stream, info, ch);
			if( cr.IsFailed() )
				break;
		} while( true );
	}

private:
	static CallResult _get_next_char(RefPtr<ICharStream>& stream, LexerTokenInfo& info, byte& ch) throw()
	{
		CallResult cr;
		cr = stream.Deref().GetChar(ch);
		if( cr.GetResult() == SystemCallResults::S_EOF ) {
			cr.SetResult(SystemCallResults::Fail);
		}
		if( cr.IsFailed() ) {
			info.SetID(TK_ERROR);
			info.SetErrorString(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("No expected comment end")));
			return cr;
		}
		(info.GetCharEnd().uCharIndex) ++;
		return cr;
	}
};

// SpaceAction
class SpaceAction : public ILexerAction
{
public:
	virtual void DoAction(INOUT RefPtr<ICharStream>& stream, INOUT LexerTokenInfo& info)
	{
		info.SetID(TK_NULL);
	}
};

// ReturnAction
class ReturnAction : public ILexerAction
{
public:
	virtual void DoAction(INOUT RefPtr<ICharStream>& stream, INOUT LexerTokenInfo& info)
	{
		(info.GetCharEnd().uRow) ++;
		info.GetCharEnd().uCol = 0;
		info.SetID(TK_NULL);
	}
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
			m_arrAction.SetCount(uID + 1, 0);  //may throw
		m_arrAction.SetAt(uID, pAction);
	}
	void SetStream(IN const RefPtr<ICharStream>& stream) throw()
	{
		m_stream = stream;
	}

	//start parsing
	void Start(OUT LexerTokenInfo& info)
	{
		m_table.Deref().GetFSA().InitializeAsStopState();  //special
		info.Init();  //may throw
	}

	// called repeatedly.
	// return : Failed
	//          SystemCallResults::S_False, the end of stream.
	//          OK, the token id may be TK_ERROR.
	CallResult Parse(INOUT LexerTokenInfo& info)
	{
		CallResult cr;

		bool bErrorOrEnd;
		//can restart (last token is error or EOE)
		if( !m_table.Deref().GetFSA().CanRestart(bErrorOrEnd) ) {
			cr.SetResult(bErrorOrEnd ? SystemCallResults::Fail : SystemCallResults::S_False);
			return cr;
		}

		uint uEvent;
		byte ch;
		//read a character
		cr = m_stream.Deref().GetChar(ch);
		if( cr.IsFailed() )
			return cr;
		if( cr.GetResult() == SystemCallResults::S_EOF ) {
			cr.SetResult(SystemCallResults::S_False);
			return cr;
		}

		//start state
		m_table.Deref().GetFSA().SetStartState();
		info.Reset();  //may throw
		//first character
		uEvent = ch;
		info.Append(ch);  //may throw

		//loop
		do {
			//state
			m_table.Deref().GetFSA().ProcessState(uEvent);
			//stopped
			if( m_table.Deref().GetFSA().IsStopped() ) {
				//match
				uintptr uBackNum;
				int iMatch = m_table.Deref().GetFSA().GetMatch(uBackNum);
				assert( iMatch >= 0 );
				if( iMatch == 0 ) {
					//error
					info.SetID(TK_ERROR);
					break;
				}
				//iMatch is the same as token id (from TK_FIRST)
				info.SetID(iMatch);
				//back
				assert( uBackNum <= (uintptr)Limits<uint>::Max );
				if( uBackNum > 0 ) {
					cr = m_stream.Deref().UngetChar((uint)uBackNum);
					assert( cr.IsOK() );
					info.BackChar((uint)uBackNum);  //may throw
				}
				//action
				RefPtr<ILexerAction> action = find_action(info.GetID());
				if( !action.IsNull() ) {
					action.Deref().DoAction(m_stream, info);  //may throw
				}
				break;
			} //end if
			//get next char
			cr = m_stream.Deref().GetChar(ch);
			if( cr.IsFailed() )
				break;
			if( cr.GetResult() == SystemCallResults::S_EOF ) {
				uEvent = FSA_END_OF_EVENT;
				cr.SetResult(SystemCallResults::OK);
			}
			else {
				uEvent = ch;
				info.Append(ch);  //may throw
			}
		} while( true );

		return cr;
	}

private:
	RefPtr<ILexerAction> find_action(uint uID) const throw()
	{
		RefPtr<ILexerAction> ret;
		if( m_arrAction.IsNull() || uID >= m_arrAction.GetCount() )
			return ret;
		ret = m_arrAction[uID].get_Value();
		return ret;
	}

private:
	//settings
	RefPtr<LexerTable>  m_table;
	SharedArray<RefPtr<ILexerAction>>  m_arrAction;
	RefPtr<ICharStream>  m_stream;

private:
	//noncopyable
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__PARSER_H__
////////////////////////////////////////////////////////////////////////////////
