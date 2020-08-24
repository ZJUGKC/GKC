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
This file contains the global variables for regex actions.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "base/DataColl.h"

#include "ldf/regex/RegexDef.h"
#include "ldf/regex/RegexCharAction.h"
#include "ldf/regex/RegexCharSymbolData.h"
#include "ldf/regex/RegexCharSymbolData_Factory.h"
#include "ldf/regex/RegexCharSetSymbolData.h"
#include "ldf/regex/RegexCharSetSymbolData_Factory.h"
#include "ldf/regex/RegexPositionSymbolData.h"
#include "ldf/regex/RegexPositionSymbolData_Factory.h"
#include "ldf/regex/RegexDoCharAction.h"
#include "ldf/regex/RegexDoCharSAction.h"
#include "ldf/regex/RegexDoCharRangeAction.h"
#include "ldf/regex/RegexDoCharItemCharEAction.h"
#include "ldf/regex/RegexDoCharItemItemCharEAction.h"
#include "ldf/regex/RegexDoCharSetAction.h"
#include "ldf/regex/RegexDoCharSetUpAction.h"
#include "ldf/regex/RegexDoFactorCharSetAction.h"
#include "ldf/regex/RegexDoFactorCharAction.h"
#include "ldf/regex/RegexDoFactorParenExpAction.h"
#include "ldf/regex/RegexDoFactor1QuestionAction.h"
#include "ldf/regex/RegexDoFactor1StarAction.h"
#include "ldf/regex/RegexDoFactor1PlusAction.h"
#include "ldf/regex/RegexDoTermTermFactor1Action.h"
#include "ldf/regex/RegexDoExpExpTermAction.h"
#include "ldf/regex/RegexGrammarAccepted.h"

#include "ldf/RegexAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// global variables

// lexer
_ScopeShareComObject<RegexCharAction> g_actionRegexChar;

// factory
_ScopeShareComObject<USE_COM_FACTORY_CLASS_NAME(RegexCharSymbolData)> g_factoryRegexCharSymbolData;
_ScopeShareComObject<USE_COM_FACTORY_CLASS_NAME(RegexCharSetSymbolData)> g_factoryRegexCharSetSymbolData;
_ScopeShareComObject<USE_COM_FACTORY_CLASS_NAME(RegexPositionSymbolData)> g_factoryRegexPositionSymbolData;

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
