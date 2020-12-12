/*
** Copyright (c) 2020, Xin YUAN, courses of Zhejiang University
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
Tk-Action
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __TK_ACTION_H__
#define __TK_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// TkAction

class TkAction : public _IRdScannerAction
{
public:
	TkAction() noexcept
	{
	}
	TkAction(const TkAction&) = delete;
	TkAction& operator=(const TkAction&) = delete;
	~TkAction() noexcept
	{
	}

// _IRdScannerAction methods
	virtual GKC::CallResult DoAction(GKC::ShareCom<GKC::ITextStreamRoot>& stream, GKC::ShareCom<_IRdScannerTables>& tables, uint& next, _LexerTokenInfo& info) noexcept
	{
		CallResult cr;
		try {
			cr = do_action(stream, tables, next, info);  //may throw
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
	CallResult do_action(ShareCom<ITextStreamRoot>& stream, ShareCom<_IRdScannerTables>& tables, uint& next, _LexerTokenInfo& info)
	{
		CallResult cr;

		CharF ch;
		cr = stream.Deref().GetChar(ch);
		if ( cr.IsFailed() )
			return cr;
		if ( cr.GetResult() == SystemCallResults::S_EOF ) {
			info.set_ID(CPL_TK_EOF);
			cr.SetResult(SystemCallResults::OK);
			return cr;
		}

		info.Append(ch);  //may throw
		cr = info.AddCharEndCharIndex(1);
		if ( cr.IsFailed() )
			return cr;
		cr = info.AddCharEndCol(1);
		if ( cr.IsFailed() )
			return cr;

		if ( ch == '%' )
			return do_sep(stream, tables, next, info);  //may throw

		if ( ch == ' ' || ch == '\t' )
			return do_skip(stream, tables, next, info);

		if ( ch == '\r' || ch == '\n' ) {
			cr = info.AddCharEndRow(1);
			if ( cr.IsFailed() )
				return cr;
			info.ResetCharEndCol();
			if ( ch == '\r' )
				return do_ln(stream, tables, next, info);
			info.set_ID(CPL_TK_NULL);
			return cr;
		}

		if ( ch >= 'a' && ch <= 'z' ) {
			next = ACT_NT;
			return cr;
		}
		if ( ch == 'T' ) {
			next = ACT_TKID;
			return cr;
		}
		if ( ch == 'A' ) {
			next = ACT_ACTID;
			return cr;
		}
		if ( ch == ':' ) {
			info.set_ID(TK_COLON);
			return cr;
		}
		if ( ch == '|' ) {
			info.set_ID(TK_VERT);
			return cr;
		}
		if ( ch == ';' ) {
			info.set_ID(TK_SEMI);
			return cr;
		}
		if ( ch == '{' ) {
			info.set_ID(TK_LCURLY);
			return cr;
		}
		if ( ch == '}' ) {
			info.set_ID(TK_RCURLY);
			return cr;
		}

		info.set_ID(CPL_TK_ERROR);
		return cr;
	}

	CallResult do_sep(ShareCom<ITextStreamRoot>& stream, ShareCom<_IRdScannerTables>& tables, uint& next, _LexerTokenInfo& info)
	{
		CallResult cr;

		CharF ch;
		cr = stream.Deref().GetChar(ch);
		if ( cr.IsFailed() )
			return cr;
		if ( cr.GetResult() == SystemCallResults::S_EOF ) {
			info.set_ID(CPL_TK_ERROR);
			cr.SetResult(SystemCallResults::OK);
			return cr;
		}

		if ( ch == '%' ) {
			cr = info.AddCharEndCharIndex(1);
			if ( cr.IsFailed() )
				return cr;
			cr = info.AddCharEndCol(1);
			if ( cr.IsFailed() )
				return cr;
			info.Append(ch);  //may throw
			info.set_ID(TK_SEP);
			return cr;
		}

		cr = stream.Deref().UngetChar(1);
		if ( cr.IsFailed() )
			return cr;
		info.set_ID(CPL_TK_ERROR);
		return cr;
	}
	CallResult do_skip(ShareCom<ITextStreamRoot>& stream, ShareCom<_IRdScannerTables>& tables, uint& next, _LexerTokenInfo& info) noexcept
	{
		CallResult cr;

		do {
			CharF ch;
			cr = stream.Deref().GetChar(ch);
			if ( cr.IsFailed() )
				return cr;
			if ( cr.GetResult() == SystemCallResults::S_EOF ) {
				info.set_ID(CPL_TK_NULL);
				cr.SetResult(SystemCallResults::OK);
				return cr;
			}

			if ( ch != ' ' && ch != '\t' ) {
				cr = stream.Deref().UngetChar(1);
				break;
			}

			cr = info.AddCharEndCharIndex(1);
			if ( cr.IsFailed() )
				return cr;
			cr = info.AddCharEndCol(1);
			if ( cr.IsFailed() )
				return cr;
		} while ( true );

		info.set_ID(CPL_TK_NULL);
		return cr;
	}
	CallResult do_ln(ShareCom<ITextStreamRoot>& stream, ShareCom<_IRdScannerTables>& tables, uint& next, _LexerTokenInfo& info) noexcept
	{
		CallResult cr;

		CharF ch;
		cr = stream.Deref().GetChar(ch);
		if ( cr.IsFailed() )
			return cr;
		if ( cr.GetResult() == SystemCallResults::S_EOF ) {
			info.set_ID(CPL_TK_NULL);
			cr.SetResult(SystemCallResults::OK);
			return cr;
		}

		info.set_ID(CPL_TK_NULL);
		if ( ch != '\n' )
			cr = stream.Deref().UngetChar(1);
		else
			cr = info.AddCharEndCharIndex(1);
		return cr;
	}
};

DECLARE_COM_TYPECAST(TkAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
