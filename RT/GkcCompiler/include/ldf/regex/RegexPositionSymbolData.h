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
This file contains component class of regular expression position symbol data.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_POSITION_SYMBOL_DATA_H__
#define __REGEX_POSITION_SYMBOL_DATA_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexPositionSymbolData

class RegexPositionSymbolData : public _GrammarSymbolDataBase,
								public _I_RegexPositionSymbolData_Utility
{
public:
	RegexPositionSymbolData() throw()
	{
	}
	~RegexPositionSymbolData() throw()
	{
	}

// _I_RegexPositionSymbolData_Utility methods
	virtual void SetPosition(const GKC::AstTree::Position& pos) throw()
	{
		m_pos = pos;
	}
	virtual GKC::AstTree::Position GetPosition() throw()
	{
		return m_pos;
	}

private:
	AstTree::Position m_pos;

private:
	//noncopyable
	RegexPositionSymbolData(const RegexPositionSymbolData&) throw();
	RegexPositionSymbolData& operator=(const RegexPositionSymbolData&) throw();
};

DECLARE_COM_TYPECAST(RegexPositionSymbolData)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
