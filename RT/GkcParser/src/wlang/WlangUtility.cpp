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
This file contains global variables for WLang utility component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcParser.h"

#include "base/ParserDef.h"

#include "wlang/base/WlangDef.h"
#include "wlang/base/WlangLex.h"
#include "wlang/base/WlangGra.h"

#include "wlang/action/WlangGrammarError.h"
#include "wlang/action/WlangGrammarAccepted.h"
#include "wlang/action/WlangPositionSymbolData.h"
#include "wlang/action/WlangPositionSymbolData_Factory.h"
#include "wlang/action/WlangDoNsBodyAction.h"
#include "wlang/action/WlangDoBodySemiAction.h"

#include "wlang/WlangAction.h"
#include "wlang/WlangUtility.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WlangUtility

BEGIN_COM_TYPECAST(WlangUtility)
	COM_TYPECAST_ENTRY(_IWlangUtility, _IWlangUtility)
	COM_TYPECAST_ENTRY(_IWlangUtility_Access, _IWlangUtility_Access)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
