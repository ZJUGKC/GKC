﻿/*
** Copyright (c) 2021, Xin YUAN, courses of Zhejiang University
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
This file contains global variables for Do-Value-String-Catenation action component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcParser.h"

#include "won/base/WonDef.h"

#include "won/action/WonDoValueStringCatenationAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WonDoValueStringCatenationAction

BEGIN_COM_TYPECAST(WonDoValueStringCatenationAction)
	COM_TYPECAST_ENTRY(IGrammarAction, IGrammarAction)
	COM_TYPECAST_ENTRY(_IWonActionUtility, _WonActionBase)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
