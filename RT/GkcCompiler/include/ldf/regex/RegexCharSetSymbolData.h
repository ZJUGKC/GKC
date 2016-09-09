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
This file contains component class of regular expression character set symbol data.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_CHAR_SET_SYMBOL_DATA_H__
#define __REGEX_CHAR_SET_SYMBOL_DATA_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexCharSetSymbolData

class RegexCharSetSymbolData : public _GrammarSymbolDataBase,
							public _I_RegexCharSetSymbolData_Utility
{
public:
	RegexCharSetSymbolData() throw()
	{
	}
	~RegexCharSetSymbolData() throw()
	{
	}

// _I_RegexCharSetSymbolData_Utility methods
	virtual GKC::RefPtr<_RegexCharRangeSet> GetCharRangeSet() throw()
	{
		return RefPtr<_RegexCharRangeSet>(m_rs);
	}

private:
	_RegexCharRangeSet m_rs;

private:
	//noncopyable
	RegexCharSetSymbolData(const RegexCharSetSymbolData&) throw();
	RegexCharSetSymbolData& operator=(const RegexCharSetSymbolData&) throw();
};

DECLARE_COM_TYPECAST(RegexCharSetSymbolData)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
