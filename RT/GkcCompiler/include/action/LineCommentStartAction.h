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
This file contains component class of line comment start action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __LINE_COMMENT_START_ACTION_H__
#define __LINE_COMMENT_START_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// LineCommentStartAction

class LineCommentStartAction : public _ILexerAction
{
public:
	LineCommentStartAction() throw()
	{
	}
	~LineCommentStartAction() throw()
	{
	}

// _ILexerAction methods
	virtual GKC::CallResult DoAction(GKC::ShareCom<GKC::ITextStream>& stream, _LexerTokenInfo& info, GKC::ConstStringA& strToken, bool& bTokenChanged) throw()
	{
		CallResult cr;

		CharF ch;
		bool  bCheckLN = false;
		do {
			//next char
			cr = _get_next_char(stream, info, ch);
			if( cr.IsFailed() )
				break;
			if( cr.GetResult() == SystemCallResults::S_EOF ) {
				cr.SetResult(SystemCallResults::OK);
				break;
			}

			//check LN
			if( bCheckLN ) {
				if( ch != '\n' ) {
					cr = stream.Deref().UngetChar(1);
					if( cr.IsFailed() )
						break;
					info.set_ID(CPL_TK_NULL);  //comment end
					break;
				}
				info.set_ID(CPL_TK_NULL);  //comment end
				break;
			}

			//CR
			if( ch == '\r' ) {
				cr = info.AddCharEndRow(1);
				if( cr.IsFailed() )
					break;
				info.ResetCharEndCol();

				bCheckLN = true;
			}
			//LN
			else if( ch == '\n' ) {
				cr = info.AddCharEndRow(1);
				if( cr.IsFailed() )
					break;
				info.ResetCharEndCol();

				info.set_ID(CPL_TK_NULL);  //comment end
				break;
			}
			else {
				cr = info.AddCharEndCol(1);
				if( cr.IsFailed() )
					break;
			} //end if
		} while( true );

		return cr;
	}

private:
	static CallResult _get_next_char(GKC::ShareCom<GKC::ITextStream>& stream, _LexerTokenInfo& info, CharF& ch) throw()
	{
		CallResult cr;
		cr = stream.Deref().GetChar(ch);
		if( cr.IsFailed() ) {
			info.set_ID(CPL_TK_ERROR);
			info.set_ErrorString(DECLARE_TEMP_CONST_STRING(GKC::ConstStringS, _S("No expected comment end")));
			return cr;
		}
		if( cr.GetResult() == SystemCallResults::S_EOF ) {
			info.set_ID(CPL_TK_NULL);  //comment end
			return cr;
		}
		cr = info.AddCharEndCharIndex(1);
		return cr;
	}

private:
	//noncopyable
	LineCommentStartAction(const LineCommentStartAction&) throw();
	LineCommentStartAction& operator=(const LineCommentStartAction&) throw();
};

DECLARE_COM_TYPECAST(LineCommentStartAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
