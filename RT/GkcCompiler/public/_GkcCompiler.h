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

typedef struct _tagLexerCharInfo
{
	uint uRow;
	uint uCol;
	uint uCharIndex;
} _LEXER_CHAR_INFO;

// _LEXER_WORD_INFO

typedef struct _tagLexerWordInfo
{
	_LEXER_CHAR_INFO infoStart;
	_LEXER_CHAR_INFO infoEnd;
} _LEXER_WORD_INFO;

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
		m_wordInfo.infoStart.uRow = m_wordInfo.infoStart.uCol = m_wordInfo.infoStart.uCharIndex = 0;
		m_wordInfo.infoEnd.uRow = m_wordInfo.infoEnd.uCol = m_wordInfo.infoEnd.uCharIndex = 0;
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
		m_wordInfo.infoStart = m_wordInfo.infoEnd;
		m_uID = CPL_TK_NULL;
	}
	//append a character
	void Append(const GKC::CharA& ch)
	{
		GKC::StringHelper::Append(ch, m_strBuffer);  //may throw
		//coordinates
		m_wordInfo.infoEnd.uCol = GKC::SafeOperators::AddThrow(m_wordInfo.infoEnd.uCol, (uint)1);  //may throw
		m_wordInfo.infoEnd.uCharIndex = GKC::SafeOperators::AddThrow(m_wordInfo.infoEnd.uCharIndex, (uint)1);  //may throw
	}
	//back characters
	void BackChar(uint uBackNum)
	{
		uintptr uCount = m_strBuffer.GetLength();
		assert( (uintptr)uBackNum <= uCount );
		m_strBuffer.SetLength(uCount - (uintptr)uBackNum);  //may throw
		//only in current line
		assert( m_wordInfo.infoEnd.uCol >= uBackNum );
		m_wordInfo.infoEnd.uCol -= uBackNum;
		m_wordInfo.infoEnd.uCharIndex -= uBackNum;
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
	_LEXER_WORD_INFO& get_WordInfo() throw()
	{
		return m_wordInfo;
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
		return GKC::SafeOperators::Add(m_wordInfo.infoEnd.uRow, uAdd, m_wordInfo.infoEnd.uRow);
	}
	GKC::CallResult AddCharEndCol(uint uAdd) throw()
	{
		return GKC::SafeOperators::Add(m_wordInfo.infoEnd.uCol, uAdd, m_wordInfo.infoEnd.uCol);
	}
	GKC::CallResult AddCharEndCharIndex(uint uAdd) throw()
	{
		return GKC::SafeOperators::Add(m_wordInfo.infoEnd.uCharIndex, uAdd, m_wordInfo.infoEnd.uCharIndex);
	}
	void ResetCharEndCol() throw()
	{
		m_wordInfo.infoEnd.uCol = 0;
	}

private:
	GKC::StringA m_strBuffer;  //token string
	//additional data
	GKC::StringA m_strData;
	//additional error string
	_CplErrorBuffer m_eBuffer;
	//character
	_LEXER_WORD_INFO m_wordInfo;
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
	virtual GKC::CallResult DoAction(GKC::ShareCom<GKC::ITextStream>& stream, _LexerTokenInfo& info, GKC::ConstStringA& strToken, bool& bTokenChanged) throw() = 0;
};

DECLARE_GUID(GUID__ILexerAction)

#pragma pack(pop)

//functions

SA_FUNCTION void _CommentStartAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _LineCommentStartAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _SpaceAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _ReturnAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw();

#pragma pack(push, 1)

// Analyzer

// _ILexerTables

class NOVTABLE _ILexerTables
{
public:
	virtual GKC::CallResult GenerateTables(const GKC::ShareCom<GKC::ITextStream>& sp) throw() = 0;
};

DECLARE_GUID(GUID__ILexerTables)

// _ILexerAnalyzer

class NOVTABLE _ILexerAnalyzer
{
public:
	virtual GKC::CallResult SetTables(const GKC::ShareCom<_ILexerTables>& sp) throw() = 0;
	virtual void SetStream(const GKC::ShareCom<GKC::ITextStream>& sp) throw() = 0;
	virtual GKC::CallResult SetAction(const GKC::ConstStringA& strToken, const GKC::ShareCom<_ILexerAction>& spAction) throw() = 0;
	virtual void Start() throw() = 0;
	// return value : SystemCallResults::OK, the call is successful. The token id may be CPL_TK_ERROR.
	//                SystemCallResults::S_False, it reaches the end of stream.
	//                otherwise, this call is failed.
	virtual GKC::CallResult Parse() throw() = 0;
	virtual GKC::RefPtr<_LexerTokenInfo> GetTokenInfo() throw() = 0;
};

DECLARE_GUID(GUID__ILexerAnalyzer)

#pragma pack(pop)

//functions

SA_FUNCTION void _LexerTables_Create(GKC::ShareCom<_ILexerTables>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _LexerAnalyzer_Create(GKC::ShareCom<_ILexerAnalyzer>& sp, GKC::CallResult& cr) throw();

//------------------------------------------------------------------------------
// Grammar

#pragma pack(push, 1)

// _IGrammarSymbolData

class _IGrammarSymbolData
{
public:
	virtual GKC::StringA get_Buffer() throw() = 0;
	virtual void set_Buffer(const GKC::StringA& str) throw() = 0;
	virtual GKC::StringA get_Data() throw() = 0;
	virtual void set_Data(const GKC::StringA& str) throw() = 0;
	virtual GKC::RefPtr<_LEXER_WORD_INFO> get_WordInfo() throw() = 0;
	virtual void set_WordInfo(const _LEXER_WORD_INFO& info) throw() = 0;
};

DECLARE_GUID(GUID__IGrammarSymbolData)

// _GrammarSymbolDataBase

class _GrammarSymbolDataBase : public _IGrammarSymbolData
{
public:
	_GrammarSymbolDataBase() throw()
	{
	}
	~_GrammarSymbolDataBase() throw()
	{
	}

// _IGrammarSymbolData methods
	virtual GKC::StringA get_Buffer() throw()
	{
		return m_strBuffer;
	}
	virtual void set_Buffer(const GKC::StringA& str) throw()
	{
		m_strBuffer = str;
	}
	virtual GKC::StringA get_Data() throw()
	{
		return m_strData;
	}
	virtual void set_Data(const GKC::StringA& str) throw()
	{
		m_strData = str;
	}
	virtual GKC::RefPtr<_LEXER_WORD_INFO> get_WordInfo() throw()
	{
		return GKC::RefPtr<_LEXER_WORD_INFO>(m_wordInfo);
	}
	virtual void set_WordInfo(const _LEXER_WORD_INFO& info) throw()
	{
		m_wordInfo = info;
	}

protected:
	GKC::StringA m_strBuffer;  //original string
	GKC::StringA m_strData;    //additional string
	_LEXER_WORD_INFO m_wordInfo;

private:
	//noncopyable
	_GrammarSymbolDataBase(const _GrammarSymbolDataBase&) throw();
	_GrammarSymbolDataBase& operator=(const _GrammarSymbolDataBase&) throw();
};

// callback

// _IGrammarError

class NOVTABLE _IGrammarError
{
public:
	virtual GKC::CallResult DoModifyEvent(INOUT GKC::ConstStringA& strEvent, INOUT GKC::ShareCom<GKC::ITextStream>& spText, INOUT bool& bChanged) throw() = 0;
};

DECLARE_GUID(GUID__IGrammarError)

// _IGrammarAction

class NOVTABLE _IGrammarAction
{
public:
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw() = 0;
};

DECLARE_GUID(GUID__IGrammarAction)

// analyzer

// _IGrammarTables

class NOVTABLE _IGrammarTables
{
public:
	virtual GKC::CallResult GenerateTables(const GKC::ShareCom<GKC::ITextStream>& sp) throw() = 0;
};

DECLARE_GUID(GUID__IGrammarTables)

// _IGrammarAnalyzer

class NOVTABLE _IGrammarAnalyzer
{
public:
	virtual GKC::CallResult SetTables(const GKC::ShareCom<_IGrammarTables>& sp) throw() = 0;
	virtual void SetLexerAnalyzer(const GKC::ShareCom<_ILexerAnalyzer>& sp) throw() = 0;
	virtual GKC::CallResult SetAction(const GKC::ConstStringA& strAction, const GKC::ShareCom<_IGrammarAction>& spAction) throw() = 0;
	virtual void SetErrorAction(const GKC::ShareCom<_IGrammarError>& sp) throw() = 0;
	virtual GKC::CallResult SetFactory(const GKC::ConstStringA& strEvent, const GKC::ShareCom<GKC::IComFactory>& sp) throw() = 0;
	virtual GKC::CallResult Start(const bool& bUnexpectedEoeAsError) throw() = 0;
	// return value : SystemCallResults::OK, the call is successful.
	//                SystemCallResults::S_False, it reaches the end of stream.
	//                SystemCallResults::S_EOF, it reaches the end of stream. This value is returned after calling Start() with the parameter value false.
	//                otherwise, this call is failed.
	// The error list may not be empty.
	virtual GKC::CallResult Parse() throw() = 0;
	virtual const GKC::ShareArray<GKC::StringS> get_ErrorArray() throw() = 0;
	// This method can be called after SystemCallResults::S_False returned by Parse().
	virtual bool IsEmpty() throw() = 0;
	// This method can be called after SystemCallResults::OK returned by Parse().
	virtual bool IsErrorState() throw() = 0;
	virtual GKC::CallResult Revert() throw() = 0;
};

DECLARE_GUID(GUID__IGrammarAnalyzer)

#pragma pack(pop)

//functions

SA_FUNCTION void _GrammarTables_Create(GKC::ShareCom<_IGrammarTables>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _GrammarAnalyzer_Create(GKC::ShareCom<_IGrammarAnalyzer>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _BasicSymbolDataFactory_Create(GKC::ShareCom<GKC::IComFactory>& sp, GKC::CallResult& cr) throw();

////////////////////////////////////////////////////////////////////////////////
#endif //__SA_GKC_COMPILER_H__
////////////////////////////////////////////////////////////////////////////////
