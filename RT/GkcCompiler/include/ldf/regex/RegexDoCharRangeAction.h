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
This file contains component class of Do-Char-Range action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_DO_CHAR_RANGE_ACTION_H__
#define __REGEX_DO_CHAR_RANGE_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexDoCharRangeAction

class RegexDoCharRangeAction : public _IGrammarAction
{
public:
	RegexDoCharRangeAction() throw()
	{
	}
	~RegexDoCharRangeAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		//values
		ShareCom<_I_RegexCharSymbolData_Utility> spU;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexCharSymbolData_Utility, arrSymbol[1], spU, cr);
		if( cr.IsFailed() )
			return cr;
		_RegexCharRange rcr;
		spU.Deref().GetCharRange(rcr);
		assert( rcr.uLow == rcr.uHigh );
		uint u1 = rcr.uLow;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexCharSymbolData_Utility, arrSymbol[3], spU, cr);
		if( cr.IsFailed() )
			return cr;
		spU.Deref().GetCharRange(rcr);
		assert( rcr.uLow == rcr.uHigh );
		assert( u1 <= rcr.uHigh );
		//set
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexCharSymbolData_Utility, arrSymbol[0], spU, cr);
		if( cr.IsFailed() )
			return cr;
		rcr.uLow = u1;
		spU.Deref().SetCharRange(rcr);
		return cr;
	}

private:
	//noncopyable
	RegexDoCharRangeAction(const RegexDoCharRangeAction&) throw();
	RegexDoCharRangeAction& operator=(const RegexDoCharRangeAction&) throw();
};

DECLARE_COM_TYPECAST(RegexDoCharRangeAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
