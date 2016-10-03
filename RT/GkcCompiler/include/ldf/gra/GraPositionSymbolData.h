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
This file contains component class of grammar position symbol data.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GRA_POSITION_SYMBOL_DATA_H__
#define __GRA_POSITION_SYMBOL_DATA_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GraPositionSymbolData

class GraPositionSymbolData : public _GrammarSymbolDataBase,
							public _I_GraPositionSymbolData_Utility
{
public:
	GraPositionSymbolData() throw()
	{
	}
	~GraPositionSymbolData() throw()
	{
	}

// _I_GraPositionSymbolData_Utility methods
	virtual uintptr GetPosition() throw()
	{
		return m_uPos;
	}
	virtual void SetPosition(uintptr uPos) throw()
	{
		m_uPos = uPos;
	}

private:
	uintptr m_uPos;

private:
	//noncopyable
	GraPositionSymbolData(const GraPositionSymbolData&) throw();
	GraPositionSymbolData& operator=(const GraPositionSymbolData&) throw();
};

DECLARE_COM_TYPECAST(GraPositionSymbolData)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
