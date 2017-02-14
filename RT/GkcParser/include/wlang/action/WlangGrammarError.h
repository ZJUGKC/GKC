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
This file contains component class of WLang grammar error.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WLANG_GRAMMAR_ERROR_H__
#define __WLANG_GRAMMAR_ERROR_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WlangGrammarError

class WlangGrammarError : public IGrammarError
{
public:
	WlangGrammarError() throw()
	{
	}
	~WlangGrammarError() throw()
	{
	}

// IGrammarError methods
	virtual GKC::CallResult DoModifyEvent(INOUT GKC::ConstStringA& strEvent, INOUT GKC::ShareCom<GKC::ITextStream>& spText, INOUT bool& bChanged) throw()
	{
		CallResult cr;

		// ">>" --> ">"
		if( ConstStringCompareTrait<ConstStringA>::IsEQ(strEvent, DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RIGHT_SHIFT")) ) {
			cr = spText.Deref().UngetChar(1);
			if( cr.IsFailed() )
				return cr;
			strEvent = DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_LOG_GT");
			bChanged = true;
		}

		return cr;
	}

private:
	//noncopyable
	WlangGrammarError(const WlangGrammarError&) throw();
	WlangGrammarError& operator=(const WlangGrammarError&) throw();
};

DECLARE_COM_TYPECAST(WlangGrammarError)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
