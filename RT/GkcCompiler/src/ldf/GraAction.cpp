/*
** Copyright (c) 2020, Xin YUAN, courses of Zhejiang University
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
This file contains the global variables for gra actions.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "base/DataColl.h"
#include "base/Fsa.h"
#include "base/Lexer.h"

#include "ldf/gra/GraDef.h"
#include "ldf/gra/GraTokenSymbolData.h"
#include "ldf/gra/GraTokenSymbolData_Factory.h"
#include "ldf/gra/GraPositionSymbolData.h"
#include "ldf/gra/GraPositionSymbolData_Factory.h"
#include "ldf/gra/GraDoRuleRightBlockAction.h"
#include "ldf/gra/GraDoRightBlockAction.h"
#include "ldf/gra/GraDoRightAction.h"
#include "ldf/gra/GraDoRightIdBlockAction.h"
#include "ldf/gra/GraDoIdBlockAction.h"
#include "ldf/gra/GraDoIdAction.h"
#include "ldf/gra/GraDoIdTokenAction.h"
#include "ldf/gra/GraDoIdMacroAction.h"

#include "ldf/GraAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// global variables

_ScopeShareComObject<USE_COM_FACTORY_CLASS_NAME(GraTokenSymbolData)>     g_factoryGraTokenSymbolData;
_ScopeShareComObject<USE_COM_FACTORY_CLASS_NAME(GraPositionSymbolData)>  g_factoryGraPositionSymbolData;

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
