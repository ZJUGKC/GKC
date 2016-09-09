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
#ifndef __REGEX_DOT_ALG_H__
#define __REGEX_DOT_ALG_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _RegexDotItem

class _RegexDotItem
{
public:
	_RegexDotItem() throw()
	{
	}
	_RegexDotItem(const _RegexDotItem& src) throw() : m_uIndex(src.m_uIndex), m_iter(src.m_iter)
	{
	}
	~_RegexDotItem() throw()
	{
	}

	_RegexDotItem& operator=(const _RegexDotItem& src) throw()
	{
		m_uIndex = src.m_uIndex;
		m_iter = src.m_iter;
		return *this;
	}

	bool operator==(const _RegexDotItem& right) const throw()
	{
		return m_uIndex == right.m_uIndex && m_iter == right.m_iter;
	}
	bool operator!=(const _RegexDotItem& right) const throw()
	{
		return m_uIndex != right.m_uIndex || m_iter != right.m_iter;
	}

	uintptr GetIndex() const throw()
	{
		return m_uIndex;
	}
	void SetIndex(uintptr uIndex) throw()
	{
		m_uIndex = uIndex;
	}
	const AstTree::Iterator& GetIterator() const throw()
	{
		return m_iter;
	}
	AstTree::Iterator& GetIterator() throw()
	{
		return m_iter;
	}
	void SetIterator(const AstTree::Iterator& iter) throw()
	{
		m_iter = iter;
	}

private:
	uintptr m_uIndex;  //index
	AstTree::Iterator m_iter;  //iterator for current position
};

// _RegexDotItemSet

class _RegexDotItemSet
{
public:
	_RegexDotItemSet() throw()
	{
	}
	~_RegexDotItemSet() throw()
	{
	}

	void Clear() throw()
	{
		if( !m_arr.IsBlockNull() )
			m_arr.RemoveAll();
	}

	uintptr GetCount() const throw()
	{
		return m_arr.GetCount();
	}

	const _RegexDotItem& GetItem(uintptr uIndex) const throw()
	{
		assert( uIndex < GetCount() );
		return m_arr[uIndex].get_Value();
	}
	_RegexDotItem& GetItem(uintptr uIndex) throw()
	{
		assert( uIndex < GetCount() );
		return m_arr[uIndex].get_Value();
	}

	void Add(const _RegexDotItem& item)
	{
		if( m_arr.IsBlockNull() )
			m_arr = ShareArrayHelper::MakeShareArray<_RegexDotItem>(MemoryHelper::GetCrtMemoryManager());  //may throw
		m_arr.Add(item);  //may throw
	}
	void RemoveHead() throw()
	{
		assert( GetCount() > 0 );
		m_arr.RemoveAt(0);
	}

	bool Find(const _RegexDotItem& item) const throw()
	{
		for( uintptr i = 0; i < GetCount(); i ++ ) {
			if( item == GetItem(i) )
				return true;
		}
		return false;
	}

private:
	ShareArray<_RegexDotItem> m_arr;

private:
	//noncopyable
	_RegexDotItemSet(const _RegexDotItemSet&) throw();
	_RegexDotItemSet& operator=(const _RegexDotItemSet&) throw();
};

// left leaf node

inline AstTree::Iterator _Regex_Tree_Left_Leaf_Node(const AstTree& tree, const AstTree::Iterator& iter) throw()
{
	AstTree::Iterator iterC, iterV(iter);
	while( !iterV.IsNull() ) {
		iterC = iterV;
		iterV.MoveChild();
	}
	return iterC;
}

// range sub-set

inline bool _Regex_CharRange_Is_Subset(const _RegexCharRange& ra, const _RegexCharRange& raC) throw()
{
	return ra.uLow >= raC.uLow && ra.uHigh <= raC.uHigh;
}

// the same set

inline bool _Regex_DotItemSet_Is_Same(const _RegexDotItemSet& s1, const _RegexDotItemSet& s2) throw()
{
	uintptr uCount = s1.GetCount();
	if( uCount != s2.GetCount() )
		return false;
	for( uintptr i = 0; i < uCount; i ++ ) {
		if( !s2.Find(s1.GetItem(i)) )
			return false;
	}
	return true;
}

// process to-do-set (all left nodes)

inline void _Regex_Process_ToDoSet(_RegexDotItemSet& toDoSet, _RegexDotItemSet& doneSet, _RegexDotItemSet& closureSet)
{
	while( toDoSet.GetCount() > 0 ) {
		//extract
		_RegexDotItem item(toDoSet.GetItem(0));
		toDoSet.RemoveHead();
		//check done set
		if( doneSet.Find(item) )
			continue;
		doneSet.Add(item);  //may throw
		AstTree::Iterator iter(item.GetIterator());
		//the end of regular expression
		if( iter.IsNull() ) {
			assert( !closureSet.Find(item) );
			closureSet.Add(item);  //may throw
			continue;
		}
		//type
		uint uType = iter.GetType();
		if( uType == REGEX_OP_NONE ) {
			assert( !closureSet.Find(item) );
			closureSet.Add(item);  //may throw
		}
		else if( uType == REGEX_OP_PLUS ) {
			//child
			iter.MoveChild();
			assert( !iter.IsNull() );
			uType = iter.GetType();
			//cannot be '?' or '*'
			assert( uType != REGEX_OP_QUESTION && uType != REGEX_OP_STAR );
			item.SetIterator(iter);
			toDoSet.Add(item);  //may throw
		}
		else if( uType == REGEX_OP_QUESTION || uType == REGEX_OP_STAR ) {
			//child
			iter.MoveChild();
			assert( !iter.IsNull() );
			item.SetIterator(iter);
			toDoSet.Add(item);  //may throw
		}
		else if( uType == REGEX_OP_OR ) {
			//child & next
			iter.MoveChild();
			assert( !iter.IsNull() );
			while( !iter.IsNull() ) {
				uType = iter.GetType();
				//cannot be '?' or '*'
				assert( uType != REGEX_OP_QUESTION && uType != REGEX_OP_STAR );
				item.SetIterator(iter);
				toDoSet.Add(item);  //may throw
				iter.MoveNext();
			}
		}
		else if( uType == REGEX_OP_LINK ) {
			//child
			iter.MoveChild();
			assert( !iter.IsNull() );
			//next
			while( !iter.IsNull() ) {
				item.SetIterator(iter);
				toDoSet.Add(item);  //may throw
				uType = iter.GetType();
				if( uType != REGEX_OP_QUESTION && uType != REGEX_OP_STAR )
					break;
				iter.MoveNext();
			}
			assert( !iter.IsNull() );  //failed: all nodes are null
		}
		else {
			assert( false );
		} //end if
	} //end while
}

// initial closure set

inline void _Regex_Initial_ClosureSet(const _Regex_AST& rast, _RegexDotItemSet& closureSet)
{
	for( uintptr i = 0; i < rast.GetCount(); i ++ ) {
		const AstTree& atree = rast.GetAST(i);
		_RegexDotItemSet toDoSet, doneSet;
		//the first child of root cannot be '?' or '*'
		AstTree::Iterator iterR(atree.GetRoot());
		assert( !iterR.IsNull() );
		iterR.MoveChild();
		assert( !iterR.IsNull() );
		uint uType = iterR.GetType();
		assert( uType != REGEX_OP_QUESTION && uType != REGEX_OP_STAR );
		//add
		_RegexDotItem item;
		item.SetIndex(i);
		item.SetIterator(iterR);
		toDoSet.Add(item);  //may throw
		//closure
		_Regex_Process_ToDoSet(toDoSet, doneSet, closureSet);  //may throw
	} //end for
}

// dot-item transition

inline void _Regex_DotItem_Transition(const _RegexDotItem& item, const _RegexCharRange& ra, _RegexDotItemSet& toDoSet)
{
	_RegexDotItem itemNew(item);
	AstTree::Iterator iter(item.GetIterator());
	//the end of regular expression
	if( iter.IsNull() )
		return ;
	//can move
	const _RegexLeafNode& node = iter.GetData<_RegexLeafNode>();
	_RegexCharRange rcr;
	rcr.uLow = node.get_Low();
	rcr.uHigh = node.get_High();
	if( !_Regex_CharRange_Is_Subset(ra, rcr) )
		return ;
	//loop
	while( true ) {
		//parent
		AstTree::Iterator iterP(iter);
		iterP.MoveParent();
		uint uType = iterP.GetType();
		if( uType == REGEX_OP_NONE ) {
			//the end of regular expression
			AstTree::Iterator iterNull;
			itemNew.SetIterator(iterNull);
			toDoSet.Add(itemNew);  //may throw
			break;
		}
		else if( uType == REGEX_OP_PLUS ) {
			uType = iter.GetType();
			//cannot be '?' or '*'
			assert( uType != REGEX_OP_QUESTION && uType != REGEX_OP_STAR );
			if( uType == REGEX_OP_NONE || uType == REGEX_OP_OR || uType == REGEX_OP_LINK ) {
				itemNew.SetIterator(iter);
				toDoSet.Add(itemNew);  //may throw
			}
			//next
			iter = iterP;
		}
		else if( uType == REGEX_OP_STAR ) {
			uType = iter.GetType();
			if( uType == REGEX_OP_NONE || uType == REGEX_OP_QUESTION || uType == REGEX_OP_OR || uType == REGEX_OP_LINK ) {
				itemNew.SetIterator(iter);
				toDoSet.Add(itemNew);  //may throw
			}
			//next
			iter = iterP;
		}
		else if( uType == REGEX_OP_QUESTION ) {
			//next
			iter = iterP;
		}
		else if( uType == REGEX_OP_OR ) {
			uType = iter.GetType();
			//cannot be '?' or '*'
			assert( uType != REGEX_OP_QUESTION && uType != REGEX_OP_STAR );
			//next
			iter = iterP;
		}
		else if( uType == REGEX_OP_LINK ) {
			iter.MoveNext();
			while( !iter.IsNull() ) {
				itemNew.SetIterator(iter);
				toDoSet.Add(itemNew);  //may throw
				uType = iter.GetType();
				if( uType != REGEX_OP_QUESTION && uType != REGEX_OP_STAR )
					break;
				iter.MoveNext();
			}
			//it is not the end of LINK
			if( !iter.IsNull() )
				break;
			//next
			iter = iterP;
		}
		else {
			assert( false );
		} //end if
	} //end while
}

// _RegexStateItem

class _RegexStateItem
{
public:
	_RegexStateItem() throw()
	{
	}
	~_RegexStateItem() throw()
	{
	}

	const _RegexDotItemSet& GetClosureSet() const throw()
	{
		return m_closureSet;
	}
	_RegexDotItemSet& GetClosureSet() throw()
	{
		return m_closureSet;
	}
	const _RegexCharRangeSet& GetTransitionSet() const throw()
	{
		return m_rs;
	}
	_RegexCharRangeSet& GetTransitionSet() throw()
	{
		return m_rs;
	}
	const ShareArray<uintptr>& GetNextStateArray() const throw()
	{
		return m_arrNext;
	}
	ShareArray<uintptr>& GetNextStateArray() throw()
	{
		return m_arrNext;
	}

	uintptr GetTransitionCount() const throw()
	{
		return m_rs.GetCount();
	}

	//transition set
	void CalcTransitionSet()
	{
		for( uintptr i = 0; i < m_closureSet.GetCount(); i ++ ) {
			const _RegexDotItem& item = m_closureSet.GetItem(i);
			const AstTree::Iterator& iter = item.GetIterator();
			if( iter.IsNull() )
				continue;
			const _RegexLeafNode& node = iter.GetData<_RegexLeafNode>();
			_RegexCharRange rcr;
			rcr.uLow = node.get_Low();
			rcr.uHigh = node.get_High();
			m_rs.AddRangeBySplit(rcr);  //may throw
		}
	}
	//next states
	void InitNextStateArray()
	{
		if( m_arrNext.IsBlockNull() )
			m_arrNext = ShareArrayHelper::MakeShareArray<uintptr>(MemoryHelper::GetCrtMemoryManager());  //may throw
		uintptr uCount = m_rs.GetCount();
		if( uCount == 0 ) {
			m_arrNext.RemoveAll();
			return ;
		}
		m_arrNext.SetCount(uCount, 0);  //may throw
	}
	//transition closure set
	void CalcTransitionClosureSet(const _RegexCharRange& rcr, _RegexDotItemSet& nextSet)
	{
		//for transition
		ShareArray<uintptr> arrIndex(ShareArrayHelper::MakeShareArray<uintptr>(MemoryHelper::GetCrtMemoryManager()));  //may throw
		ShareArray<_RegexDotItemSet> arrDoneSet(ShareArrayHelper::MakeShareArray<_RegexDotItemSet>(MemoryHelper::GetCrtMemoryManager()));  //may throw
		for( uintptr i = 0; i < m_closureSet.GetCount(); i ++ ) {
			//each dot-item
			const _RegexDotItem& item = m_closureSet.GetItem(i);
			uintptr uItemIndex = item.GetIndex();
			RefPtr<_RegexDotItemSet> doneSet;
			uintptr uFind = _find_index_array(arrIndex, uItemIndex);
			if( uFind == (uintptr)-1 ) {
				arrIndex.Add(uItemIndex);  //may throw
				arrDoneSet.Add();  //may throw
				uFind = arrDoneSet.GetCount() - 1;
			}
			doneSet = RefPtr<_RegexDotItemSet>(arrDoneSet[uFind].get_Value());
			_RegexDotItemSet toDoSet;
			_Regex_DotItem_Transition(item, rcr, toDoSet);  //may throw
			_Regex_Process_ToDoSet(toDoSet, doneSet.Deref(), nextSet);  //may throw
		} //end for
	}

	//get matching index
	uintptr GetMatchIndex() const throw()
	{
		for( uintptr i = 0; i < m_closureSet.GetCount(); i ++ ) {
			//each dot-item
			const _RegexDotItem& item = m_closureSet.GetItem(i);
			if( item.GetIterator().IsNull() )
				return item.GetIndex();
		}
		return (uintptr)-1;
	}

private:
	//find
	static uintptr _find_index_array(const ShareArray<uintptr>& arr, uintptr index) throw()
	{
		for( uintptr i = 0; i < arr.GetCount(); i ++ ) {
			if( arr[i].get_Value() == index )
				return i;
		}
		return (uintptr)-1;
	}

private:
	_RegexDotItemSet     m_closureSet;  //closure set
	_RegexCharRangeSet   m_rs;          //transition set
	ShareArray<uintptr>  m_arrNext;     //next states

private:
	//noncopyable
	_RegexStateItem(const _RegexStateItem&) throw();
	_RegexStateItem& operator=(const _RegexStateItem&) throw();
};

// _RegexStateSet

class _RegexStateSet
{
private:
	typedef List<_RegexStateItem>  _RegexStateList;

public:
	_RegexStateSet() throw() : m_pool(MemoryHelper::GetCrtMemoryManager())
	{
	}
	~_RegexStateSet() throw()
	{
	}

	//generate
	//  called only once
	void Generate(const _Regex_AST& rast)
	{
		if( m_arrState.IsBlockNull() )
			m_arrState = ShareArrayHelper::MakeShareArray<_RegexStateList::Iterator>(MemoryHelper::GetCrtMemoryManager());  //may throw
		//to-do
		ShareArray<_RegexStateList::Iterator> arrToDo(ShareArrayHelper::MakeShareArray<_RegexStateList::Iterator>(MemoryHelper::GetCrtMemoryManager()));  //may throw
		//initial
		{
			auto iter(m_pool.AddTail());  //may throw
			m_arrState.Add(iter);  //may throw
			_Regex_Initial_ClosureSet(rast, iter.get_Value().GetClosureSet());  //may throw
			arrToDo.Add(iter);  //may throw
		} //end block
		//loop
		while( arrToDo.GetCount() > 0 ) {
			//extract
			_RegexStateList::Iterator iterC(arrToDo[0].get_Value());
			arrToDo.RemoveAt(0);
			//move
			_RegexStateItem& itemState = iterC.get_Value();
			itemState.CalcTransitionSet();  //may throw
			itemState.InitNextStateArray();  //may throw
			//moving count
			uintptr uCount = itemState.GetTransitionCount();
			if( uCount == 0 )
				continue;
			ShareArray<uintptr>& arrNext = itemState.GetNextStateArray();
			//transition
			for( uintptr i = 0; i < uCount; i ++ ) {
				const _RegexCharRange& rcr = itemState.GetTransitionSet().GetRange(i);
				auto iterList(m_pool.AddTail());  //may throw
				_RegexDotItemSet& nextSet = iterList.get_Value().GetClosureSet();
				itemState.CalcTransitionClosureSet(rcr, nextSet);  //may throw
				if( nextSet.GetCount() == 0 ) {
					m_pool.RemoveAt(iterList);
					continue;
				}
				//check states
				uintptr uFind = _find_state_array(nextSet);
				if( uFind == (uintptr)-1 ) {
					m_arrState.Add(iterList);  //may throw
					arrToDo.Add(iterList);  //may throw
					arrNext[i].set_Value(m_arrState.GetCount() - 1);
				}
				else {
					arrNext[i].set_Value(uFind);
					m_pool.RemoveAt(iterList);
				}
			} //end for
		} //end while
	}

	uintptr GetCount() const throw()
	{
		return m_arrState.GetCount();
	}
	const _RegexStateItem& GetState(uintptr index) const throw()
	{
		assert( index < GetCount() );
		return m_arrState[index].get_Value().get_Value();
	}

private:
	//find
	uintptr _find_state_array(const _RegexDotItemSet& s) const throw()
	{
		for( uintptr i = 0; i < m_arrState.GetCount(); i ++ ) {
			if( _Regex_DotItemSet_Is_Same(s, m_arrState[i].get_Value().get_Value().GetClosureSet()) )
				return i;
		}
		return (uintptr)-1;
	}

private:
	_RegexStateList m_pool;
	ShareArray<_RegexStateList::Iterator> m_arrState;

private:
	//noncopyable
	_RegexStateSet(const _RegexStateSet&) throw();
	_RegexStateSet& operator=(const _RegexStateSet&) throw();
};

// generate table

inline void _Regex_Generate_Tables(const _Regex_AST& rast, FsaTable& table)
{
	//generate
	_RegexStateSet rss;
	rss.Generate(rast);  //may throw

	//check
	assert( rss.GetCount() < (uintptr)(Limits<int>::Max - 1) );  //with last NULL item

	int iMaxStateNo = (int)rss.GetCount();
	int iMaxMatchNo = (int)rast.GetCount();

	//transition
	ShareArray<int> arrTransitionNum(ShareArrayHelper::MakeShareArray<int>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	arrTransitionNum.SetCount(iMaxStateNo + 1, 0);  //may throw
	arrTransitionNum[0].set_Value(0);
	for( int i = 1; i <= iMaxStateNo; i ++ ) {
		uintptr uCount = rss.GetState(i - 1).GetTransitionCount();
		assert( uCount < (uintptr)(Limits<int>::Max) );  //with last NULL item
		arrTransitionNum[i].set_Value((int)uCount);
	}

	//table
	ConstArray<int> arrC;
	ConstArrayHelper::SetInternalPointer(ShareArrayHelper::GetInternalPointer(arrTransitionNum), arrTransitionNum.GetCount(), arrC);
	table.Allocate(iMaxStateNo, arrC, iMaxMatchNo);  //may throw
	for( int i = 1; i <= iMaxStateNo; i ++ ) {
		const _RegexCharRangeSet& rs = rss.GetState(i - 1).GetTransitionSet();
		const ShareArray<uintptr>& arrNext = rss.GetState(i - 1).GetNextStateArray();
		uintptr uMatch = rss.GetState(i - 1).GetMatchIndex();
		table.SetState(i, FSA_STATE_STOP, uMatch == (uintptr)-1 ? 100 : -((int)(uMatch + 1)));
		for( int j = 0; j < arrTransitionNum[i].get_Value(); j ++ ) {
			const _RegexCharRange& rcr = rs.GetRange(j);
			table.SetTransition(i, j, rcr.uLow, rcr.uHigh, (int)(arrNext[j].get_Value() + 1));
		}
	}
	for( int i = 1; i <= iMaxMatchNo; i ++ )
		table.SetMatch(i, i);
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
