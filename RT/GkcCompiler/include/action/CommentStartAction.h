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
This file contains component class of comment start action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __COMMENT_START_ACTION_H__
#define __COMMENT_START_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// CommentStartAction

class CommentStartAction : public _ILexerAction
{
public:
	CommentStartAction() throw()
	{
	}
	~CommentStartAction() throw()
	{
	}

// _ILexerAction methods
	virtual GKC::CallResult DoAction(GKC::ShareCom<GKC::ITextStream>& stream, _LexerTokenInfo& info) throw()
	{
		CallResult cr;

		CharA ch;
		//next char
		cr = _get_next_char(stream, info, ch);
		if( cr.IsFailed() )
			return cr;

		do {
			//CR
			if( ch == '\r' ) {
				cr = info.AddCharEndRow(1);
				if( cr.IsFailed() )
					break;
				info.ResetCharEndCol();

				//next char
				cr = _get_next_char(stream, info, ch);
				if( cr.IsFailed() )
					break;
				if( ch != '\n' )
					continue;
			}
			//LN
			else if( ch == '\n' ) {
				cr = info.AddCharEndRow(1);
				if( cr.IsFailed() )
					break;
				info.ResetCharEndCol();
			}
			//*
			else if( ch == '*' ) {
				cr = info.AddCharEndCol(1);
				if( cr.IsFailed() )
					break;

				//next char
				cr = _get_next_char(stream, info, ch);
				if( cr.IsFailed() )
					break;
				if( ch == '/' ) {
					cr = info.AddCharEndCol(1);
					if( cr.IsFailed() )
						break;
					info.set_ID(CPL_TK_NULL);  //comment end
					break;
				}
				continue;
			}
			else {
				cr = info.AddCharEndCol(1);
				if( cr.IsFailed() )
					break;
			} //end if

			//next char
			cr = _get_next_char(stream, info, ch);
			if( cr.IsFailed() )
				break;
		} while( true );

		return cr;
	}

private:
	static CallResult _get_next_char(GKC::ShareCom<GKC::ITextStream>& stream, _LexerTokenInfo& info, CharA& ch) throw()
	{
		CallResult cr;
		cr = stream.Deref().GetCharA(ch);
		if( cr.GetResult() == SystemCallResults::S_EOF ) {
			cr.SetResult(SystemCallResults::Fail);
		}
		if( cr.IsFailed() ) {
			info.set_ID(CPL_TK_ERROR);
			info.set_ErrorString(DECLARE_TEMP_CONST_STRING(GKC::ConstStringS, _S("No expected comment end")));
			return cr;
		}
		cr = info.AddCharEndCharIndex(1);
		return cr;
	}

private:
	//noncopyable
	CommentStartAction(const CommentStartAction&) throw();
	CommentStartAction& operator=(const CommentStartAction&) throw();
};

DECLARE_COM_TYPECAST(CommentStartAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
