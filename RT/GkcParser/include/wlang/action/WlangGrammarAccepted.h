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
This file contains component class of WLANG grammar accepted action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WLANG_GRAMMAR_ACCEPTED_H__
#define __WLANG_GRAMMAR_ACCEPTED_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WlangGrammarAccepted

class WlangGrammarAccepted : public IGrammarAccepted,
							public _CplMetaDataActionBase
{
public:
	WlangGrammarAccepted() throw()
	{
	}
	~WlangGrammarAccepted() throw()
	{
	}

// IGrammarAccepted methods
	virtual GKC::CallResult DoAccepted(INOUT GKC::ShareCom<_IGrammarSymbolData>& sym) throw()
	{
		CallResult cr;
		m_spMeta.Deref().FinishZeroLevel();
		return cr;
	}

private:
	//noncopyable
	WlangGrammarAccepted(const WlangGrammarAccepted&) throw();
	WlangGrammarAccepted& operator=(const WlangGrammarAccepted&) throw();
};

DECLARE_COM_TYPECAST(WlangGrammarAccepted)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
