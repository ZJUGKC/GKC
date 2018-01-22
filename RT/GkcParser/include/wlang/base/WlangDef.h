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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WLANG_DEF_H__
#define __WLANG_DEF_H__
////////////////////////////////////////////////////////////////////////////////

// _WlangUtility_Objects

struct _WlangUtility_Objects
{
	//tables
	GKC::ShareCom<GKC::ILexerTables>   spLexerTables;
	GKC::ShareCom<GKC::IGrammarTables> spGrammarTables;
	//lexer actions
	GKC::ShareCom<GKC::ILexerAction> spCommentStart;
	GKC::ShareCom<GKC::ILexerAction> spLineCommentStart;
	GKC::ShareCom<GKC::ILexerAction> spSpace;
	GKC::ShareCom<GKC::ILexerAction> spReturn;
	//factory
	GKC::ShareCom<GKC::IComFactory> spBasicFactory;
	GKC::ShareCom<GKC::IComFactory> spPositionFactory;
	//grammar actions
	GKC::ShareCom<GKC::IGrammarError> spGrammarError;
};

// _IWlangUtility_Access

class NOVTABLE _IWlangUtility_Access
{
public:
	virtual void GetObjects(_WlangUtility_Objects& obj) throw() = 0;
};

DECLARE_GUID(GUID__IWlangUtility_Access)

//specifier

// noun

#define WLANG_NOUN_NAMESPACE    (15)

// adj

#ddefine WLANG_ADJ_CONSTANT
#ddefine WLANG_ADJ_ALIAS

// scope

#define WLANG_SCOPE_PUBLIC
#define WLANG_SCOPE_PRIVATE
#define WLANG_SCOPE_STATIC

// classes

#pragma pack(push, 1)

// _WlangMetaDataBase

class _WlangMetaDataBase
{
public:
	_WlangMetaDataBase() throw()
	{
	}
	~_WlangMetaDataBase() throw()
	{
	}

	//properties
	uint GetStringAddr() const throw()
	{
		return m_uStringAddr.get_Value();
	}
	void SetStringAddr(uint uAddr) throw()
	{
		m_uStringAddr.set_Value(uAddr);
	}
	uint GetRow() const throw()
	{
		return m_uRow.get_Value();
	}
	void SetRow(uint uRow) throw()
	{
		m_uRow.set_Value(uRow);
	}
	uint GetCol() const throw()
	{
		return m_uCol.get_Value();
	}
	void SetCol(uint uCol) throw()
	{
		m_uCol.set_Value(uCol);
	}

protected:
	GKC::BeType<uint> m_uStringAddr;
	GKC::BeType<uint> m_uRow;
	GKC::BeType<uint> m_uCol;

private:
	//noncopyable
	_WlangMetaDataBase(const _WlangMetaDataBase&) throw();
	_WlangMetaDataBase& operator=(const _WlangMetaDataBase&) throw();
};

// _WlangNsClassData

class _WlangNsClassData
{
public:
	_WlangNsClassData() throw()
	{
	}
	~_WlangNsClassData() throw()
	{
	}

	uint GetMembers() const throw()
	{
		return m_uMembers.get_Value();
	}
	void SetMembers(uint uAddr) throw()
	{
		m_uMembers.set_Value(uAddr);
	}

private:
	GKC::BeType<uint> m_uMembers;

private:
	//noncopyable
	_WlangNsClassData(const _WlangNsClassData&) throw();
	_WlangNsClassData& operator=(const _WlangNsClassData&) throw();
};

// _WlangNsClassMetaData

class _WlangNsClassMetaData : public _WlangMetaDataBase,
							public _WlangNsClassData
{
public:
	_WlangNsClassMetaData() throw()
	{
	}
	~_WlangNsClassMetaData() throw()
	{
	}

private:
	//noncopyable
	_WlangNsClassMetaData(const _WlangNsClassMetaData&) throw();
	_WlangNsClassMetaData& operator=(const _WlangNsClassMetaData&) throw();
};

#pragma pack(pop)

//declarator

// kind
//   ARRAY POINTER METHOD

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
