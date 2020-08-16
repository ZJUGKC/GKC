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
	virtual GKC::CallResult DoAction(GKC::ShareCom<GKC::ITextStreamRoot>& stream, _LexerTokenInfo& info, GKC::ConstStringA& strToken, bool& bTokenChanged) throw()
	{
		//fetch the actual character
		_LexerTokenString& str = info.get_Buffer();
		assert( str.GetLength() > 1 );
		//value
		CharF ch;
		str.GetAt(1, ch);
		//reserve "\xDD..."
		if( ch != 'x' ) {
			if( ch == 'r' )
				str.SetAt(0, '\r');
			else if( ch == 'n' )
				str.SetAt(0, '\n');
			else if( ch == 't' )
				str.SetAt(0, '\t');
			else if( ch == 's' )
				str.SetAt(0, ' ');
			else
				str.SetAt(0, ch);
			str.SetLength(1);
		}
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
