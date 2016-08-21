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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_DEF_H__
#define __REGEX_DEF_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _RegexCharRange

struct _RegexCharRange
{
	uint uLow;
	uint uHigh;
};

// _RegexCharRangeSet

class _RegexCharRangeSet
{
public:
	_RegexCharRangeSet() throw()
	{
	}
	~_RegexCharRangeSet() throw()
	{
	}

	//add
	void AddRange(const _RegexCharRange& rcr)
	{
		//init
		if( m_arr.IsBlockNull() )
			m_arr = ShareArrayHelper::MakeShareArray<_RegexCharRange>(MemoryHelper::GetCrtMemoryManager());  //may throw
		//add
		if( m_arr.GetCount() == 0 ) {
			m_arr.Add(rcr);  //may throw
			return ;
		}
		//check ranges
		uintptr index = 0;
		while( index < m_arr.GetCount() ) {
			_RegexCharRange& rcrC = m_arr[index].get_Value();
			//left
			if( rcr.uHigh < rcrC.uLow - 1 )
				break;
			//overlapped
			if( rcr.uHigh <= rcrC.uHigh ) {
				//combine
				if( rcrC.uLow > rcr.uLow )
					rcrC.uLow = rcr.uLow;
				return ;
			}
			//extended
			if( rcr.uLow <= rcrC.uHigh + 1 ) {
				//combine
				if( rcrC.uLow > rcr.uLow )
					rcrC.uLow = rcr.uLow;
				rcrC.uHigh = rcr.uHigh;
				//next
				uintptr indexNext = index + 1;
				while( indexNext < m_arr.GetCount() ) {
					_RegexCharRange& rcrCurrent = m_arr[index].get_Value();
					_RegexCharRange& rcrNext = m_arr[indexNext].get_Value();
					if( rcrCurrent.uHigh < rcrNext.uLow - 1 )
						break;
					//combine
					if( rcrCurrent.uHigh < rcrNext.uHigh )
						rcrCurrent.uHigh = rcrNext.uHigh;
					m_arr.RemoveAt(indexNext);
				}
				return ;
			}
			//right
			index ++;
		} //end while
		//insert
		m_arr.InsertAt(index, 1, rcr);  //may throw
	}

	//complement set
	void CalcComplement(_RegexCharRangeSet& rs) const
	{
		if( rs.m_arr.IsBlockNull() )
			rs.m_arr = ShareArrayHelper::MakeShareArray<_RegexCharRange>(MemoryHelper::GetCrtMemoryManager());  //may throw
		//clear
		rs.m_arr.RemoveAll();
		//empty
		const uint c_Max = 255;
		uint uLeft = 0;
		_RegexCharRange rcrNow;
		uintptr uCount = GetCount();
		if( uCount == 0 ) {
			rcrNow.uLow = uLeft + 1;
			rcrNow.uHigh = c_Max;
			rs.m_arr.Add(rcrNow);  //may throw
			return ;
		}
		//loop
		for( uintptr i = 0; i < uCount; i ++ ) {
			const _RegexCharRange& rcr = GetRange(i);
			if( rcr.uLow <= 1 ) {
				uLeft = rcr.uHigh;
				continue;
			}
			rcrNow.uLow = uLeft + 1;
			rcrNow.uHigh = rcr.uLow - 1;
			rs.m_arr.Add(rcrNow);  //may throw
			uLeft = rcr.uHigh;
		}
		//last
		if( uLeft < c_Max ) {
			rcrNow.uLow = uLeft + 1;
			rcrNow.uHigh = c_Max;
			rs.m_arr.Add(rcrNow);  //may throw
		}
	}

	//move
	void MoveTo(_RegexCharRangeSet& rs) throw()
	{
		rs.m_arr = rv_forward(m_arr);
	}

	uintptr GetCount() const throw()
	{
		return m_arr.GetCount();
	}
	const _RegexCharRange& GetRange(uintptr uIndex) const throw()
	{
		assert( uIndex < GetCount() );
		return m_arr[uIndex].get_Value();
	}
	_RegexCharRange& GetRange(uintptr uIndex) throw()
	{
		assert( uIndex < GetCount() );
		return m_arr[uIndex].get_Value();
	}

private:
	ShareArray<_RegexCharRange> m_arr;

private:
	//noncopyable
	_RegexCharRangeSet(const _RegexCharRangeSet&) throw();
	_RegexCharRangeSet& operator=(const _RegexCharRangeSet&) throw();
};

// _I_RegexCharSymbolData_Utility

class NOVTABLE _I_RegexCharSymbolData_Utility
{
public:
	virtual void GetCharRange(_RegexCharRange& rcr) throw() = 0;
	virtual void SetCharRange(const _RegexCharRange& rcr) throw() = 0;
};

DECLARE_GUID(GUID__I_RegexCharSymbolData_Utility)

// _I_RegexCharSetSymbolData_Utility

class NOVTABLE _I_RegexCharSetSymbolData_Utility
{
public:
	virtual RefPtr<_RegexCharRangeSet> GetCharRangeSet() throw() = 0;
};

DECLARE_GUID(GUID__I_RegexCharSetSymbolData_Utility)

// _I_RegexPositionSymbolData_Utility

class NOVTABLE _I_RegexPositionSymbolData_Utility
{
public:
	virtual void SetPosition(const AstTree::Position& pos) throw() = 0;
	virtual AstTree::Position GetPosition() throw() = 0;
};

DECLARE_GUID(GUID__I_RegexPositionSymbolData_Utility)

// _I_RegexAstAction_Utility

class NOVTABLE _I_RegexAstAction_Utility
{
public:
	virtual void SetAST(const RefPtr<AstTree>& tree) throw() = 0;
	virtual RefPtr<AstTree> GetAST() throw() = 0;
};

DECLARE_GUID(GUID__I_RegexAstAction_Utility)

//regex operators

enum {
	REGEX_OP_NONE,
	REGEX_OP_LINK,
	REGEX_OP_OR,
	REGEX_OP_QUESTION,
	REGEX_OP_STAR,
	REGEX_OP_PLUS,
	REGEX_OP_MAX
};

//regex leaf node

#pragma pack(push, 1)

class _RegexLeafNode
{
public:
	_RegexLeafNode() throw()
	{
	}
	~_RegexLeafNode() throw()
	{
	}

	uint get_Low() const throw()
	{
		return m_uLow.get_Value();
	}
	void set_Low(uint uLow) throw()
	{
		m_uLow.set_Value(uLow);
	}
	uint get_High() const throw()
	{
		return m_uHigh.get_Value();
	}
	void set_High(uint uHigh) throw()
	{
		m_uHigh.set_Value(uHigh);
	}

private:
	BeType<uint> m_uLow;
	BeType<uint> m_uHigh;

private:
	//noncopyable
	_RegexLeafNode(const _RegexLeafNode&) throw();
	_RegexLeafNode& operator=(const _RegexLeafNode&) throw();
};

#pragma pack(pop)

// _RegexCharRangeSet_To_AST

inline AstTree::Iterator _RegexCharRangeSet_To_AST(const _RegexCharRangeSet& rs, AstTree& tree)
{
	uintptr uCount = rs.GetCount();
	assert( uCount > 0 );
	if( uCount == 1 ) {
		//only a leaf
		const _RegexCharRange& rcr = rs.GetRange(0);
		auto iterLeaf(tree.CreateNode(sizeof(_RegexLeafNode), REGEX_OP_NONE));  //may throw
		_RegexLeafNode& node = iterLeaf.GetData<_RegexLeafNode>();
		node.set_Low(rcr.uLow);
		node.set_High(rcr.uHigh);
		return iterLeaf;
	}
	//OR tree
	AstTree::Iterator iterOP(tree.CreateNode(0, REGEX_OP_OR));  //may throw
	AstTree::Iterator iterLast(tree.GetNullIterator());
	for( uintptr i = 0; i < uCount; i ++ ) {
		const _RegexCharRange& rcr = rs.GetRange(i);
		auto iterLeaf(tree.CreateNode(sizeof(_RegexLeafNode), REGEX_OP_NONE));  //may throw
		_RegexLeafNode& node = iterLeaf.GetData<_RegexLeafNode>();
		node.set_Low(rcr.uLow);
		node.set_High(rcr.uHigh);
		tree.SetParent(iterLeaf, iterOP);
		if( iterLast.IsNull() )
			tree.SetChild(iterOP, iterLeaf);
		else
			tree.SetNext(iterLast, iterLeaf);
		iterLast = iterLeaf;
	} //end for
	return iterOP;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
