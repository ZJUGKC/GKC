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
This file contains GkcCompiler helper classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_COMPILER_H__
#define __GKC_COMPILER_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_SYS_H__
	#error GkcCompiler.h requires GkcSys.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////

#include "_GkcCompiler.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

// LEXER_CHAR_INFO
typedef _LEXER_CHAR_INFO  LEXER_CHAR_INFO;

// LEXER_WORD_INFO
typedef _LEXER_WORD_INFO  LEXER_WORD_INFO;

// LexerTokenInfo
typedef _LexerTokenInfo  LexerTokenInfo;

// ILexerAction
typedef _ILexerAction  ILexerAction;

#define GUID_ILexerAction  GUID__ILexerAction

// LexerActionHelper

class LexerActionHelper
{
public:
	// comment start action
	static CallResult CreateCommentStartAction(ShareCom<ILexerAction>& sp) throw()
	{
		CallResult cr;
		::_CommentStartAction_Create(sp, cr);
		return cr;
	}
	// line comment start action
	static CallResult CreateLineCommentStartAction(ShareCom<ILexerAction>& sp) throw()
	{
		CallResult cr;
		::_LineCommentStartAction_Create(sp, cr);
		return cr;
	}
	// space action
	static CallResult CreateSpaceAction(ShareCom<ILexerAction>& sp) throw()
	{
		CallResult cr;
		::_SpaceAction_Create(sp, cr);
		return cr;
	}
	// return action
	static CallResult CreateReturnAction(ShareCom<ILexerAction>& sp) throw()
	{
		CallResult cr;
		::_ReturnAction_Create(sp, cr);
		return cr;
	}
};

// ILexerTables
typedef _ILexerTables  ILexerTables;

#define GUID_ILexerTables  GUID__ILexerTables

// ILexerAnalyzer
typedef _ILexerAnalyzer  ILexerAnalyzer;

#define GUID_ILexerAnalyzer  GUID__ILexerAnalyzer

// IGrammarSymbolData
typedef _IGrammarSymbolData  IGrammarSymbolData;

#define GUID_IGrammarSymbolData  GUID__IGrammarSymbolData

// IGrammarError
typedef _IGrammarError  IGrammarError;

#define GUID_IGrammarError  GUID__IGrammarError

// IGrammarAccepted
typedef _IGrammarAccepted  IGrammarAccepted;

#define GUID_IGrammarAccepted  GUID__IGrammarAccepted

// IGrammarAction
typedef _IGrammarAction  IGrammarAction;

#define GUID_IGrammarAction  GUID__IGrammarAction

// IGrammarTables
typedef _IGrammarTables  IGrammarTables;

#define GUID_IGrammarTables  GUID__IGrammarTables

// IGrammarAnalyzer
typedef _IGrammarAnalyzer  IGrammarAnalyzer;

#define GUID_IGrammarAnalyzer  GUID__IGrammarAnalyzer

// CplAnalyzerHelper

class CplAnalyzerHelper
{
public:
	// lexer tables
	static CallResult CreateLexerTables(ShareCom<ILexerTables>& sp) throw()
	{
		CallResult cr;
		::_LexerTables_Create(sp, cr);
		return cr;
	}
	// lexer analyzer
	static CallResult CreateLexerAnalyzer(ShareCom<ILexerAnalyzer>& sp) throw()
	{
		CallResult cr;
		::_LexerAnalyzer_Create(sp, cr);
		return cr;
	}
	// grammar tables
	static CallResult CreateGrammarTables(ShareCom<IGrammarTables>& sp) throw()
	{
		CallResult cr;
		::_GrammarTables_Create(sp, cr);
		return cr;
	}
	// grammar analyzer
	static CallResult CreateGrammarAnalyzer(ShareCom<IGrammarAnalyzer>& sp) throw()
	{
		CallResult cr;
		::_GrammarAnalyzer_Create(sp, cr);
		return cr;
	}
	// basic symbol data factory
	static CallResult CreateBasicSymbolDataFactory(ShareCom<IComFactory>& sp) throw()
	{
		CallResult cr;
		::_BasicSymbolDataFactory_Create(sp, cr);
		return cr;
	}
};

// ICplMetaData
typedef _ICplMetaData  ICplMetaData;

#define GUID_ICplMetaData  GUID__ICplMetaData

// CplMetaDataHelper

class CplMetaDataHelper
{
public:
	//meta data
	static CallResult CreateCplMetaData(ShareCom<ICplMetaData>& sp) throw()
	{
		CallResult cr;
		::_CplMetaData_Create(sp, cr);
		return cr;
	}
};

// CplLevelStack

class CplLevelStack
{
public:
	CplLevelStack() throw()
	{
	}
	~CplLevelStack() throw()
	{
	}

// methods
	uint& Add()
	{
		if( m_arr.IsBlockNull() )
			m_arr = ShareArrayHelper::MakeShareArray<uint>(MemoryHelper::GetCrtMemoryManager());  //may throw
		return m_arr.Add().get_Value();  //may throw
	}
	void Pop() throw()
	{
		m_arr.RemoveAt(m_arr.GetCount() - 1);
	}

	uint GetTop() const throw()
	{
		return m_arr[m_arr.GetCount() - 1].get_Value();
	}
	bool IsEmpty() const throw()
	{
		return m_arr.GetCount() == 0;
	}

private:
	ShareArray<uint> m_arr;

private:
	//noncopyable
	CplLevelStack(const CplLevelStack&) throw();
	CplLevelStack& operator=(const CplLevelStack&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_COMPILER_H__
////////////////////////////////////////////////////////////////////////////////
