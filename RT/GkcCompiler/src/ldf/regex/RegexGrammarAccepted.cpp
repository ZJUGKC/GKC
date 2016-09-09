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
This file contains global variables for regular expression grammar accepted action component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "base/DataColl.h"

#include "ldf/regex/RegexDef.h"
#include "ldf/regex/RegexGrammarAccepted.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexGrammarAccepted

BEGIN_COM_TYPECAST(RegexGrammarAccepted)
	COM_TYPECAST_ENTRY(_IGrammarAccepted, _IGrammarAccepted)
	COM_TYPECAST_ENTRY(_I_RegexAstAction_Utility, _RegexAstActionBase)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
