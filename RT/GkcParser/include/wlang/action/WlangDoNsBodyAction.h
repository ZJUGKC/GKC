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
This file contains component class of Do-Ns-Body action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WLANG_DO_NS_BODY_ACTION_H__
#define __WLANG_DO_NS_BODY_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WlangDoNsBodyAction

class WlangDoNsBodyAction : public IGrammarAction,
							public _CplMetaDataActionBase
{
public:
	WlangDoNsBodyAction() throw()
	{
	}
	~WlangDoNsBodyAction() throw()
	{
	}

// IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		try {
			cr = do_action(arrSymbol, errorArray);  //may throw
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
	//find the same symbol
	_CplMetaDataPosition find_symbol(const ConstStringA& str) throw()
	{
		uint uLevel = m_spMeta.Deref().GetCurrentLevel();
		_CplMetaDataPosition pos(m_spMeta.Deref().Find(str));
		while( !pos.IsNull() ) {
			_CplMetaDataInfo info;
			bool bAnalysis;
			m_spMeta.Deref().GetInfo(pos, info, bAnalysis);
			if( (info.uType & WLANG_NOUN_MASK) == WLANG_NOUN_NAMESPACE && info.uLevel == uLevel && bAnalysis )
				return pos;
			pos = m_spMeta.Deref().FindNext(pos);
		}
		return pos;
	}

	//do action
	CallResult do_action(ShareArray<ShareCom<IGrammarSymbolData>>& arrSymbol, ShareArray<StringS>& errorArray)
	{
		CallResult cr;
		//namespace
		StringA str(arrSymbol[2].get_Value().Deref().get_Buffer().Deref().ToUTF8());  //may throw
		ConstStringA c_str(StringUtilHelper::To_ConstString(str));
		_CplMetaDataPosition pos;
		pos = find_symbol(c_str);
		if( pos.IsNull() ) {
			cr = m_spMeta.Deref().InsertSymbol(c_str, WLANG_NOUN_NAMESPACE & WLANG_NOUN_MASK, true, pos);
			if( cr.IsFailed() ) {
				StringS strError(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
				_CplErrorStringHelper::GenerateError(cr, strError);  //may throw
				errorArray.Add(strError);  //may throw
				return cr;
			}
			_CplMetaDataPosition posData;
			cr = m_spMeta.Deref().InsertData(sizeof(_WlangNsClassMetaData), posData);
			if( cr.IsFailed() ) {
				StringS strError(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
				_CplErrorStringHelper::GenerateError(cr, strError);  //may throw
				errorArray.Add(strError);  //may throw
				return cr;
			}
			m_spMeta.Deref().SetData(pos, posData);
			_WlangNsClassMetaData& data = *((_WlangNsClassMetaData*)(m_spMeta.Deref().GetData(posData)));
			data.SetRow(arrSymbol[2].get_Value().Deref().get_WordInfo().Deref().infoStart.uRow);
			data.SetCol(arrSymbol[2].get_Value().Deref().get_WordInfo().Deref().infoStart.uCol);
		}
		//set position
		ShareCom<_ICplMetaDataPositionSymbolDataUtility> spU;
		_COMPONENT_INSTANCE_INTERFACE(IGrammarSymbolData, _ICplMetaDataPositionSymbolDataUtility, arrSymbol[0].get_Value(), spU, cr);
		if( cr.IsFailed() )
			return cr;
		spU.Deref().SetPosition(pos);
		return cr;
	}

private:
	//noncopyable
	WlangDoNsBodyAction(const WlangDoNsBodyAction&) throw();
	WlangDoNsBodyAction& operator=(const WlangDoNsBodyAction&) throw();
};

DECLARE_COM_TYPECAST(WlangDoNsBodyAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
