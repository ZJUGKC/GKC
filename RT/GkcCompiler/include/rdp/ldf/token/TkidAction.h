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
Tkid-Action
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __TKID_ACTION_H__
#define __TKID_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// TkidAction

class TkidAction : public _IRdScannerAction
{
public:
	TkidAction() noexcept
	{
	}
	TkidAction(const TkidAction&) = delete;
	TkidAction& operator=(const TkidAction&) = delete;
	~TkidAction() noexcept
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

		int state = 1;

		do {
			CharF ch;
			cr = stream.Deref().GetChar(ch);
			if ( cr.IsFailed() )
				return cr;
			if ( cr.GetResult() == SystemCallResults::S_EOF ) {
				if ( state == 4 )
					info.set_ID(TK_TK);
				else
					info.set_ID(CPL_TK_ERROR);
				cr.SetResult(SystemCallResults::OK);
				return cr;
			}

			switch ( state ) {
			case 1:
				if ( ch == 'K' )
					state = 2;
				else
					state = 0;
				break;
			case 2:
				if ( ch == '_' )
					state = 3;
				else
					state = 0;
				break;
			case 3:
				if ( is_id_char(ch) )
					state = 4;
				else
					state = 0;
				break;
			case 4:
				if ( !is_id_char(ch) )
					state = 5;
				break;
			default:
				info.set_ID(CPL_TK_ERROR);
				return cr;
				break;
			}

			if ( is_id_char(ch) )
				info.Append(ch);  //may throw
			else
				cr = stream.Deref().UngetChar(1);

			if ( state == 5 ) {
				info.set_ID(TK_TK);
				break;
			}
			if ( state == 0 ) {
				info.set_ID(CPL_TK_ERROR);
				break;
			}
		} while ( true );

		return cr;
	}

	static bool is_id_char(CharF ch) noexcept
	{
		return ch == '_' || (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z');
	}
};

DECLARE_COM_TYPECAST(TkidAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
