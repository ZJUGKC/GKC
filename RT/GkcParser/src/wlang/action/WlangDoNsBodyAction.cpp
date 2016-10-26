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
This file contains global variables for Do-Ns-Body action component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "wlang/base/WlangDef.h"

#include "wlang/action/WlangDoNsBodyAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WlangDoNsBodyAction

BEGIN_COM_TYPECAST(WlangDoNsBodyAction)
	COM_TYPECAST_ENTRY(IGrammarAction, IGrammarAction)
	COM_TYPECAST_ENTRY(_ICplMetaDataActionUtility, _CplMetaDataActionBase)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
