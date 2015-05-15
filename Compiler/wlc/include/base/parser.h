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

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//token
#define TK_ERROR   ((uint)-1)     //error
#define TK_NULL    0              //noop

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
	const RefPtr<CharA> szBuffer;  //token string
	uint  uLength;  //string length
	LEXER_CHAR_INFO startInfo;
	LEXER_CHAR_INFO endInfo;
	uint  uID;  //token id
} LEXER_TOKEN_INFO;

#pragma pack(pop)

// ILexerTable
//   include: FSA, token table
class NOVTABLE ILexerTable
{
public:
	//FSA
	virtual void SetFSA(const SharedPtr<FiniteStateAutomata>& fsa) throw() = 0;
	virtual const SharedPtr<FiniteStateAutomata>& GetFSA() const throw() = 0;
	//token table
	virtual uint GetTokenID(IN const RefPtr<Char>& szToken) throw() = 0;
	virtual const RefPtr<CharA> GetTokenName(IN uint uID) throw() = 0;
};

// ILexerAction
class NOVTABLE ILexerAction
{
public:
	//return : 0 -- invalid, > 0 -- token ID
	//  info : startInfo and endInfo can be revised
	virtual uint DoAction(IN int iMatch, INOUT IoHandle& hd, INOUT LEXER_TOKEN_INFO& info) throw() = 0;
};

// ILexerParser
class NOVTABLE ILexerParser
{
public:
	//settings
	virtual void SetLexerTable(IN　const RefPtr<ILexerTable>& pTable) throw() = 0;
	virtual void SetAction(IN const RefPtr<CharA>& szToken, IN const RefPtr<ILexerAction>& pAction) throw() = 0;
	virtual void SetIoHandle(INOUT IoHandle& hd) throw() = 0;
	// called repeatedly.
	// return : S_FALSE, the end of stream, uID will be TK_ERROR or TK_NULL
	virtual CallResult Parse(OUT LEXER_TOKEN_INFO& info) throw() = 0;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__PARSER_H__
////////////////////////////////////////////////////////////////////////////////
