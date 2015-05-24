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

#pragma pack(push, 1)

// LEXER_CHAR_INFO
typedef struct _tagLexerCharInfo
{
	uint uRow;
	uint uCol;
	uint uCharIndex;
} LEXER_CHAR_INFO;

// LEXER_TOKEN_INFO
typedef struct _tagLexerTokenInfo
{
	ConstStringA strBuffer;  //token string
	LEXER_CHAR_INFO startInfo;
	LEXER_CHAR_INFO endInfo;
	uint uID;  //token id
} LEXER_TOKEN_INFO;

#pragma pack(pop)

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
	//return : 0 -- invalid, > 0 -- token ID
	//  info : startInfo and endInfo can be revised
	virtual uint DoAction(IN int iMatch, INOUT IoHandle& hd, INOUT LEXER_TOKEN_INFO& info) throw() = 0;
};

// LexerParser

class LexerParser
{
private:
	HashMap<ConstStringA, RefPtr<ILexerAction>, ConstStringHashTrait<ConstStringA>, ConstStringCompareTrait<ConstStringA>>  mapClass;

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
	void SetAction(const ConstStringA& strAction, const RefPtr<ILexerAction>& pAction) throw()
	{
	}
	virtual void SetIoHandle(INOUT IoHandle& hd) throw() = 0;
	// called repeatedly.
	// return : CR_S_FALSE, the end of stream, uID will be TK_ERROR or TK_NULL
	virtual CallResult Parse(OUT LEXER_TOKEN_INFO& info) throw() = 0;

private:
	RefPtr<LexerTable>  m_table;
	mapClass  m_actionMap;  //name --- action

private:
	//noncopyable
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__PARSER_H__
////////////////////////////////////////////////////////////////////////////////
