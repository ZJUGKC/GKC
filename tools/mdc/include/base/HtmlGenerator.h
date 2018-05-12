/*
** Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
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
This file contains classes for html generator.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __HTML_GENERATOR_H__
#define __HTML_GENERATOR_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _gh_get_root

inline CplMetaDataPosition _gh_get_root(ShareCom<ICplMetaData>& spMeta) throw()
{
	CplMetaDataPosition pos;
	pos = spMeta.Deref().Find(DECLARE_TEMP_CONST_STRING(ConstStringA, WMARK_ROOT_SYMBOL));
	if( pos.IsNull() )
		return pos;
	CplMetaDataInfo info;
	bool bAnalysis;
	spMeta.Deref().GetInfo(pos, info, bAnalysis);
	if( info.posData.IsNull() )
		return info.posData;
	//root
	pos = spMeta.Deref().GetAstRoot(info.posData);
	return pos;
}

// _Generate_Html

inline CallResult _Generate_Html(ShareCom<ICplMetaData>& spMeta, ShareCom<ITextStream>& spText) throw()
{
	CallResult cr;
	try {
		//map
		_HG_Utility hgu;
		hgu.Initialize();  //may throw

		//root
		CplMetaDataPosition posRoot(_gh_get_root(spMeta));
		if( posRoot.IsNull() ) {
			cr.SetResult(SystemCallResults::Corrupt);
			return cr;
		}
		CplAstNodeInfo astInfo;
		spMeta.Deref().GetAstNodeInfo(posRoot, astInfo);
		//empty
		if( astInfo.posChild.IsNull() )
			return cr;

		//traverse
		ShareArray<CplMetaDataPosition> arrStack(ShareArrayHelper::MakeShareArray<CplMetaDataPosition>(MemoryHelper::GetCrtMemoryManager()));  //may throw
		arrStack.Add(astInfo.posChild);  //may throw
		uintptr uCount;
		//loop
		while( (uCount = arrStack.GetCount()) > 0 ) {
			CplMetaDataPosition pos;
			//pop
			pos = arrStack.GetAt(uCount - 1).get_Value();
			arrStack.RemoveAt(uCount - 1);
			//links
			spMeta.Deref().GetAstNodeInfo(pos, astInfo);
			//output open tag
			SharePtr<_IHtmlGenerator> spG(hgu.Find(astInfo.uType));
			if( !spG.IsBlockNull() ) {
				cr = spG.Deref().Generate(true, spMeta, astInfo.posData, spText);  //may throw
				if( cr.IsFailed() )
					return cr;
			}
			//the end of link
			if( astInfo.posNext.IsNull() && astInfo.posChild.IsNull() ) {
				pos = astInfo.posParent;
				while( pos != posRoot ) {
					spMeta.Deref().GetAstNodeInfo(pos, astInfo);
					//output close tag
					spG = hgu.Find(astInfo.uType);
					if( !spG.IsBlockNull() ) {
						cr = spG.Deref().Generate(false, spMeta, astInfo.posData, spText);  //may throw
						if( cr.IsFailed() )
							return cr;
					}
					if( !astInfo.posNext.IsNull() )
						break;
					pos = astInfo.posParent;
				} //end while
				continue;
			}
			if( !astInfo.posNext.IsNull() )
				arrStack.Add(astInfo.posNext);  //may throw
			if( !astInfo.posChild.IsNull() )
				arrStack.Add(astInfo.posChild);  //may throw
		} //end while
	}
	catch(Exception& e) {
		cr = e.GetResult();
	}
	catch(...) {
		cr.SetResult(SystemCallResults::Fail);
	}
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
