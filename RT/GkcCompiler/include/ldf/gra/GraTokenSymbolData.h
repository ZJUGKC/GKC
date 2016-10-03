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
This file contains component class of grammar token symbol data.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GRA_TOKEN_SYMBOL_DATA_H__
#define __GRA_TOKEN_SYMBOL_DATA_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GraTokenSymbolData

class GraTokenSymbolData : public _GrammarSymbolDataBase,
						public _I_GraTokenSymbolData_Utility
{
public:
	GraTokenSymbolData() throw()
	{
	}
	~GraTokenSymbolData() throw()
	{
	}

// _I_GraTokenSymbolData_Utility methods
	virtual void GetToken(_GraToken& tk) throw()
	{
		tk = m_token;
	}
	virtual void SetToken(const _GraToken& tk) throw()
	{
		m_token = tk;
	}

private:
	_GraToken m_token;

private:
	//noncopyable
	GraTokenSymbolData(const GraTokenSymbolData&) throw();
	GraTokenSymbolData& operator=(const GraTokenSymbolData&) throw();
};

DECLARE_COM_TYPECAST(GraTokenSymbolData)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
