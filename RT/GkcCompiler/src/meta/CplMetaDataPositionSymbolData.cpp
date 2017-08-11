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
This file contains global variables for WLANG position symbol data component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "wlang/action/WlangPositionSymbolData.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WlangPositionSymbolData

BEGIN_COM_TYPECAST(WlangPositionSymbolData)
	COM_TYPECAST_ENTRY(IGrammarSymbolData, _GrammarSymbolDataBase)
	COM_TYPECAST_ENTRY(_ICplMetaDataPositionSymbolDataUtility, _CplMetaDataPositionSymbolDataBase)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
