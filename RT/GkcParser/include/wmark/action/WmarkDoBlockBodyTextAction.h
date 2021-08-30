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
This file contains component class of Do-Block-Body-Text action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WMARK_DO_BLOCK_BODY_TEXT_ACTION_H__
#define __WMARK_DO_BLOCK_BODY_TEXT_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkDoBlockBodyTextAction

class WmarkDoBlockBodyTextAction : public IGrammarAction,
								public _CplMetaDataActionBase
{
public:
	WmarkDoBlockBodyTextAction() throw()
	{
	}
	~WmarkDoBlockBodyTextAction() throw()
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
	//do action
	CallResult do_action(ShareArray<ShareCom<IGrammarSymbolData>>& arrSymbol, ShareArray<StringS>& errorArray)
	{
		//text
		CallResult cr;
		StringA str(arrSymbol[1].Deref().get_Buffer().Deref().ToUTF8());  //may throw
		uintptr uLength = str.GetLength();
		if( uLength > (uintptr)Limits<uint>::Max - 1 ) {
			cr.SetResult(SystemCallResults::Overflow);
			generate_error(cr, errorArray);  //may throw
			return cr;
		}
		CplMetaDataPosition pos;
		cr = m_spMeta.Deref().InsertAstNode(WMARK_NOUN_TEXT, pos);
		if( cr.IsFailed() ) {
			generate_error(cr, errorArray);  //may throw
			return cr;
		}
		CplMetaDataPosition posData;
		cr = m_spMeta.Deref().InsertData((uint)(uLength + 1), posData);
		if( cr.IsFailed() ) {
			generate_error(cr, errorArray);  //may throw
			return cr;
		}
		mem_copy(ShareArrayHelper::GetInternalPointer(str), uLength + 1, (void*)m_spMeta.Deref().GetData(posData));
		m_spMeta.Deref().SetAstData(pos, posData);
		//set position
		ShareCom<_ICplMetaDataPositionSymbolDataUtility> spU;
		_COMPONENT_INSTANCE_INTERFACE(IGrammarSymbolData, _ICplMetaDataPositionSymbolDataUtility, arrSymbol[0], spU, cr);
		if( cr.IsFailed() )
			return cr;
		spU.Deref().SetPosition(pos);
		return cr;
	}

	static void generate_error(const CallResult& cr, ShareArray<StringS>& errorArray)
	{
		StringS strError(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
		_CplErrorStringHelper::GenerateError(cr, strError);  //may throw
		errorArray.Add(strError);  //may throw
	}

private:
	//noncopyable
	WmarkDoBlockBodyTextAction(const WmarkDoBlockBodyTextAction&) throw();
	WmarkDoBlockBodyTextAction& operator=(const WmarkDoBlockBodyTextAction&) throw();
};

DECLARE_COM_TYPECAST(WmarkDoBlockBodyTextAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
