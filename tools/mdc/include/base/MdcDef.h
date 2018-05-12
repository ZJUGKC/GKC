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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __MDC_DEF_H__
#define __MDC_DEF_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _IHtmlGenerator

class NOVTABLE _IHtmlGenerator
{
public:
	virtual CallResult Generate(bool bOpen, ShareCom<ICplMetaData>& spMeta, const CplMetaDataPosition& posData, ShareCom<ITextStream>& spText) = 0;
};

// _GH_Utility

class _GH_Utility
{
public:
	_GH_Utility() throw() : m_map(MemoryHelper::GetCrtMemoryManager())
	{
	}
	~_GH_Utility() throw()
	{
	}

	void Initialize()
	{
		assert( m_map.IsEmpty() );
		//comment
		{
			SharePtr<_CommentGenerator> spCG(SharePtrHelper::MakeSharePtr<_CommentGenerator>(MemoryHelper::GetCrtMemoryManager()));  //may throw
			m_map.InsertWithoutFind(WMARK_NOUN_COMMENT, SharePtrHelper::TypeCast<_CommentGenerator, _IHtmlGenerator>(spCG));  //may throw
		} //end block
		//indent
		{
			SharePtr<_IndentGenerator> spCG(SharePtrHelper::MakeSharePtr<_IndentGenerator>(MemoryHelper::GetCrtMemoryManager()));  //may throw
			m_map.InsertWithoutFind(WMARK_NOUN_INDENT, SharePtrHelper::TypeCast<_IndentGenerator, _IHtmlGenerator>(spCG));  //may throw
		} //end block
	}

	SharePtr<_IHtmlGenerator> Find(uint uType) const throw()
	{
		auto iter(m_map.Find(uType));
		if( iter == m_map.GetEnd() )
			return SharePtr<_IHtmlGenerator>();
		return iter.get_Value();
	}

private:
	RBMap<uint, SharePtr<_IHtmlGenerator>> m_map;

private:
	_GH_Utility(const _GH_Utility&) throw();
	_GH_Utility& operator=(const _GH_Utility&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
