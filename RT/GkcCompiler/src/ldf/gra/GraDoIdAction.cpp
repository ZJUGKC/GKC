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
This file contains global variables for Do-Id action component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "base/DataColl.h"
#include "base/Fsa.h"
#include "base/Lexer.h"

#include "ldf/gra/GraDef.h"
#include "ldf/gra/GraDoIdAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GraDoIdAction

BEGIN_COM_TYPECAST(GraDoIdAction)
	COM_TYPECAST_ENTRY(_IGrammarAction, _IGrammarAction)
	COM_TYPECAST_ENTRY(_I_GraDataAction_Utility, _GraDataActionBase)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
