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
Nt-Action
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __NT_ACTION_H__
#define __NT_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// NtAction

class NtAction : public _IRdScannerAction
{
public:
	NtAction() noexcept
	{
	}
	NtAction(const NtAction&) = delete;
	NtAction& operator=(const NtAction&) = delete;
	~NtAction() noexcept
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
		info.get_Buffer().GetAt(0, ch);

		int state = 1;
		if ( ch == 'e' )
			state = 2;

		do {
			cr = stream.Deref().GetChar(ch);
			if ( cr.IsFailed() )
				return cr;
			if ( cr.GetResult() == SystemCallResults::S_EOF ) {
				if ( state == 8 )
					info.set_ID(TK_SEP);
				else
					info.set_ID(TK_NT);
				cr.SetResult(SystemCallResults::OK);
				return cr;
			}

			switch ( state ) {
			case 1:
				break;
			case 2:
				if ( ch == 'p' )
					state = 3;
				else
					state = 1;
				break;
			case 3:
				if ( ch == 's' )
					state = 4;
				else
					state = 1;
				break;
			case 4:
				if ( ch == 'i' )
					state = 5;
				else
					state = 1;
				break;
			case 5:
				if ( ch == 'l' )
					state = 6;
				else
					state = 1;
				break;
			case 6:
				if ( ch == 'o' )
					state = 7;
				else
					state = 1;
				break;
			case 7:
				if ( ch == 'n' )
					state = 8;
				else
					state = 1;
				break;
			case 8:
				if ( !is_id_char(ch) ) {
					info.set_ID(TK_EPS);
					cr = stream.Deref().UngetChar(1);
					return cr;
				}
				state = 1;
				break;
			default:
				info.set_ID(CPL_TK_ERROR);
				return cr;
				break;
			}

			if ( is_id_char(ch) ) {
				info.Append(ch);  //may throw
				cr = info.AddCharEndCharIndex(1);
				if ( cr.IsFailed() )
					return cr;
				cr = info.AddCharEndCol(1);
				if ( cr.IsFailed() )
					return cr;
			}
			else {
				cr = stream.Deref().UngetChar(1);
				break;
			}
		} while ( true );

		info.set_ID(TK_NT);
		return cr;
	}

	static bool is_id_char(CharF ch) noexcept
	{
		return ch == '_' || (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z');
	}
};

DECLARE_COM_TYPECAST(NtAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
