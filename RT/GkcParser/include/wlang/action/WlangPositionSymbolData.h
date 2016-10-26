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
This file contains component class of WLANG position symbol data.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WLANG_POSITION_SYMBOL_DATA_H__
#define __WLANG_POSITION_SYMBOL_DATA_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WlangPositionSymbolData

class WlangPositionSymbolData : public _GrammarSymbolDataBase,
								public _CplMetaDataPositionSymbolDataBase
{
public:
	WlangPositionSymbolData() throw()
	{
	}
	~WlangPositionSymbolData() throw()
	{
	}

private:
	//noncopyable
	WlangPositionSymbolData(const WlangPositionSymbolData&) throw();
	WlangPositionSymbolData& operator=(const WlangPositionSymbolData&) throw();
};

DECLARE_COM_TYPECAST(WlangPositionSymbolData)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
