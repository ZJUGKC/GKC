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

// _LexerTokenString

class _LexerTokenString
{
public:
	//types
	enum {
		None = 0,
		Char8 = sizeof(GKC::CharA), Char16 = sizeof(GKC::CharH), Char32 = sizeof(GKC::CharL),
		MaxType
	};

public:
	_LexerTokenString() throw() : m_iType(None)
	{
		assert( sizeof(GKC::StringA) == sizeof(GKC::StringH)
			&& sizeof(GKC::StringA) == sizeof(GKC::StringL) );
	}
	_LexerTokenString(const _LexerTokenString& src) throw() : m_iType(None)
	{
		operator=(src);
	}
	_LexerTokenString(_LexerTokenString&& src) throw() : m_iType(None)
	{
		operator=(rv_forward(src));
	}
	~_LexerTokenString() throw()
	{
		destroy();
	}

	//assignment
	_LexerTokenString& operator=(const _LexerTokenString& src) throw()
	{
		SetType(src.m_iType);
		if( m_iType == Char8 )
			get_string<GKC::StringA>() = src.get_string<GKC::StringA>();
		else if( m_iType == Char16 )
			get_string<GKC::StringH>() = src.get_string<GKC::StringH>();
		else if( m_iType == Char32 )
			get_string<GKC::StringL>() = src.get_string<GKC::StringL>();
		return *this;
	}
	_LexerTokenString& operator=(_LexerTokenString&& src) throw()
	{
		SetType(src.m_iType);
		if( m_iType == Char8 )
			get_string<GKC::StringA>() = rv_forward(src.get_string<GKC::StringA>());
		else if( m_iType == Char16 )
			get_string<GKC::StringH>() = rv_forward(src.get_string<GKC::StringH>());
		else if( m_iType == Char32 )
			get_string<GKC::StringL>() = rv_forward(src.get_string<GKC::StringL>());
		return *this;
	}

	//methods

	// iType : it can be sizeof(CharX)
	void SetType(int iType) throw()
	{
		if( m_iType != iType ) {
			destroy();
			construct(iType);
			m_iType = iType;
		}
	}
	int GetType() const throw()
	{
		return m_iType;
	}

	void Reset()
	{
		if( m_iType == Char8 ) {
			GKC::StringA& str = get_string<GKC::StringA>();
			if( str.IsBlockNull() )
				str = GKC::StringHelper::MakeEmptyString<GKC::CharA>(GKC::MemoryHelper::GetCrtMemoryManager());  //may throw
		}
		else if( m_iType == Char16 ) {
			GKC::StringH& str = get_string<GKC::StringH>();
			if( str.IsBlockNull() )
				str = GKC::StringHelper::MakeEmptyString<GKC::CharH>(GKC::MemoryHelper::GetCrtMemoryManager());  //may throw
		}
		else if( m_iType == Char32 ) {
			GKC::StringL& str = get_string<GKC::StringL>();
			if( str.IsBlockNull() )
				str = GKC::StringHelper::MakeEmptyString<GKC::CharL>(GKC::MemoryHelper::GetCrtMemoryManager());  //may throw
		}
		SetLength(0);  //may throw
	}

	uintptr GetLength() const throw()
	{
		uintptr uLength = 0;
		if( m_iType == Char8 )
			uLength = get_string<GKC::StringA>().GetLength();
		else if( m_iType == Char16 )
			uLength = get_string<GKC::StringH>().GetLength();
		else if( m_iType == Char32 )
			uLength = get_string<GKC::StringL>().GetLength();
		return uLength;
	}
	void SetLength(uintptr uLength)
	{
		if( m_iType == Char8 )
			get_string<GKC::StringA>().SetLength(uLength);  //may throw
		else if( m_iType == Char16 )
			get_string<GKC::StringH>().SetLength(uLength);  //may throw
		else if( m_iType == Char32 )
			get_string<GKC::StringL>().SetLength(uLength);  //may throw
	}
	void GetAt(uintptr uIndex, GKC::CharF& ch) const throw()
	{
		ch = 0;
		if( m_iType == Char8 )
			ch = (GKC::CharF)(get_string<GKC::StringA>().GetAt(uIndex).get_Value());
		else if( m_iType == Char16 )
			ch = (GKC::CharF)(get_string<GKC::StringH>().GetAt(uIndex).get_Value());
		else if( m_iType == Char32 )
			ch = (GKC::CharF)(get_string<GKC::StringL>().GetAt(uIndex).get_Value());
		else
			assert( false );
	}
	void SetAt(uintptr uIndex, const GKC::CharF& ch) throw()
	{
		if( m_iType == Char8 )
			get_string<GKC::StringA>().GetAt(uIndex).set_Value((GKC::CharA)ch);
		else if( m_iType == Char16 )
			get_string<GKC::StringH>().GetAt(uIndex).set_Value((GKC::CharH)ch);
		else if( m_iType == Char32 )
			get_string<GKC::StringL>().GetAt(uIndex).set_Value((GKC::CharL)ch);
		else
			assert( false );
	}
	uintptr GetAddress() const throw()
	{
		uintptr uAddress = 0;
		if( m_iType == Char8 )
			uAddress = (uintptr)GKC::ShareArrayHelper::GetInternalPointer(get_string<GKC::StringA>());
		else if( m_iType == Char16 )
			uAddress = (uintptr)GKC::ShareArrayHelper::GetInternalPointer(get_string<GKC::StringH>());
		else if( m_iType == Char32 )
			uAddress = (uintptr)GKC::ShareArrayHelper::GetInternalPointer(get_string<GKC::StringL>());
		return uAddress;
	}

	void Delete(uintptr uStart, uintptr uLength) throw()
	{
		if( m_iType == Char8 )
			GKC::StringHelper::Delete(uStart, uLength, get_string<GKC::StringA>());
		else if( m_iType == Char16 )
			GKC::StringHelper::Delete(uStart, uLength, get_string<GKC::StringH>());
		else if( m_iType == Char32 )
			GKC::StringHelper::Delete(uStart, uLength, get_string<GKC::StringL>());
		else
			assert( false );
	}
	void Append(const GKC::CharF& uChar)
	{
		if( m_iType == Char8 )
			GKC::StringHelper::Append((GKC::CharA)uChar, get_string<GKC::StringA>());  //may throw
		else if( m_iType == Char16 )
			GKC::StringHelper::Append((GKC::CharH)uChar, get_string<GKC::StringH>());  //may throw
		else if( m_iType == Char32 )
			GKC::StringHelper::Append((GKC::CharL)uChar, get_string<GKC::StringL>());  //may throw
	}
	void Insert(uintptr uStart, const _LexerTokenString& strAdd)
	{
		if( m_iType == Char8 )
			GKC::StringUtilHelper::Insert(uStart, strAdd.get_string<GKC::StringA>(), get_string<GKC::StringA>());  //may throw
		else if( m_iType == Char16 )
			GKC::StringUtilHelper::Insert(uStart, strAdd.get_string<GKC::StringH>(), get_string<GKC::StringH>());  //may throw
		else if( m_iType == Char32 )
			GKC::StringUtilHelper::Insert(uStart, strAdd.get_string<GKC::StringL>(), get_string<GKC::StringL>());  //may throw
		else
			assert( false );
	}

	void CloneTo(_LexerTokenString& str) const
	{
		str.SetType(GetType());
		if( m_iType == Char8 )
			str.get_string<GKC::StringA>() = GKC::StringHelper::Clone(get_string<GKC::StringA>());  //may throw
		else if( m_iType == Char16 )
			str.get_string<GKC::StringH>() = GKC::StringHelper::Clone(get_string<GKC::StringH>());  //may throw
		else if( m_iType == Char32 )
			str.get_string<GKC::StringL>() = GKC::StringHelper::Clone(get_string<GKC::StringL>());  //may throw
	}
	void ToSubString(uintptr uStart, uintptr uLength, _LexerTokenString& str) const
	{
		str.SetType(GetType());
		str.Reset();  //may throw
		if( m_iType == Char8 )
			GKC::StringUtilHelper::Sub(get_string<GKC::StringA>(), uStart, uLength, str.get_string<GKC::StringA>());  //may throw
		else if( m_iType == Char16 )
			GKC::StringUtilHelper::Sub(get_string<GKC::StringH>(), uStart, uLength, str.get_string<GKC::StringH>());  //may throw
		else if( m_iType == Char32 )
			GKC::StringUtilHelper::Sub(get_string<GKC::StringL>(), uStart, uLength, str.get_string<GKC::StringL>());  //may throw
		else
			assert( false );
	}

	//convert
	GKC::StringS ToSystemString() const
	{
		GKC::StringS str;
		if( m_iType == Char8 )
			str = GKC::CS_U2S(get_string<GKC::StringA>()).GetV();  //may throw
		else if( m_iType == Char16 )
			str = GKC::CS_H2S(get_string<GKC::StringH>()).GetV();  //may throw
		else if( m_iType == Char32 )
			str = GKC::CS_L2S(get_string<GKC::StringL>()).GetV();  //may throw
		return str;
	}
	GKC::StringA ToUTF8() const
	{
		GKC::StringA str;
		if( m_iType == Char8 )
			str = get_string<GKC::StringA>();
		else if( m_iType == Char16 )
			str = GKC::CS_H2U(GKC::StringUtilHelper::To_ConstString(get_string<GKC::StringH>())).GetV();  //may throw
		else if( m_iType == Char32 )
			str = GKC::CS_L2U(GKC::StringUtilHelper::To_ConstString(get_string<GKC::StringL>())).GetV();  //may throw
		return str;
	}

	//to number
	bool ToHexadecimalInteger(uintptr uStart, uint& uValue) const throw()
	{
		uValue = 0;
		uintptr uLength = GetLength();
		for( uintptr i = uStart; i < uLength; i ++ ) {
			GKC::CharF ch;
			GetAt(i, ch);
			uint v;
			if( ch >= '0' && ch <= '9' )
				v = (uint)ch - '0';
			else if( ch >= 'a' && ch <= 'f' )
				v = (uint)ch - 'a' + 10;
			else if( ch >= 'A' && ch <= 'F' )
				v = (uint)ch - 'A' + 10;
			else
				return false;
			uValue = (uValue << 4) + v;
		}
		return true;
	}

private:
	void destroy() throw()
	{
		if( m_iType == Char8 )
			_SObjSoloHelper::object_destruction<GKC::StringA>(&get_string<GKC::StringA>());
		else if( m_iType == Char16 )
			_SObjSoloHelper::object_destruction<GKC::StringH>(&get_string<GKC::StringH>());
		else if( m_iType == Char32 )
			_SObjSoloHelper::object_destruction<GKC::StringL>(&get_string<GKC::StringL>());
		m_iType = None;
	}
	void construct(int iType) throw()
	{
		assert( iType >= None && iType < MaxType );
		if( iType == Char8 )
			call_constructor(get_string<GKC::StringA>());
		else if( iType == Char16 )
			call_constructor(get_string<GKC::StringH>());
		else if( iType == Char32 )
			call_constructor(get_string<GKC::StringL>());
	}

	template <class TString>
	TString& get_string() throw()
	{
		TString* p = (TString*)m_bt;
		return *p;
	}
	template <class TString>
	const TString& get_string() const throw()
	{
		const TString* p = (const TString*)m_bt;
		return *p;
	}

private:
	byte m_bt[sizeof(GKC::StringA)];

	int m_iType;  //character type
};

// _BOMTypeToLexerTokenCharType

inline int _BOMTypeToLexerTokenCharType(int iBomType) throw()
{
	int iCharType = _LexerTokenString::Char8;
	switch( iBomType ) {
	case GKC::BOMTypes::UTF16LE:
	case GKC::BOMTypes::UTF16BE:
		iCharType = _LexerTokenString::Char16;
		break;
	case GKC::BOMTypes::UTF32LE:
	case GKC::BOMTypes::UTF32BE:
		iCharType = _LexerTokenString::Char32;
		break;
	default:
		break;
	}
	return iCharType;
}

// _LexerTokenCharTypeToBOMType

inline int _LexerTokenCharTypeToBOMType(int iCharType) throw()
{
	int iBomType = GKC::BOMTypes::None;
	switch( iCharType ) {
	case _LexerTokenString::Char8:
		iBomType = GKC::BOMTypes::UTF8;
		break;
	case _LexerTokenString::Char16:
		iBomType = GKC::ByteOrderHelper::IsBigEndianHost() ? GKC::BOMTypes::UTF16BE : GKC::BOMTypes::UTF16LE;
		break;
	case _LexerTokenString::Char32:
		iBomType = GKC::ByteOrderHelper::IsBigEndianHost() ? GKC::BOMTypes::UTF32BE : GKC::BOMTypes::UTF32LE;
		break;
	default:
		break;
	}
	return iBomType;
}

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

	void SetCharType(int iType) throw()
	{
		m_strBuffer.SetType(iType);
		m_strData.SetType(iType);
	}

	//reset for parsing new token
	void Reset()
	{
		m_strBuffer.Reset();  //may throw
		m_strData.Reset();  //may throw
		m_eBuffer.SetLength(0);
		m_wordInfo.infoStart = m_wordInfo.infoEnd;
		m_uID = CPL_TK_NULL;
	}
	//append a character
	void Append(const GKC::CharF& ch)
	{
		m_strBuffer.Append(ch);  //may throw
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
	_LexerTokenString& get_Buffer() throw()
	{
		return m_strBuffer;
	}
	_LexerTokenString& get_Data() throw()
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
	_LexerTokenString m_strBuffer;  //token string
	_LexerTokenString m_strData;    //additional data
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
	virtual GKC::ShareCom<_ILexerTables> GetTables() throw() = 0;
	virtual GKC::CallResult SetTables(const GKC::ShareCom<_ILexerTables>& sp) throw() = 0;
	virtual void SetStream(const GKC::ShareCom<GKC::ITextStream>& sp, int& iCharType) throw() = 0;
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

class NOVTABLE _IGrammarSymbolData
{
public:
	virtual GKC::RefPtr<_LexerTokenString> get_Buffer() throw() = 0;
	virtual void set_Buffer(const _LexerTokenString& str) throw() = 0;
	virtual GKC::RefPtr<_LexerTokenString> get_Data() throw() = 0;
	virtual void set_Data(const _LexerTokenString& str) throw() = 0;
	virtual GKC::RefPtr<_LEXER_WORD_INFO> get_WordInfo() throw() = 0;
	virtual void set_WordInfo(const _LEXER_WORD_INFO& info) throw() = 0;
};

DECLARE_GUID(GUID__IGrammarSymbolData)

// _GrammarSymbolDataBase

class NOVTABLE _GrammarSymbolDataBase : public _IGrammarSymbolData
{
public:
	_GrammarSymbolDataBase() throw()
	{
	}
	~_GrammarSymbolDataBase() throw()
	{
	}

// _IGrammarSymbolData methods
	virtual GKC::RefPtr<_LexerTokenString> get_Buffer() throw()
	{
		return GKC::RefPtr<_LexerTokenString>(m_strBuffer);
	}
	virtual void set_Buffer(const _LexerTokenString& str) throw()
	{
		m_strBuffer = str;
	}
	virtual GKC::RefPtr<_LexerTokenString> get_Data() throw()
	{
		return GKC::RefPtr<_LexerTokenString>(m_strData);
	}
	virtual void set_Data(const _LexerTokenString& str) throw()
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
	_LexerTokenString m_strBuffer;  //original string
	_LexerTokenString m_strData;    //additional string
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

// _IGrammarAccepted

class NOVTABLE _IGrammarAccepted
{
public:
	virtual GKC::CallResult DoAccepted(INOUT GKC::ShareCom<_IGrammarSymbolData>& sym) throw() = 0;
};

DECLARE_GUID(GUID__IGrammarAccepted)

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
	virtual GKC::CallResult GenerateTables(const GKC::ShareCom<GKC::ITextStream>& sp, const GKC::ShareCom<_ILexerTables>& spTables) throw() = 0;
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
	virtual void SetAcceptedAction(const GKC::ShareCom<_IGrammarAccepted>& sp) throw() = 0;
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

// helper

// _CplErrorStringHelper

class _CplErrorStringHelper
{
public:
	static void GenerateError(uint uRow, uint uCol, const GKC::ConstStringS& strMark, const GKC::ConstStringS& strAdd, GKC::StringS& str)
	{
		_CplErrorBuffer tmp;
		int ret;
		if( strMark.GetCount() == 0 )
			ret = value_to_string(GKC::FixedArrayHelper::GetInternalPointer(tmp), _CplErrorBuffer::c_size,
								_S("Error : (%u:%u)"),
								GKC::SafeOperators::AddThrow(uRow, (uint)1),
								GKC::SafeOperators::AddThrow(uCol, (uint)1)
								);  //may throw
		else
			ret = value_to_string(GKC::FixedArrayHelper::GetInternalPointer(tmp), _CplErrorBuffer::c_size,
								_S("Error : (%u:%u) [%s]"),
								GKC::SafeOperators::AddThrow(uRow, (uint)1),
								GKC::SafeOperators::AddThrow(uCol, (uint)1),
								GKC::ConstArrayHelper::GetInternalPointer(strMark)
								);  //may throw
		if( ret >= 0 )
			tmp.SetLength(ret);
		GKC::StringUtilHelper::MakeString(tmp, str);  //may throw
		if( strAdd.GetCount() != 0 ) {
			GKC::StringHelper::Append(_S(' '), str);  //may throw
			GKC::StringUtilHelper::Append(strAdd, str);  //may throw
		}
	}
	static void GenerateError(uint uRow, uint uCol, const GKC::ConstStringS& strAdd, GKC::StringS& str)
	{
		GenerateError(uRow, uCol, DECLARE_TEMP_CONST_STRING(GKC::ConstStringS, _S("")), strAdd, str);  //may throw
	}
	static void GenerateError(const GKC::CallResult& cr, GKC::StringS& str)
	{
		_CplErrorBuffer tmp;
		result_to_string(cr, GKC::FixedArrayHelper::GetInternalPointer(tmp), _CplErrorBuffer::c_size);
		tmp.RecalcLength();
		GKC::StringUtilHelper::MakeString(DECLARE_TEMP_CONST_STRING(GKC::ConstStringS, _S("Error : ")), str);  //may throw
		GKC::StringUtilHelper::Append(tmp, str);  //may throw
	}
};

//------------------------------------------------------------------------------
// Meta Data

#pragma pack(push, 1)

// _CplMetaDataPosition

class _CplMetaDataPosition
{
public:
	explicit _CplMetaDataPosition(uint uAddr = 0) throw() : m_uAddr(uAddr)
	{
	}
	_CplMetaDataPosition(const _CplMetaDataPosition& src) throw() : m_uAddr(src.m_uAddr)
	{
	}
	~_CplMetaDataPosition() throw()
	{
	}

	_CplMetaDataPosition& operator=(const _CplMetaDataPosition& src) throw()
	{
		if( this != &src ) {
			m_uAddr = src.m_uAddr;
		}
		return *this;
	}

	bool IsNull() const throw()
	{
		return m_uAddr == 0;
	}

	uint GetAddr() const throw()
	{
		return m_uAddr;
	}
	void SetAddr(uint uAddr) throw()
	{
		m_uAddr = uAddr;
	}

private:
	uint m_uAddr;  //address
};

// _CplMetaDataInfo

struct _CplMetaDataInfo
{
	uint uType;
	uint uLevel;
	_CplMetaDataPosition posData;
};

// _ICplMetaData

class NOVTABLE _ICplMetaData
{
public:
	virtual uint GetCount() throw() = 0;
	virtual _CplMetaDataPosition Find(const GKC::ConstStringA& str) throw() = 0;
	virtual _CplMetaDataPosition FindNext(const _CplMetaDataPosition& pos) throw() = 0;
	virtual _CplMetaDataPosition GetZeroLevelHead() throw() = 0;
	virtual _CplMetaDataPosition GetLevelNext(const _CplMetaDataPosition& pos) throw() = 0;
	virtual void GetInfo(const _CplMetaDataPosition& pos, _CplMetaDataInfo& info, bool& bAnalysis) throw() = 0;
	virtual uintptr GetData(const _CplMetaDataPosition& posData) throw() = 0;
	virtual void SetType(const _CplMetaDataPosition& pos, const uint& uType) throw() = 0;
	virtual void SetLevel(const _CplMetaDataPosition& pos, const uint& uLevel) throw() = 0;
	virtual void SetData(const _CplMetaDataPosition& pos, const _CplMetaDataPosition& posData) throw() = 0;
	virtual void ClearAnalysisFlag(const _CplMetaDataPosition& pos) throw() = 0;
	virtual GKC::CallResult InsertSymbol(const GKC::ConstStringA& str, const uint& uType, const bool& bLevelLink, _CplMetaDataPosition& pos) throw() = 0;
	virtual GKC::CallResult InsertData(const uint& uSize, _CplMetaDataPosition& pos) throw() = 0;
	virtual GKC::CallResult EnterLevel() throw() = 0;
	virtual _CplMetaDataPosition LeaveLevel(const bool& bReverseLevelLink) throw() = 0;
	virtual uint GetCurrentLevel() throw() = 0;
	virtual void FinishZeroLevel(const bool& bReverseLevelLink) throw() = 0;
	virtual GKC::CallResult Load(const GKC::ShareCom<GKC::IByteStream>& sp) throw() = 0;
	virtual GKC::CallResult Save(const GKC::ShareCom<GKC::IByteStream>& sp) throw() = 0;
};

DECLARE_GUID(GUID__ICplMetaData)

// _ICplMetaDataPositionSymbolDataUtility

class NOVTABLE _ICplMetaDataPositionSymbolDataUtility
{
public:
	virtual _CplMetaDataPosition GetPosition() throw() = 0;
	virtual void SetPosition(const _CplMetaDataPosition& pos) throw() = 0;
};

DECLARE_GUID(GUID__ICplMetaDataPositionSymbolDataUtility)

// _CplMetaDataPositionSymbolDataBase

class NOVTABLE _CplMetaDataPositionSymbolDataBase : public _ICplMetaDataPositionSymbolDataUtility
{
public:
	_CplMetaDataPositionSymbolDataBase() throw()
	{
	}
	~_CplMetaDataPositionSymbolDataBase() throw()
	{
	}

// _ICplMetaDataPositionSymbolDataUtility methods
	virtual _CplMetaDataPosition GetPosition() throw()
	{
		return m_pos;
	}
	virtual void SetPosition(const _CplMetaDataPosition& pos) throw()
	{
		m_pos = pos;
	}

private:
	_CplMetaDataPosition m_pos;

private:
	//noncopyable
	_CplMetaDataPositionSymbolDataBase(const _CplMetaDataPositionSymbolDataBase&) throw();
	_CplMetaDataPositionSymbolDataBase& operator=(const _CplMetaDataPositionSymbolDataBase&) throw();
};

// _ICplMetaDataActionUtility

class NOVTABLE _ICplMetaDataActionUtility
{
public:
	virtual void SetMetaData(const GKC::ShareCom<_ICplMetaData>& sp) throw() = 0;
};

DECLARE_GUID(GUID__ICplMetaDataActionUtility)

// _CplMetaDataActionBase

class NOVTABLE _CplMetaDataActionBase : public _ICplMetaDataActionUtility
{
public:
	_CplMetaDataActionBase() throw()
	{
	}
	~_CplMetaDataActionBase() throw()
	{
	}

// _ICplMetaDataActionUtility methods
	virtual void SetMetaData(const GKC::ShareCom<_ICplMetaData>& sp) throw()
	{
		m_spMeta = sp;
	}

protected:
	GKC::ShareCom<_ICplMetaData> m_spMeta;

private:
	//noncopyable
	_CplMetaDataActionBase(const _CplMetaDataActionBase&) throw();
	_CplMetaDataActionBase& operator=(const _CplMetaDataActionBase&) throw();
};

#pragma pack(pop)

//functions

SA_FUNCTION void _CplMetaData_Create(GKC::ShareCom<_ICplMetaData>& sp, GKC::CallResult& cr) throw();

////////////////////////////////////////////////////////////////////////////////
#endif //__SA_GKC_COMPILER_H__
////////////////////////////////////////////////////////////////////////////////
