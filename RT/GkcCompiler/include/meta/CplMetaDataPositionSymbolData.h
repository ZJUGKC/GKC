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
This file contains component class of meta data position symbol data.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __CPL_META_DATA_POSITION_SYMBOL_DATA_H__
#define __CPL_META_DATA_POSITION_SYMBOL_DATA_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// CplMetaDataPositionSymbolData

class CplMetaDataPositionSymbolData : public _GrammarSymbolDataBase,
									public _CplMetaDataPositionSymbolDataBase
{
public:
	CplMetaDataPositionSymbolData() throw()
	{
	}
	~CplMetaDataPositionSymbolData() throw()
	{
	}

private:
	//noncopyable
	CplMetaDataPositionSymbolData(const CplMetaDataPositionSymbolData&) throw();
	CplMetaDataPositionSymbolData& operator=(const CplMetaDataPositionSymbolData&) throw();
};

DECLARE_COM_TYPECAST(CplMetaDataPositionSymbolData)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
