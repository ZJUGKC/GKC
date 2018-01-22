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
This file contains global variables for WMARK grammar accepted action component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "wmark/action/WmarkGrammarAccepted.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkGrammarAccepted

BEGIN_COM_TYPECAST(WmarkGrammarAccepted)
	COM_TYPECAST_ENTRY(IGrammarAccepted, IGrammarAccepted)
	COM_TYPECAST_ENTRY(_ICplMetaDataActionUtility, _CplMetaDataActionBase)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
