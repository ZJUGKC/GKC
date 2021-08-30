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
Actid-Action
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __ACTID_ACTION_H__
#define __ACTID_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// ActidAction

class ActidAction : public _IRdScannerAction
{
public:
	ActidAction() noexcept
	{
	}
	ActidAction(const ActidAction&) = delete;
	ActidAction& operator=(const ActidAction&) = delete;
	~ActidAction() noexcept
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
				if ( state == 5 )
					info.set_ID(TK_ACT);
				else
					info.set_ID(CPL_TK_ERROR);
				cr.SetResult(SystemCallResults::OK);
				return cr;
			}

			switch ( state ) {
			case 1:
				if ( ch == 'C' )
					state = 2;
				else
					state = 0;
				break;
			case 2:
				if ( ch == 'T' )
					state = 3;
				else
					state = 0;
				break;
			case 3:
				if ( ch == '_' )
					state = 4;
				else
					state = 0;
				break;
			case 4:
				if ( is_id_char(ch) )
					state = 5;
				else
					state = 0;
				break;
			case 5:
				if ( !is_id_char(ch) )
					state = 6;
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

			if ( state == 6 ) {
				info.set_ID(TK_ACT);
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
		return ch == '_' || (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
	}
};

DECLARE_COM_TYPECAST(ActidAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
