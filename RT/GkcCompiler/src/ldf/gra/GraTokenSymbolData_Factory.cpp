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
This file contains global variables for grammar token symbol data factory component.
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

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_COM_FACTORY_CLASS(GraTokenSymbolData)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
