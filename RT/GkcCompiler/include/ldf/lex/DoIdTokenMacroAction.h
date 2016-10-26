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
This file contains component class of Do-Id-Token-Macro action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __DO_ID_TOKENMACRO_ACTION_H__
#define __DO_ID_TOKENMACRO_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// DoIdTokenMacroAction

class DoIdTokenMacroAction : public _IGrammarAction,
							public _I_IdTokenMacroAction_Utility
{
public:
	DoIdTokenMacroAction() throw() : m_uMinID(CPL_TK_FIRST)
	{
	}
	~DoIdTokenMacroAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		try {
			do_action(arrSymbol, errorArray);  //may throw
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}

// _I_IdTokenMacroAction_Utility methods
	virtual void SetOutput(const GKC::RefPtr<GKC::TokenTable>& table, const GKC::ShareArray<GKC::StringA>& arr, const GKC::RefPtr<uint>& uID) throw()
	{
		m_table = table;
		m_arr = arr;
		m_uCurrentID = uID;
		m_uMinID = uID.Deref();
	}

private:
	void do_action(ShareArray<ShareCom<_IGrammarSymbolData>>& arrSymbol, ShareArray<StringS>& errorArray)
	{
		StringA strToken(arrSymbol[1].get_Value().Deref().get_Buffer());
		RefPtr<_LEXER_WORD_INFO> info(arrSymbol[1].get_Value().Deref().get_WordInfo());
		ConstStringA cs_token(StringUtilHelper::To_ConstString(strToken));
		if( m_table.Deref().get_ID(cs_token) > 0 ) {
			StringS strError(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
			_CplErrorStringHelper::GenerateError(info.Deref().infoStart.uRow,
												info.Deref().infoStart.uCol,
												CS_U2S(cs_token).GetC(),
												DECLARE_TEMP_CONST_STRING(ConstStringS, _S("redifinition.")),
												strError);  //may throw
			errorArray.Add(strError);  //may throw
			return ;
		}
		uint& uCID = m_uCurrentID.Deref();
		uint uID = uCID - m_uMinID;
		uint uLeastCount = SafeOperators::AddThrow(uID, (uint)1);  //may throw
		if( m_arr.GetCount() < (uintptr)uLeastCount )
			m_arr.SetCount((uintptr)uLeastCount, 0);  //may throw
		m_arr.SetAt(uID, arrSymbol[1].get_Value().Deref().get_Data());
		m_table.Deref().InsertToken(cs_token, uCID);  //may throw
		uCID = SafeOperators::AddThrow(uCID, (uint)1);  //may throw
	}

private:
	RefPtr<TokenTable>   m_table;
	ShareArray<StringA>  m_arr;
	//ID
	uint m_uMinID;
	RefPtr<uint> m_uCurrentID;

private:
	//noncopyable
	DoIdTokenMacroAction(const DoIdTokenMacroAction&) throw();
	DoIdTokenMacroAction& operator=(const DoIdTokenMacroAction&) throw();
};

DECLARE_COM_TYPECAST(DoIdTokenMacroAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
