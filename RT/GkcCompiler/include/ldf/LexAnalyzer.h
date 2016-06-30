/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef ___LDF_LEX_H__
#define ___LDF_LEX_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//for lex file

private:
	// _LDF_lex_SymUserData
	class _LDF_lex_SymUserData : public SymbolDataBase
	{
	public:
	};

	// reduction action table for lex file
	class _LDF_lex_ActionTable
	{
	public:
		//called only once
		void Init()
		{
			uint uID = 1;
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_ref"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_rule_block_id"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_rule_id"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_token"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_macro"), uID ++);
		}

		const TokenTable& GetTable() const throw()
		{
			return m_table;
		}

	private:
		TokenTable m_table;
	};

	// PDA for lex file
	class _LDF_lex_PdaTraits
	{
	public:
	};

	typedef PushDownMachineT<_LDF_lex_SymUserData, _LDF_lex_PdaTraits> _LDF_lex_PDM;

	// grammar actions
	class Lexgra_DoIdTokenMacroAction : public IGrammarAction<_LDF_lex_SymUserData>
	{
	public:
		Lexgra_DoIdTokenMacroAction(TokenTable& tt, SharedArray<StringA>& arr) throw() : m_tt(tt), m_arr(arr), m_uCurrentID(TK_FIRST)
		{
		}
		virtual void DoAction(INOUT SharedArray<RefPtr<_LDF_lex_SymUserData>>& arr, INOUT SharedArray<StringS>& errorArray)
		{
			StringA& strToken = arr[1].get_Value().Deref().GetBuffer();
			ConstStringA cs_token = StringUtilHelper::To_ConstString(strToken);
			if( m_tt.GetTokenID(cs_token) > 0 ) {
				StringS strError(StringUtilHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
				CplErrorBuffer tmp;
				int ret = value_to_string(FixedArrayHelper::GetInternalPointer(tmp), CplErrorBuffer::c_size,
										_S("Error (%u) : (%u) %s redifinition"),
										arr[1].get_Value().Deref().GetCharStart().uRow + 1,
										arr[1].get_Value().Deref().GetCharStart().uCol + 1,
										ConstArrayHelper::GetInternalPointer(CS_U2S(cs_token).GetC())
										);
				if( ret >= 0 )
					tmp.SetLength(ret);
				StringUtilHelper::MakeString(tmp, strError);  //may throw
				errorArray.Add(strError);
				return ;
			}
			uint uLeastCount = SafeOperators::AddThrow(m_uCurrentID, (uint)1);  //may throw
			if( m_arr.GetCount() < (uintptr)uLeastCount )
				m_arr.SetCount((uintptr)uLeastCount, 0);
			m_arr.SetAt(m_uCurrentID, arr[1].get_Value().Deref().GetAux());
			m_tt.InsertToken(cs_token, m_uCurrentID ++);
		}

	private:
		TokenTable&            m_tt;
		SharedArray<StringA>&  m_arr;
		uint m_uCurrentID;
	};

public:
	// _LDF_lex_GrammarTable
	class _LDF_lex_GrammarTable
	{
	public:
		//called only once
		void Init()
		{
			m_actTable.Init();  //may throw
			m_graTable.SetPDA(RefPtrHelper::TypeCast<_LDF_lex_PDM, typename _LDF_lex_PDM::baseClass>(RefPtr<_LDF_lex_PDM>(m_pdm)));
			m_graTable.SetReductionActionTable(RefPtrHelper::MakeRefPtr(m_actTable.GetTable()));
		}

		RefPtr<GrammarTable<_LDF_lex_SymUserData>> GetGrammarTable() throw()
		{
			return RefPtr<GrammarTable<_LDF_lex_SymUserData>>(m_graTable);
		}

	private:
		_LDF_lex_ActionTable m_actTable;
		_LDF_lex_PDM m_pdm;
		GrammarTable<_LDF_lex_SymUserData> m_graTable;
	};

	class _LDF_lex_GrammarAction;
	// _LDF_lex_GrammarData
	class _LDF_lex_GrammarData
	{
	public:
		_LDF_lex_GrammarData(TokenTable& tt) : m_tokenTable(tt),
											m_tokenRegex(SharedArrayHelper::MakeSharedArray<StringA>(MemoryHelper::GetCrtMemoryManager())),
											m_macroRegex(SharedArrayHelper::MakeSharedArray<StringA>(MemoryHelper::GetCrtMemoryManager()))
		{
		}

		SharedArray<StringA>& GetTokenRegex() throw()
		{
			return m_tokenRegex;
		}

		void ExpandTokenMacros()
		{
			for( auto iter(m_tokenRegex.GetBegin()); iter != m_tokenRegex.GetEnd(); iter.MoveNext() )
				expand_macro_one(iter.get_Value());
		}

	private:
		void expand_macro_one(StringA& str)
		{
			if( str.IsNull() )
				return ;

			//loop
			do {
				//find
				uintptr uCount = 0;
				uintptr uStart = 0;
				do {
					auto iter(StringUtilHelper::Find(str, '{', uStart));
					if( iter.IsNull() )
						break;
					intptr delta = iter.CalcDelta(str.GetAt(uStart));
					if( iter != str.GetBegin() ) {
						auto iter1(iter);
						iter1.MovePrev();
						// \{
						if( iter1.get_Value() == '\\' ) {
							uStart += (delta + 1);
							continue;
						}
					}
					uStart += (delta + 1);
					uCount ++;
					uintptr uLeftC = uStart;
					do {
						auto iterE(StringUtilHelper::Find(str, '}', uStart));
						if( iterE.IsNull() ) {
							uCount --;
							break;
						}
						delta = iterE.CalcDelta(str.GetAt(uStart));
						auto iter1(iterE);
						iter1.MovePrev();
						// \}
						if( iter1.get_Value() == '\\' ) {
							uStart += (delta + 1);
							continue;
						}
						uStart += (delta + 1);
						intptr len = iterE.CalcDelta(iter);
						//empty
						if( len <= 1 ) {
							uCount --;
							break;
						}
						//macro name
						StringA strMacro(StringUtilHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));
						StringUtilHelper::Sub(str, uLeftC, len - 1, strMacro);
						ConstStringA c_macro_name = StringUtilHelper::To_ConstString(strMacro);
						uint id = m_macroTable.GetTokenID(c_macro_name);
						StringA strV;
						if( id == 0 ) {
							id = m_tokenTable.GetTokenID(c_macro_name);
							if( id == 0 ) {
								uCount --;
								break;
							}
							strV = m_tokenRegex[id].get_Value();
						}
						else {
							strV = m_macroRegex[id].get_Value();
						}
						iter.get_Value() = '(';
						iterE.get_Value() = ')';
						StringUtilHelper::Delete(uLeftC, len - 1, str);
						StringUtilHelper::Insert(uLeftC, strV, str);
						//now iterators are invalid
						uStart = uLeftC + strV.GetLength() + 1;
						break;
					} while(true);
				} while(true);
				if( uCount == 0 )
					break;
			} while(true);
		}

	private:
		TokenTable& m_tokenTable;
		TokenTable  m_macroTable;
		SharedArray<StringA> m_tokenRegex;  //regular expression
		SharedArray<StringA> m_macroRegex;  //regular expression

	private:
		friend class _LDF_lex_GrammarAction;
	};

	// _LDF_lex_GrammarAction
	class _LDF_lex_GrammarAction
	{
	public:
		_LDF_lex_GrammarAction(_LDF_lex_GrammarData& data) throw() : m_actionDoIdToken(data.m_tokenTable, data.m_tokenRegex),
																	m_actionDoIdMacro(data.m_macroTable, data.m_macroRegex)
		{
		}

		void Apply(GrammarParser<_LDF_lex_SymUserData>& graParser)
		{
			graParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_token"),
								RefPtrHelper::TypeCast<Lexgra_DoIdTokenMacroAction, IGrammarAction<_LDF_lex_SymUserData>>(RefPtr<Lexgra_DoIdTokenMacroAction>(m_actionDoIdToken)));
			graParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_macro"),
								RefPtrHelper::TypeCast<Lexgra_DoIdTokenMacroAction, IGrammarAction<_LDF_lex_SymUserData>>(RefPtr<Lexgra_DoIdTokenMacroAction>(m_actionDoIdMacro)));
		}

	private:
		Lexgra_DoIdTokenMacroAction m_actionDoIdToken;
		Lexgra_DoIdTokenMacroAction m_actionDoIdMacro;
	};

	// _LDF_lex_GrammarParser
	typedef _LDF_GrammarParser<_LDF_lex_SymUserData>  _LDF_lex_GrammarParser;

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //___LDF_LEX_H__
////////////////////////////////////////////////////////////////////////////////
