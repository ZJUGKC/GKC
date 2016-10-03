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
This file contains global variables for grammar token symbol data component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "base/DataColl.h"
#include "base/Fsa.h"
#include "base/Lexer.h"

#include "ldf/gra/GraDef.h"
#include "ldf/gra/GraTokenSymbolData.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GraTokenSymbolData

BEGIN_COM_TYPECAST(GraTokenSymbolData)
	COM_TYPECAST_ENTRY(_IGrammarSymbolData, _GrammarSymbolDataBase)
	COM_TYPECAST_ENTRY(_I_GraTokenSymbolData_Utility, _I_GraTokenSymbolData_Utility)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
