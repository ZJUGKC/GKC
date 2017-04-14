/*
** Copyright (c) 2017, Xin YUAN, courses of Zhejiang University
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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WON_DEF_H__
#define __WON_DEF_H__
////////////////////////////////////////////////////////////////////////////////

// _WonUtility_Objects

struct _WonUtility_Objects
{
	//tables
	GKC::ShareCom<GKC::ILexerTables>   spLexerTables;
	GKC::ShareCom<GKC::IGrammarTables> spGrammarTables;
	//lexer actions
	GKC::ShareCom<GKC::ILexerAction> spCommentStart;
	GKC::ShareCom<GKC::ILexerAction> spSpace;
	GKC::ShareCom<GKC::ILexerAction> spReturn;
	//factory
	GKC::ShareCom<GKC::IComFactory> spBasicFactory;
};

// _IWonUtility_Access

class NOVTABLE _IWonUtility_Access
{
public:
	virtual void GetObjects(_WonUtility_Objects& obj) throw() = 0;
};

DECLARE_GUID(GUID__IWonUtility_Access)

// _WonActionData

class _WonActionData
{
public:
	_WonActionData() throw() : m_iType(_WonTokenTypes::Unknown)
	{
	}
	~_WonActionData() throw()
	{
	}

	int get_Type() const throw()
	{
		return m_iType;
	}
	void set_Type(int iType) throw()
	{
		m_iType = iType;
	}
	GKC::StringA get_String() const throw()
	{
		return m_str;
	}
	void set_String(const GKC::StringA& str) throw()
	{
		m_str = str;
	}

private:
	int m_iType;
	GKC::StringA m_str;

private:
	//noncopyable
	_WonActionData(const _WonActionData&) throw();
	_WonActionData& operator=(const _WonActionData&) throw();
};

// _IWonActionUtility

class NOVTABLE _IWonActionUtility
{
public:
	virtual void SetData(int iType, const GKC::RefPtr<_WonActionData>& data) throw() = 0;
};

DECLARE_GUID(GUID__IWonActionUtility)

// _WonActionBase

class NOVTABLE _WonActionBase : public _IWonActionUtility
{
public:
	_WonActionBase() throw() : m_iTokenType(_WonTokenTypes::Unknown)
	{
	}
	~_WonActionBase() throw()
	{
	}

// _IWonActionUtility methods
	virtual void SetData(int iType, const GKC::RefPtr<_WonActionData>& data) throw()
	{
		m_iTokenType = iType;
		m_data = data;
	}

protected:
	GKC::RefPtr<_WonActionData> m_data;
	int m_iTokenType;

private:
	//noncopyable
	_WonActionBase(const _WonActionBase&) throw();
	_WonActionBase& operator=(const _WonActionBase&) throw();
};

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
