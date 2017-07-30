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
This file contains component class of Do-Char action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_DO_CHAR_ACTION_H__
#define __REGEX_DO_CHAR_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexDoCharAction

class RegexDoCharAction : public _IGrammarAction
{
public:
	RegexDoCharAction() throw()
	{
	}
	~RegexDoCharAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		//character value
		uint uValue = 0;
		{
			_LexerTokenString& str = arrSymbol[1].get_Value().Deref().get_Buffer().Deref();
			if( str.GetLength() > 2 ) {
				bool bOK = str.ToHexadecimalInteger(2, uValue);
				bOK;
				assert( bOK );
			}
			else {
				CharF ch;
				str.GetAt(0, ch);
				uValue = (uint)ch;
			}
		} //end block
		assert( uValue != 0 );
		//set value
		ShareCom<_I_RegexCharSymbolData_Utility> spU;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexCharSymbolData_Utility, arrSymbol[0].get_Value(), spU, cr);
		if( cr.IsFailed() )
			return cr;
		_RegexCharRange rcr;
		rcr.uLow = rcr.uHigh = uValue;
		spU.Deref().SetCharRange(rcr);
		return cr;
	}

private:
	//noncopyable
	RegexDoCharAction(const RegexDoCharAction&) throw();
	RegexDoCharAction& operator=(const RegexDoCharAction&) throw();
};

DECLARE_COM_TYPECAST(RegexDoCharAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
