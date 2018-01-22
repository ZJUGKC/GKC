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
This file contains component class of WMARK grammar accepted action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WMARK_GRAMMAR_ACCEPTED_H__
#define __WMARK_GRAMMAR_ACCEPTED_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkGrammarAccepted

class WmarkGrammarAccepted : public IGrammarAccepted,
							public _CplMetaDataActionBase
{
public:
	WmarkGrammarAccepted() throw()
	{
	}
	~WmarkGrammarAccepted() throw()
	{
	}

// IGrammarAccepted methods
	virtual GKC::CallResult DoAccepted(INOUT GKC::ShareCom<_IGrammarSymbolData>& sym) throw()
	{
		CallResult cr;
		m_spMeta.Deref().FinishZeroLevel(false);
		return cr;
	}

private:
	//noncopyable
	WmarkGrammarAccepted(const WmarkGrammarAccepted&) throw();
	WmarkGrammarAccepted& operator=(const WmarkGrammarAccepted&) throw();
};

DECLARE_COM_TYPECAST(WmarkGrammarAccepted)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
