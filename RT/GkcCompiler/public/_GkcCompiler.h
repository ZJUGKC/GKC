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
This file contains GkcCompiler functions.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __SA_GKC_COMPILER_H__
#define __SA_GKC_COMPILER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// Lexer

//for error string
#define CPL_MAX_ERROR_LENGTH  (8*1024)

//token definitions
#define CPL_TK_ERROR   ((uint)-2)     //error
#define CPL_TK_EOF     ((uint)-1)     //EOF
#define CPL_TK_NULL    (0)            //noop or start token (S0)
#define CPL_TK_FIRST   (1)            //the first user token

// _CplErrorBuffer
typedef GKC::FixedStringT<GKC::CharS, CPL_MAX_ERROR_LENGTH>  _CplErrorBuffer;

#pragma pack(push, 1)

// _LEXER_CHAR_INFO

typedef struct _LexerCharInfo
{
	uint uRow;
	uint uCol;
	uint uCharIndex;
} _LEXER_CHAR_INFO;

// _LexerTokenInfo

class _LexerTokenInfo
{
public:
	_LexerTokenInfo() throw() : m_uID(CPL_TK_NULL)
	{
		ResetCharInfo();
	}
	~_LexerTokenInfo() throw()
	{
	}

	void ResetCharInfo() throw()
	{
		m_startInfo.uRow = m_startInfo.uCol = m_startInfo.uCharIndex = 0;
		m_endInfo.uRow = m_endInfo.uCol = m_endInfo.uCharIndex = 0;
	}

	//reset for parsing new token
	void Reset()
	{
		if( m_strBuffer.IsBlockNull() )
			m_strBuffer = GKC::StringHelper::MakeEmptyString<GKC::CharA>(GKC::MemoryHelper::GetCrtMemoryManager());  //may throw
		if( m_strData.IsBlockNull() )
			m_strData = GKC::StringHelper::MakeEmptyString<GKC::CharA>(GKC::MemoryHelper::GetCrtMemoryManager());  //may throw
		m_strBuffer.SetLength(0);  //may throw
		m_strData.SetLength(0);    //may throw
		m_eBuffer.SetLength(0);
		m_startInfo = m_endInfo;
		m_uID = CPL_TK_NULL;
	}
	//append a character
	void Append(const byte& ch)
	{
		GKC::StringHelper::Append((GKC::CharA)ch, m_strBuffer);  //may throw
		//coordinates
		m_endInfo.uCol = GKC::SafeOperators::AddThrow(m_endInfo.uCol, (uint)1);  //may throw
		m_endInfo.uCharIndex = GKC::SafeOperators::AddThrow(m_endInfo.uCharIndex, (uint)1);  //may throw
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
	GKC::StringA& get_Buffer() throw()
	{
		return m_strBuffer;
	}
	GKC::StringA& get_Data() throw()
	{
		return m_strData;
	}
	const _CplErrorBuffer& get_ErrorString() const throw()
	{
		return m_eBuffer;
	}
	void set_ErrorString(const GKC::ConstStringS& str) throw()
	{
		GKC::StringUtilHelper::MakeString(str, m_eBuffer);
	}
	_LEXER_CHAR_INFO& get_CharStart() throw()
	{
		return m_startInfo;
	}
	_LEXER_CHAR_INFO& get_CharEnd() throw()
	{
		return m_endInfo;
	}
	uint get_ID() const throw()
	{
		return m_uID;
	}
	void set_ID(uint uID) throw()
	{
		m_uID = uID;
	}

	//methods
	GKC::CallResult AddCharEndRow(uint uAdd) throw()
	{
		return GKC::SafeOperators::Add(m_endInfo.uRow, uAdd, m_endInfo.uRow);
	}
	GKC::CallResult AddCharEndCol(uint uAdd) throw()
	{
		return GKC::SafeOperators::Add(m_endInfo.uCol, uAdd, m_endInfo.uCol);
	}
	GKC::CallResult AddCharEndCharIndex(uint uAdd) throw()
	{
		return GKC::SafeOperators::Add(m_endInfo.uCharIndex, uAdd, m_endInfo.uCharIndex);
	}
	void ResetCharEndCol() throw()
	{
		m_endInfo.uCol = 0;
	}

private:
	GKC::StringA m_strBuffer;  //token string
	//additional data
	GKC::StringA m_strData;
	//additional error string
	_CplErrorBuffer m_eBuffer;
	//character
	_LEXER_CHAR_INFO m_startInfo;
	_LEXER_CHAR_INFO m_endInfo;
	uint m_uID;  //token id

private:
	//noncopyable
	_LexerTokenInfo(const _LexerTokenInfo&) throw();
	_LexerTokenInfo& operator=(const _LexerTokenInfo&) throw();
};

#pragma pack(pop)

#pragma pack(push, 1)

// Callback

// _ILexerAction

class NOVTABLE _ILexerAction
{
public:
	// info : all fields can be revised
	virtual GKC::CallResult DoAction(GKC::ShareCom<GKC::ITextStream>& stream, _LexerTokenInfo& info) throw() = 0;
};

DECLARE_GUID(GUID__ILexerAction)

#pragma pack(pop)

//functions

SA_FUNCTION void _CommentStartAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _SpaceAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _ReturnAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw();

#pragma pack(push, 1)

// Analyzer

// _ILexerTables



// _ILexerAnalyzer

class NOVTABLE _ILexerAnalyzer
{
public:
	virtual void SetStream(const GKC::ShareCom<GKC::ITextStream>& sp) throw() = 0;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////
#endif //__SA_GKC_COMPILER_H__
////////////////////////////////////////////////////////////////////////////////
