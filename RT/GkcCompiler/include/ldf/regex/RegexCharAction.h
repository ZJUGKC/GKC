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
This file contains component class of regular expression character action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_CHAR_ACTION_H__
#define __REGEX_CHAR_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexCharAction

class RegexCharAction : public _ILexerAction
{
public:
	RegexCharAction() throw()
	{
	}
	~RegexCharAction() throw()
	{
	}

// _ILexerAction methods
	virtual GKC::CallResult DoAction(GKC::ShareCom<GKC::ITextStream>& stream, _LexerTokenInfo& info, GKC::ConstStringA& strToken, bool& bTokenChanged) throw()
	{
		//fetch the actual character
		StringA& str = info.get_Buffer();
		assert( str.GetLength() > 1 );
		//value
		CharA ch = str.GetAt(1).get_Value();
		if( ch == 'r' )
			str.GetAt(0).set_Value('\r');
		else if( ch == 'n' )
			str.GetAt(0).set_Value('\n');
		else if( ch == 't' )
			str.GetAt(0).set_Value('\t');
		else if( ch == 's' )
			str.GetAt(0).set_Value(' ');
		else if( ch == 'x' ) {
			bool bOK;
			uint v;
			string_to_value(ShareArrayHelper::GetInternalPointer(str) + 2, 16, v, bOK);  //no check
			assert( bOK );
			str.GetAt(0).set_Value((CharA)v);
		}
		else
			str.GetAt(0).set_Value(ch);
		str.SetLength(1);
		//change ID
		strToken = DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR");
		bTokenChanged = true;
		return CallResult();
	}

private:
	//noncopyable
	RegexCharAction(const RegexCharAction&) throw();
	RegexCharAction& operator=(const RegexCharAction&) throw();
};

DECLARE_COM_TYPECAST(RegexCharAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
