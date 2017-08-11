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
This file contains global variables for meta data position symbol data component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "meta/CplMetaDataPositionSymbolData.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// CplMetaDataPositionSymbolData

BEGIN_COM_TYPECAST(CplMetaDataPositionSymbolData)
	COM_TYPECAST_ENTRY(_IGrammarSymbolData, _GrammarSymbolDataBase)
	COM_TYPECAST_ENTRY(_ICplMetaDataPositionSymbolDataUtility, _CplMetaDataPositionSymbolDataBase)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
