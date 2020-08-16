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
This file contains component class of comment start action for WMARK.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WMARK_COMMENT_START_ACTION_H__
#define __WMARK_COMMENT_START_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkCommentStartAction

class WmarkCommentStartAction : public ILexerAction
{
public:
	WmarkCommentStartAction() throw()
	{
	}
	~WmarkCommentStartAction() throw()
	{
	}

// ILexerAction methods
	virtual GKC::CallResult DoAction(GKC::ShareCom<GKC::ITextStreamRoot>& stream, _LexerTokenInfo& info, GKC::ConstStringA& strToken, bool& bTokenChanged) throw()
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
	static CallResult _get_next_char(GKC::ShareCom<GKC::ITextStreamRoot>& stream, _LexerTokenInfo& info, CharF& ch) throw()
	{
		CallResult cr;
		cr = stream.Deref().GetChar(ch);
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
	static CallResult do_action(ShareCom<ITextStreamRoot>& stream, _LexerTokenInfo& info)
	{
		CallResult cr;
		CharF ch;

		//receive the content of comment
		info.get_Data().SetLength(0);  //may throw
		//next char
		cr = _get_next_char(stream, info, ch);
		if( cr.IsFailed() )
			return cr;

		//states
		int iState = 1;
		do {
			info.get_Data().Append(ch);  //may throw

			//-
			if( ch == '-' ) {
				cr = info.AddCharEndCol(1);
				if( cr.IsFailed() )
					break;

				//states
				switch( iState ) {
				case 1:
					iState = 2;
					break;
				case 2:
					iState = 3;
					break;
				default:
					break;
				}
			}
			else if( ch == '>' ) {
				cr = info.AddCharEndCol(1);
				if( cr.IsFailed() )
					break;

				bool bEnd = false;
				//states
				switch( iState ) {
				case 2:
					iState = 1;
					break;
				case 3:
					bEnd = true;
					break;
				default:
					break;
				}
				if( bEnd ) {
					/*
					info.set_ID(CPL_TK_NULL);  //comment end
					*/
					break;
				}
			}
			else {
				//states
				switch( iState ) {
				case 2:
				case 3:
					iState = 1;
					break;
				default:
					break;
				}

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
				else {
					cr = info.AddCharEndCol(1);
					if( cr.IsFailed() )
						break;
				}
			} //end if

			//next char
			cr = _get_next_char(stream, info, ch);
			if( cr.IsFailed() )
				break;
		} while( true );

		return cr;
	}

private:
	//noncopyable
	WmarkCommentStartAction(const WmarkCommentStartAction&) throw();
	WmarkCommentStartAction& operator=(const WmarkCommentStartAction&) throw();
};

DECLARE_COM_TYPECAST(WmarkCommentStartAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
