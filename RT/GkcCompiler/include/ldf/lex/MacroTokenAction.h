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
This file contains component class of Macro-Token action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __MACROTOKEN_ACTION_H__
#define __MACROTOKEN_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// MacroTokenAction

class MacroTokenAction : public _ILexerAction
{
public:
	MacroTokenAction() throw()
	{
	}
	~MacroTokenAction() throw()
	{
	}

// _ILexerAction methods
	virtual GKC::CallResult DoAction(GKC::ShareCom<GKC::ITextStream>& stream, _LexerTokenInfo& info, GKC::ConstStringA& strToken, bool& bTokenChanged) throw()
	{
		CallResult cr;
		try {
			cr = do_action(stream, info);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}

private:
	static CallResult _get_next_char(ShareCom<ITextStream>& stream, CharF& ch) throw()
	{
		CallResult cr;
		cr = stream.Deref().GetChar(ch);
		if( cr.GetResult() == SystemCallResults::S_EOF )
			cr.SetResult(SystemCallResults::Fail);  //not a line
		return cr;
	}
	static CallResult do_action(ShareCom<ITextStream>& stream, _LexerTokenInfo& info)
	{
		CallResult cr;
		CharF ch;

		//receive the regular expression
		info.get_Data().SetLength(0);  //may throw
		//next char
		cr = _get_next_char(stream, ch);
		if( cr.IsFailed() )
			return cr;
		//skip blanks
		while( ch == ' ' || ch == '\t' ) {
			cr = info.AddCharEndCharIndex(1);
			if( cr.IsFailed() )
				return cr;
			cr = info.AddCharEndCol(1);
			if( cr.IsFailed() )
				return cr;
			//next char
			cr = _get_next_char(stream, ch);
			if( cr.IsFailed() )
				return cr;
		}
		//regex
		while( ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n' ) {
			cr = info.AddCharEndCharIndex(1);
			if( cr.IsFailed() )
				return cr;
			cr = info.AddCharEndCol(1);
			if( cr.IsFailed() )
				return cr;
			info.get_Data().Append(ch);  //may throw
			//next char
			cr = _get_next_char(stream, ch);
			if( cr.IsFailed() )
				return cr;
		}

		//back char
		cr = stream.Deref().UngetChar(1);
		return cr;
	}

private:
	//noncopyable
	MacroTokenAction(const MacroTokenAction&) throw();
	MacroTokenAction& operator=(const MacroTokenAction&) throw();
};

DECLARE_COM_TYPECAST(MacroTokenAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
