/*
** Copyright (c) 2017, Xin YUAN, courses of Zhejiang University
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
This file contains global variables for Wmark utility component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcParser.h"

#include "base/ParserDef.h"

#include "wmark/base/WmarkDef.h"
#include "wmark/base/WmarkLex.h"
#include "wmark/base/WmarkGra.h"

#include "wmark/action/WmarkReturnAction.h"
#include "wmark/action/WmarkCommentStartAction.h"
#include "wmark/action/WmarkGrammarAccepted.h"
#include "wmark/action/WmarkDoDocRsBlockListAction.h"
#include "wmark/action/WmarkDoBlockListListBlockAction.h"
#include "wmark/action/WmarkDoBlockBlockBodyAction.h"
#include "wmark/action/WmarkDoBlockBodyCommentAction.h"
#include "wmark/action/WmarkDoBlockBodyIndentAction.h"

#include "wmark/WmarkAction.h"
#include "wmark/WmarkUtility.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkUtility

BEGIN_COM_TYPECAST(WmarkUtility)
	COM_TYPECAST_ENTRY(_IWmarkUtility, _IWmarkUtility)
	COM_TYPECAST_ENTRY(_IWmarkUtility_Access, _IWmarkUtility_Access)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
