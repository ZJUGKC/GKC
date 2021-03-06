﻿/*
** Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
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
This file contains global variables for Do-Block-Body-Indent action component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcParser.h"

#include "wmark/action/WmarkDoBlockBodyIndentAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkDoBlockBodyIndentAction

BEGIN_COM_TYPECAST(WmarkDoBlockBodyIndentAction)
	COM_TYPECAST_ENTRY(IGrammarAction, IGrammarAction)
	COM_TYPECAST_ENTRY(_ICplMetaDataActionUtility, _CplMetaDataActionBase)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
