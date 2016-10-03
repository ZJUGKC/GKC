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
This file contains global variables for Do-Right-Id-Block action component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "base/DataColl.h"
#include "base/Fsa.h"
#include "base/Lexer.h"

#include "ldf/gra/GraDef.h"
#include "ldf/gra/GraDoRightIdBlockAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GraDoRightIdBlockAction

BEGIN_COM_TYPECAST(GraDoRightIdBlockAction)
	COM_TYPECAST_ENTRY(_IGrammarAction, _IGrammarAction)
	COM_TYPECAST_ENTRY(_I_GraDataAction_Utility, _GraDataActionBase)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
