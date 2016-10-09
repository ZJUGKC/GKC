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
This file contains global variables for WLang grammar error component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "wlang/WlangGrammarError.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WlangGrammarError

BEGIN_COM_TYPECAST(WlangGrammarError)
	COM_TYPECAST_ENTRY(IGrammarError, IGrammarError)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
