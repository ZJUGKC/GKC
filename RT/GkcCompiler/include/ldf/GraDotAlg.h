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
#ifndef __GRA_DOT_ALG_H__
#define __GRA_DOT_ALG_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _GraDotItem

class _GraDotItem
{
public:
	//first : the state No., second : index of seed item
	typedef Pair<uintptr, uintptr>  pairClass;

public:
	_GraDotItem() throw() : m_uRuleNo(0), m_uDotPos(1)
	{
	}
	_GraDotItem(const _GraDotItem& src) throw() : m_uRuleNo(src.m_uRuleNo), m_uDotPos(src.m_uDotPos), m_tsLookAhead(src.m_tsLookAhead), m_arrPropagation(src.m_arrPropagation)
	{
	}
	_GraDotItem(_GraDotItem&& src) throw() : m_uRuleNo(src.m_uRuleNo), m_uDotPos(src.m_uDotPos), m_tsLookAhead(rv_forward(src.m_tsLookAhead)), m_arrPropagation(rv_forward(src.m_arrPropagation))
	{
		src.m_uRuleNo = 0;
		src.m_uDotPos = 1;
	}
	~_GraDotItem() throw()
	{
	}

	_GraDotItem& operator=(const _GraDotItem& src) throw()
	{
		m_uRuleNo = src.m_uRuleNo;
		m_uDotPos = src.m_uDotPos;
		m_tsLookAhead = src.m_tsLookAhead;
		m_arrPropagation = src.m_arrPropagation;
		return *this;
	}
	_GraDotItem& operator=(_GraDotItem&& src) throw()
	{
		if( this != &src ) {
			m_uRuleNo = src.m_uRuleNo;
			src.m_uRuleNo = 0;
			m_uDotPos = src.m_uDotPos;
			src.m_uDotPos = 1;
		}
		m_tsLookAhead = rv_forward(src.m_tsLookAhead);
		m_arrPropagation = rv_forward(src.m_arrPropagation);
		return *this;
	}

	bool operator==(const _GraDotItem& right) const throw()
	{
		return m_uRuleNo == right.m_uRuleNo && m_uDotPos == right.m_uDotPos
				&& is_same_look_ahead(m_tsLookAhead, right.m_tsLookAhead);
	}

	//rule no
	uintptr GetRuleNo() const throw()
	{
		return m_uRuleNo;
	}
	void SetRuleNo(uintptr index) throw()
	{
		m_uRuleNo = index;
	}

	//dot position
	uintptr GetDotPos() const throw()
	{
		return m_uDotPos;
	}
	void SetDotPos(uintptr uPos) throw()
	{
		m_uDotPos = uPos;
	}

	//look ahead
	const _GraTokenSet& GetLookAhead() const throw()
	{
		return m_tsLookAhead;
	}
	void SetLookAhead(const _GraTokenSet& ts) throw()
	{
		m_tsLookAhead = ts;
	}

	uintptr GetLookAheadCount() const throw()
	{
		return m_tsLookAhead.GetCount();
	}
	const _GraToken& GetLookAheadAt(uintptr index) const throw()
	{
		assert( index < GetLookAheadCount() );
		return m_tsLookAhead.GetToken(index);
	}
	uintptr FindLookAhead(const _GraToken& tk) const throw()
	{
		return m_tsLookAhead.Find(tk);
	}
	void AddLookAhead(const _GraToken& tk)
	{
		m_tsLookAhead.Add(tk);  //may throw
	}
	uintptr CombineLookAhead(const _GraDotItem& item)
	{
		return combine_look_ahead(item.m_tsLookAhead);  //may throw
	}

	//propagation
	const ShareArray<pairClass>& GetPropagation() const throw()
	{
		return m_arrPropagation;
	}
	void SetPropagation(const ShareArray<pairClass>& arr) throw()
	{
		m_arrPropagation = arr;
	}

	uintptr GetPropagationCount() const throw()
	{
		return m_arrPropagation.GetCount();
	}
	void GetPropagationAt(uintptr index, uintptr& uState, uintptr& uSeed) const throw()
	{
		assert( index < GetPropagationCount() );
		const pairClass& pair = m_arrPropagation[index].get_Value();
		uState = pair.get_First();
		uSeed  = pair.get_Second();
	}
	uintptr FindPropagation(uintptr uState, uintptr uSeed) const throw()
	{
		return ArrayUtilHelper::Find<pairClass, ShareArray<pairClass>>(m_arrPropagation, pairClass(uState, uSeed)).GetIndex();
	}
	void AddPropagation(uintptr uState, uintptr uSeed)
	{
		if( m_arrPropagation.IsBlockNull() )
			m_arrPropagation = ShareArrayHelper::MakeShareArray<pairClass>(MemoryHelper::GetCrtMemoryManager());  //may throw
		auto iter(m_arrPropagation.Add());  //may throw
		iter.get_Value().get_First() = uState;
		iter.get_Value().get_Second() = uSeed;
	}
	void CombinePropagation(const _GraDotItem& item)
	{
		combine_propagation(item.m_arrPropagation);  //may throw
	}
	void ReleasePropagation() throw()
	{
		m_arrPropagation.Release();
	}

private:
	static bool is_same_look_ahead(const _GraTokenSet& ts1, const _GraTokenSet& ts2) throw()
	{
		uintptr uCount = ts1.GetCount();
		if( uCount != ts2.GetCount() )
			return false;
		for( uintptr i = 0; i < uCount; i ++ ) {
			if( ts2.Find(ts1.GetToken(i)) == INVALID_ARRAY_INDEX )
				return false;
		}
		return true;
	}

	//combine
	uintptr combine_look_ahead(const _GraTokenSet& ts)
	{
		uintptr k = 0;
		for( uintptr i = 0; i < ts.GetCount(); i ++ ) {
			if( m_tsLookAhead.AddUniqueToken(ts.GetToken(i)) )  //may throw
				k ++;
		}
		return k;
	}
	void combine_propagation(const ShareArray<pairClass>& arr)
	{
		for( uintptr i = 0; i < arr.GetCount(); i ++ ) {
			const pairClass& pair = arr[i].get_Value();
			uintptr uState = pair.get_First();
			uintptr uSeed = pair.get_Second();
			if( FindPropagation(uState, uSeed) == INVALID_ARRAY_INDEX )
				AddPropagation(uState, uSeed);  //may throw
		}
	}

private:
	uintptr m_uRuleNo;  //rule No.
	uintptr m_uDotPos;  //dot position
	_GraTokenSet m_tsLookAhead;  //look ahead symbols
	ShareArray<pairClass> m_arrPropagation;  //propagation indices from seed items
};

// _GraDotItemSet

class _GraDotItemSet
{
public:
	_GraDotItemSet() throw()
	{
	}
	~_GraDotItemSet() throw()
	{
	}

	uintptr GetCount() const throw()
	{
		return m_arr.GetCount();
	}
	const _GraDotItem& GetItem(uintptr index) const throw()
	{
		assert( index < GetCount() );
		return m_arr[index].get_Value();
	}
	_GraDotItem& GetItem(uintptr index) throw()
	{
		assert( index < GetCount() );
		return m_arr[index].get_Value();
	}

	//find
	uintptr Find(const _GraDotItem& item) const throw()
	{
		return ArrayUtilHelper::Find<_GraDotItem, ShareArray<_GraDotItem>>(m_arr, item).GetIndex();
	}
	uintptr Find2(const _GraDotItem& item) const throw()
	{
		return find_item_array(item);
	}

	//add
	_GraDotItem& Add()
	{
		if( m_arr.IsBlockNull() )
			m_arr = ShareArrayHelper::MakeShareArray<_GraDotItem>(MemoryHelper::GetCrtMemoryManager());  //may throw
		return m_arr.Add().get_Value();  //may throw
	}

	//combine for closure set
	void Combine(const _GraDotItemSet& dis)
	{
		for( uintptr i = 0; i < dis.GetCount(); i ++ ) {
			const _GraDotItem& item = dis.GetItem(i);
			assert( item.GetLookAheadCount() != 0 || item.GetPropagationCount() != 0 );
			//find
			uintptr uIndex = find_item_array(item);
			RefPtr<_GraDotItem> rItem;
			if( uIndex != INVALID_ARRAY_INDEX ) {
				rItem = RefPtr<_GraDotItem>(m_arr[uIndex].get_Value());
			}
			else {
				//add
				rItem = RefPtr<_GraDotItem>(Add());  //may throw
				rItem.Deref().SetRuleNo(item.GetRuleNo());
				rItem.Deref().SetDotPos(item.GetDotPos());
			}
			rItem.Deref().CombineLookAhead(item);  //may throw
			rItem.Deref().CombinePropagation(item);  //may throw
		}
	}
	//the same set
	bool IsSameSet(const _GraDotItemSet& dis) const throw()
	{
		uintptr uCount = dis.GetCount();
		if( uCount != m_arr.GetCount() )
			return false;
		for( uintptr i = 0; i < uCount; i ++ ) {
			const _GraDotItem& item = dis.GetItem(i);
			if( find_item_array(item) == INVALID_ARRAY_INDEX )
				return false;
		}
		return true;
	}

private:
	//trait
	class _dot_item_trait
	{
	public:
		static bool IsEQ(const _GraDotItem& d1, const _GraDotItem& d2) throw()
		{
			return d1.GetRuleNo() == d2.GetRuleNo() && d1.GetDotPos() == d2.GetDotPos();
		}
	};

	//find
	uintptr find_item_array(const _GraDotItem& item) const throw()
	{
		return ArrayUtilHelper::Find<_GraDotItem, ShareArray<_GraDotItem>, _dot_item_trait>(m_arr, item).GetIndex();
	}

private:
	ShareArray<_GraDotItem> m_arr;

private:
	//noncopyable
	_GraDotItemSet(const _GraDotItemSet&) throw();
	_GraDotItemSet& operator=(const _GraDotItemSet&) throw();
};

// first set

class _GraFirstSetTable
{
private:
	typedef RBMap<_GraToken, _GraTokenSet>  mapClass;

public:
	_GraFirstSetTable() throw() : m_map(MemoryHelper::GetCrtMemoryManager())
	{
	}
	~_GraFirstSetTable() throw()
	{
	}

	//generate table
	void Generate(uint uTerminalMaxID, const _GraRuleSet& rs, const TokenTable& nt)
	{
		//nonterminal
		for( uint i = nt.GetMinID(); i <= nt.GetMaxID(); i ++ ) {
			_GraToken tk;
			tk.SetID(i);
			auto iter(m_map.InsertWithoutFind(tk));  //may throw
			_GraTokenSet& ts = iter.get_Value().get_Second();
			calc_first(uTerminalMaxID, rs, tk, ts);  //may throw
			//the terminal tokens must be deduced from production rules
			assert( ts.GetCount() != 0 );
		} //end for
	}

	//find
	const _GraTokenSet& Find(uint uID) const throw()
	{
		_GraToken tk;
		tk.SetID(uID);
		auto iter(m_map.Find(tk));
		assert( !(iter.GetPosition().IsNull()) );
		return iter.get_Value().get_Second();
	}

private:
	static void calc_first(uint uTerminalMaxID, const _GraRuleSet& rs, const _GraToken& tk, _GraTokenSet& ts)
	{
		ShareArray<_GraToken> arrToDo(ShareArrayHelper::MakeShareArray<_GraToken>(MemoryHelper::GetCrtMemoryManager()));  //may throw
		arrToDo.Add(tk);  //may throw
		//loop
		_GraTokenSet tsDone;
		while( arrToDo.GetCount() > 0 ) {
			_GraToken tkc(arrToDo[0].get_Value());
			arrToDo.RemoveAt(0);
			//done
			tsDone.Add(tkc);  //may throw
			//rules
			for( uintptr i = 0; i < rs.GetCount(); i ++ ) {
				//left part
				if( rs.GetRule(i).GetToken(0) == tkc ) {
					const _GraToken& tka = rs.GetRule(i).GetToken(1);
					//terminal
					if( tka.GetID() <= uTerminalMaxID ) {
						ts.AddUniqueToken(tka);  //may throw
					}
					else {
						if( tsDone.Find(tka) == INVALID_ARRAY_INDEX ) {
							if( !(ArrayUtilHelper::Find<_GraToken, ShareArray<_GraToken>>(arrToDo, tka).IsValid()) )
								arrToDo.Add(tka);  //may throw
						}
					}
				}
			} //end for
		} //end while
	}

private:
	mapClass m_map;

private:
	//noncopyable
	_GraFirstSetTable(const _GraFirstSetTable&) throw();
	_GraFirstSetTable& operator=(const _GraFirstSetTable&) throw();
};

inline _GraTokenSet _Gra_Calc_After_NT_First(uint uTerminalMaxID, const _GraRuleSet& rs, uintptr uRuleNo, uintptr uDotPos, const _GraFirstSetTable& fst)
{
	_GraTokenSet ts;
	//the end of rule
	uintptr uStart = uDotPos + 1;
	if( uStart >= rs.GetRule(uRuleNo).GetCount() )
		return ts;
	uint uID = rs.GetRule(uRuleNo).GetToken(uStart).GetID();
	//terminal
	if( uID <= uTerminalMaxID ) {
		_GraToken tk;
		tk.SetID(uID);
		ts.Add(tk);  //may throw
		return ts;
	}
	ts = fst.Find(uID);
	return ts;
}

// seed set -> closure set

inline void _Gra_SeedItem_To_ClosureSet(const _GraDotItem& item, uintptr uSeedIndex, uint uTerminalMaxID, const _GraRuleSet& rs, const _GraFirstSetTable& fst, _GraDotItemSet& dis)
{
	uintptr uRuleNo = item.GetRuleNo();
	uintptr uDotPos = item.GetDotPos();
	//the end of rule
	if( uDotPos == INVALID_ARRAY_INDEX )
		return ;
	//terminal
	if( rs.GetRule(uRuleNo).GetToken(uDotPos).GetID() <= uTerminalMaxID )
		return ;
	//propagation
	ShareArray<_GraDotItem::pairClass> arrPropagation(ShareArrayHelper::MakeShareArray<_GraDotItem::pairClass>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	arrPropagation.Add(_GraDotItem::pairClass(0, uSeedIndex));  //may throw
	//loop
	ShareArray<_GraDotItem> arrStack(ShareArrayHelper::MakeShareArray<_GraDotItem>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	{
		//virtual look ahead symbol $
		_GraDotItem itemA;
		itemA.SetRuleNo(uRuleNo);
		itemA.SetDotPos(uDotPos);
		arrStack.Add(itemA);  //may throw
	} //end block
	while( arrStack.GetCount() > 0 ) {
		_GraDotItem itemC(arrStack[0].get_Value());
		arrStack.RemoveAt(0);
		uRuleNo = itemC.GetRuleNo();
		uDotPos = itemC.GetDotPos();
		//first
		_GraTokenSet tsFirst(_Gra_Calc_After_NT_First(uTerminalMaxID, rs, uRuleNo, uDotPos, fst));  //may throw
		//token
		_GraToken tkc(rs.GetRule(uRuleNo).GetToken(uDotPos));
		//rules
		uintptr rule_num = 0;
		for( uintptr i = 0; i < rs.GetCount(); i ++ ) {
			//left part
			if( rs.GetRule(i).GetToken(0) == tkc ) {
				//token
				uRuleNo = i;
				uDotPos = 1;
				const _GraToken& tka = rs.GetRule(uRuleNo).GetToken(uDotPos);
				//new item
				_GraDotItem itemN;
				itemN.SetRuleNo(uRuleNo);
				itemN.SetDotPos(uDotPos);
				if( tsFirst.GetCount() == 0 ) {
					//propagation
					itemN.SetLookAhead(itemC.GetLookAhead());
					if( itemN.GetLookAheadCount() == 0 )
						itemN.SetPropagation(arrPropagation);
				}
				else {
					//self-generated
					itemN.SetLookAhead(tsFirst);
				}
				//output
				if( dis.Find(itemN) == INVALID_ARRAY_INDEX ) {
					dis.Add() = itemN;  //may throw
					if( tka.GetID() > uTerminalMaxID )
						arrStack.Add(itemN);  //may throw
				}
				rule_num ++;
			}
		} //end for
		//the nonterminal must be the left part of at least one production rule
		assert( rule_num != 0 );
	} //end while
}

inline void _Gra_SeedSet_To_ClosureSet(const _GraDotItemSet& seedSet, uint uTerminalMaxID, const _GraRuleSet& rs, const _GraFirstSetTable& fst, _GraDotItemSet& closureSet)
{
	for( uintptr i = 0; i < seedSet.GetCount(); i ++ ) {
		_GraDotItemSet dis;
		_Gra_SeedItem_To_ClosureSet(seedSet.GetItem(i), i, uTerminalMaxID, rs, fst, dis);  //may throw
		closureSet.Combine(dis);  //may throw
	}
}

// transition

inline void _Gra_Calc_DotItemSet_TransitionSet(const _GraDotItemSet& dis, const _GraRuleSet& rs, _GraTokenSet& tsShift)
{
	for( uintptr i = 0; i < dis.GetCount(); i ++ ) {
		const _GraDotItem& item = dis.GetItem(i);
		uintptr uRuleNo = item.GetRuleNo();
		uintptr uDotPos = item.GetDotPos();
		if( uDotPos == INVALID_ARRAY_INDEX )
			continue;
		tsShift.AddUniqueToken(rs.GetRule(uRuleNo).GetToken(uDotPos));  //may throw
	} //end for
}
inline void _Gra_Calc_TransitionSet(const _GraDotItemSet& seedSet, const _GraDotItemSet& closureSet, const _GraRuleSet& rs, _GraTokenSet& tsShift)
{
	_Gra_Calc_DotItemSet_TransitionSet(seedSet, rs, tsShift);  //may throw
	_Gra_Calc_DotItemSet_TransitionSet(closureSet, rs, tsShift);  //may throw
}

inline void _Gra_Calc_NextSet(const _GraDotItemSet& seedSet, const _GraDotItemSet& closureSet, const _GraToken& tk, const _GraRuleSet& rs, _GraDotItemSet& nextSet)
{
	//seed set
	for( uintptr i = 0; i < seedSet.GetCount(); i ++ ) {
		const _GraDotItem& item = seedSet.GetItem(i);
		uintptr uRuleNo = item.GetRuleNo();
		uintptr uDotPos = item.GetDotPos();
		if( uDotPos == INVALID_ARRAY_INDEX )
			continue;
		if( tk != rs.GetRule(uRuleNo).GetToken(uDotPos) )
			continue;
		//transition
		uDotPos ++;
		_GraDotItem& itemN = nextSet.Add();  //may throw
		itemN.SetRuleNo(uRuleNo);
		itemN.SetDotPos((uDotPos >= rs.GetRule(uRuleNo).GetCount()) ? INVALID_ARRAY_INDEX : uDotPos);
		//propagation
		itemN.AddPropagation(0, i);  //may throw
	} //end for
	//closure set
	for( uintptr i = 0; i < closureSet.GetCount(); i ++ ) {
		const _GraDotItem& item = closureSet.GetItem(i);
		uintptr uRuleNo = item.GetRuleNo();
		uintptr uDotPos = item.GetDotPos();
		assert( uDotPos == 1 );
		if( tk != rs.GetRule(uRuleNo).GetToken(uDotPos) )
			continue;
		//transition
		uDotPos ++;
		_GraDotItem& itemN = nextSet.Add();  //may throw
		itemN.SetRuleNo(uRuleNo);
		itemN.SetDotPos((uDotPos >= rs.GetRule(uRuleNo).GetCount()) ? INVALID_ARRAY_INDEX : uDotPos);
		//self-generated
		if( item.GetLookAheadCount() != 0 )
			itemN.SetLookAhead(item.GetLookAhead());
		//propagation
		if( item.GetPropagationCount() != 0 )
			itemN.SetPropagation(item.GetPropagation());
	} //end for
}

// propagation relationship

inline void _Gra_Calc_DotItemSet_Propagation_Relationship(_GraDotItemSet& nextSet, _GraDotItemSet* pSet, uintptr uStateNo, _GraDotItemSet& seedSet)
{
	for( uintptr i = 0; i < nextSet.GetCount(); i ++ ) {
		_GraDotItem& item = nextSet.GetItem(i);
		uintptr uCount = item.GetPropagationCount();
		if( uCount == 0 )
			continue;
		uintptr k = i;
		if( pSet != NULL ) {
			k = pSet->Find2(item);
			assert( k != INVALID_ARRAY_INDEX );
			pSet->GetItem(k).CombineLookAhead(item);  //may throw
		}
		for( uintptr j = 0; j < uCount; j ++ ) {
			uintptr uState, uSeed;
			item.GetPropagationAt(j, uState, uSeed);
			_GraDotItem& itemS = seedSet.GetItem(uSeed);
			if( itemS.FindPropagation(uStateNo, k) == INVALID_ARRAY_INDEX )
				itemS.AddPropagation(uStateNo, k);  //may throw
		}
		item.ReleasePropagation();
	} //end for
}

// _GraStateItem

class _GraStateItem
{
public:
	_GraStateItem() throw()
	{
	}
	~_GraStateItem() throw()
	{
	}

	const _GraDotItemSet& GetSeedSet() const throw()
	{
		return m_seedSet;
	}
	_GraDotItemSet& GetSeedSet() throw()
	{
		return m_seedSet;
	}
	const _GraTokenSet& GetShiftSet() const throw()
	{
		return m_tsShift;
	}
	_GraTokenSet& GetShiftSet() throw()
	{
		return m_tsShift;
	}
	const ShareArray<uintptr>& GetNextStateArray() const throw()
	{
		return m_arrNext;
	}
	ShareArray<uintptr>& GetNextStateArray() throw()
	{
		return m_arrNext;
	}

	uintptr GetNextStateNo(uintptr index) const throw()
	{
		return m_arrNext[index].get_Value();
	}
	const _GraTokenSet& GetReductionSet() const throw()
	{
		return m_tsReduction;
	}
	uintptr GetReductionNo(uintptr index) const throw()
	{
		return m_arrReduction[index].get_Value();
	}

	uintptr GetShiftCount() const throw()
	{
		return m_tsShift.GetCount();
	}
	uintptr GetReductionCount() const throw()
	{
		return m_tsReduction.GetCount();
	}

	//next
	//  called only once
	void InitNextStateArray()
	{
		uintptr uCount = m_tsShift.GetCount();
		if( uCount == 0 )
			return ;
		if( m_arrNext.IsBlockNull() )
			m_arrNext = ShareArrayHelper::MakeShareArray<uintptr>(MemoryHelper::GetCrtMemoryManager());  //may throw
		m_arrNext.SetCount(uCount, 0);  //may throw
	}

	//reduction
	//  called only once
	void GenerateReduction()
	{
		//add tokens
		for( uintptr i = 0; i < m_seedSet.GetCount(); i ++ ) {
			const _GraDotItem& item = m_seedSet.GetItem(i);
			if( item.GetDotPos() == INVALID_ARRAY_INDEX ) {
				for( uintptr j = 0; j < item.GetLookAheadCount(); j ++ )
					m_tsReduction.AddUniqueToken(item.GetLookAheadAt(j));  //may throw
			}
		} //end for
		//remove reduction tokens which are in shift tokens
		uintptr i = 0;
		while( i < m_tsReduction.GetCount() ) {
			if( m_tsShift.Find(m_tsReduction.GetToken(i)) != INVALID_ARRAY_INDEX ) {
				m_tsReduction.RemoveAt(i);
			}
			else {
				i ++;
			}
		} //end while
		uintptr uCount = m_tsReduction.GetCount();
		if( uCount == 0 )
			return ;
		//reduction no.
		if( m_arrReduction.IsBlockNull() )
			m_arrReduction = ShareArrayHelper::MakeShareArray<uintptr>(MemoryHelper::GetCrtMemoryManager());  //may throw
		m_arrReduction.SetCount(uCount, 0);  //may throw
		for( i = 0; i < uCount; i ++ ) {
			const _GraToken& tk = m_tsReduction.GetToken(i);
			uintptr k = Limits<uintptr>::Max;
			for( uintptr j = 0; j < m_seedSet.GetCount(); j ++ ) {
				const _GraDotItem& item = m_seedSet.GetItem(j);
				if( item.GetDotPos() == INVALID_ARRAY_INDEX ) {
					if( item.FindLookAhead(tk) != INVALID_ARRAY_INDEX ) {
						uintptr v = item.GetRuleNo();
						if( v < k )
							k = v;
					}
				}
			}
			assert( k != Limits<uintptr>::Max );
			m_arrReduction.SetAt(i, k);
		} //end for
	}

private:
	_GraDotItemSet  m_seedSet;
	_GraTokenSet    m_tsShift;  //shift tokens
	_GraTokenSet    m_tsReduction;  //reduction tokens
	//transition
	ShareArray<uintptr> m_arrNext;  //next states
	ShareArray<uintptr> m_arrReduction;  //reduction no.

private:
	//noncopyable
	_GraStateItem(const _GraStateItem&) throw();
	_GraStateItem& operator=(const _GraStateItem&) throw();
};

// _GraStateSet

class _GraStateSet
{
private:
	typedef List<_GraStateItem>  _GraStateList;

public:
	_GraStateSet() throw() : m_pool(MemoryHelper::GetCrtMemoryManager())
	{
	}
	~_GraStateSet() throw()
	{
	}

	uintptr GetCount() const throw()
	{
		return m_arrState.GetCount();
	}
	const _GraStateItem& GetState(uintptr index) const throw()
	{
		return m_arrState[index].get_Value().get_Value();
	}

	//generate
	void Generate(const TokenTable& tt, const TokenTable& nt, const _GraRuleSet& rs)
	{
		//pre-compute
		_GraFirstSetTable fst;
		fst.Generate(tt.GetMaxID(), rs, nt);  //may throw
		//state
		if( m_arrState.IsBlockNull() )
			m_arrState = ShareArrayHelper::MakeShareArray<_GraStateList::Iterator>(MemoryHelper::GetCrtMemoryManager());  //may throw
		//to-do
		ShareArray<_GraStateList::Iterator> arrToDo(ShareArrayHelper::MakeShareArray<_GraStateList::Iterator>(MemoryHelper::GetCrtMemoryManager()));  //may throw
		ShareArray<RefPtr<_GraDotItem>> arrPG(ShareArrayHelper::MakeShareArray<RefPtr<_GraDotItem>>(MemoryHelper::GetCrtMemoryManager()));  //may throw
		//initial
		{
			auto iter(m_pool.AddTail());  //may throw
			m_arrState.Add(iter);  //may throw
			_GraDotItemSet& dis = iter.get_Value().GetSeedSet();
			_GraDotItem& item = dis.Add();  //may throw
			//S -> start_nonterminal {$}
			item.SetRuleNo(0);
			item.SetDotPos(1);
			_GraToken tk;
			tk.SetID(PDA_END_OF_EVENT);
			item.AddLookAhead(tk);  //may throw
			arrToDo.Add(iter);  //may throw
		} //end block
		//loop
		while( arrToDo.GetCount() > 0 ) {
			//extract
			_GraStateList::Iterator iterC(arrToDo[0].get_Value());
			arrToDo.RemoveAt(0);
			//closure set
			_GraStateItem& itemState = iterC.get_Value();
			_GraDotItemSet& seedSet = itemState.GetSeedSet();
			_GraDotItemSet closureSet;
			_Gra_SeedSet_To_ClosureSet(seedSet, tt.GetMaxID(), rs, fst, closureSet);  //may throw
			//transition
			_Gra_Calc_TransitionSet(seedSet, closureSet, rs, itemState.GetShiftSet());  //may throw
			itemState.InitNextStateArray();  //may throw
			uintptr uCount = itemState.GetShiftSet().GetCount();
			if( uCount == 0 )
				continue;
			ShareArray<uintptr>& arrNext = itemState.GetNextStateArray();
			for( uintptr i = 0; i < uCount; i ++ ) {
				const _GraToken& tk = itemState.GetShiftSet().GetToken(i);
				auto iterList(m_pool.AddTail());  //may throw
				_GraDotItemSet& nextSet = iterList.get_Value().GetSeedSet();
				_Gra_Calc_NextSet(seedSet, closureSet, tk, rs, nextSet);  //may throw
				if( nextSet.GetCount() == 0 ) {
					m_pool.RemoveAt(iterList);
					continue;
				}
				//check states
				uintptr uFind = find_state_array(nextSet);
				if( uFind == INVALID_ARRAY_INDEX ) {
					m_arrState.Add(iterList);  //may throw
					arrToDo.Add(iterList);  //may throw
					uintptr uStateNo = m_arrState.GetCount() - 1;
					arrNext[i].set_Value(uStateNo);
					//propagation
					_Gra_Calc_DotItemSet_Propagation_Relationship(nextSet, NULL, uStateNo, seedSet);  //may throw
				}
				else {
					arrNext[i].set_Value(uFind);
					//propagation
					_Gra_Calc_DotItemSet_Propagation_Relationship(nextSet, &(m_arrState[uFind].get_Value().get_Value().GetSeedSet()), uFind, seedSet);  //may throw
					//remove
					m_pool.RemoveAt(iterList);
				}
			} //end for
			//seed set -> propagation to-do array
			for( uintptr i = 0; i < seedSet.GetCount(); i ++ ) {
				const _GraDotItem& item = seedSet.GetItem(i);
				if( item.GetPropagationCount() != 0 )
					arrPG.Add(RefPtr<_GraDotItem>(item));  //may throw
			}
		} //end while
		//propagation
		process_propagation_todo(arrPG);  //may throw
		//reduction
		for( uintptr i = 0; i < m_arrState.GetCount(); i ++ )
			m_arrState[i].get_Value().get_Value().GenerateReduction();  //may throw
	}

private:
	uintptr find_state_array(const _GraDotItemSet& dis) const throw()
	{
		for( uintptr i = 0; i < m_arrState.GetCount(); i ++ ) {
			if( dis.IsSameSet(m_arrState[i].get_Value().get_Value().GetSeedSet()) )
				return i;
		}
		return INVALID_ARRAY_INDEX;
	}

	//propagation
	void process_propagation_todo(ShareArray<RefPtr<_GraDotItem>>& arrPG)
	{
		while( arrPG.GetCount() > 0 ) {
			const _GraDotItem& itemC = arrPG[0].get_Value().Deref();
			arrPG.RemoveAt(0);
			//propagation
			for( uintptr i = 0; i < itemC.GetPropagationCount(); i ++ ) {
				uintptr uState, uSeed;
				itemC.GetPropagationAt(i, uState, uSeed);
				_GraDotItem& itemN = m_arrState[uState].get_Value().get_Value().GetSeedSet().GetItem(uSeed);
				if( itemN.CombineLookAhead(itemC) != 0 && itemN.GetPropagationCount() != 0 ) {  //may throw
					RefPtr<_GraDotItem> rItem(itemN);
					if( !(ArrayUtilHelper::Find<RefPtr<_GraDotItem>, ShareArray<RefPtr<_GraDotItem>>>(arrPG, rItem).IsValid()) )
						arrPG.Add(rItem);  //may throw
				}
			} //end for
		} //end while
	}

private:
	_GraStateList m_pool;
	ShareArray<_GraStateList::Iterator> m_arrState;

private:
	//noncopyable
	_GraStateSet(const _GraStateSet&) throw();
	_GraStateSet& operator=(const _GraStateSet&) throw();
};

// generate table

inline void _Gra_Generate_Tables(const TokenTable& tt, const TokenTable& nt, const _GraRuleSet& rs, PdaTable& table)
{
	//generate
	_GraStateSet gss;
	gss.Generate(tt, nt, rs);  //may throw

	//check (start from 2)
	assert( gss.GetCount() < (uintptr)(Limits<int>::Max - 1 - 1) );  //with last NULL item

	int iMaxStateNo = (int)(gss.GetCount() - 1 + 2);
	int iMaxRuleNo  = (int)(rs.GetCount() - 1);

	//transition
	ShareArray<int> arrTransitionNum(ShareArrayHelper::MakeShareArray<int>(MemoryHelper::GetCrtMemoryManager()));  //may throw
	arrTransitionNum.SetCount(iMaxStateNo + 1, 0);  //may throw
	arrTransitionNum[0].set_Value(0);
	arrTransitionNum[1].set_Value(0);
	for( int i = 2; i <= iMaxStateNo; i ++ ) {
		const _GraStateItem& si = gss.GetState(i - 2);
		uintptr uCount = SafeOperators::AddThrow(si.GetShiftCount(), si.GetReductionCount());  //may throw
		assert( uCount < (uintptr)(Limits<int>::Max) );  //with last NULL item
		arrTransitionNum[i].set_Value((int)uCount);
	}

	//table
	ConstArray<int> arrC;
	ConstArrayHelper::SetInternalPointer(ShareArrayHelper::GetInternalPointer(arrTransitionNum), arrTransitionNum.GetCount(), arrC);
	table.Allocate(iMaxStateNo, arrC, iMaxRuleNo);  //may throw
	for( int i = 2; i <= iMaxStateNo; i ++ ) {
		const _GraStateItem& si = gss.GetState(i - 2);
		table.SetState(i, PDA_STATE_ERROR);
		uintptr uCount = si.GetReductionCount();
		for( int j = 0; j < (int)uCount; j ++ ) {
			uint uID = si.GetReductionSet().GetToken(j).GetID();
			uintptr uR = si.GetReductionNo(j);
			table.SetTransition(i, j, uID, uID, (uR == 0) ? PDA_STATE_ACCEPTED : -((int)uR));
		}
		for( int j = 0; j < (int)si.GetShiftCount(); j ++ ) {
			uint uID = si.GetShiftSet().GetToken(j).GetID();
			uintptr uR = si.GetNextStateNo(j);
			table.SetTransition(i, j + (int)uCount, uID, uID, (int)(uR + 2));
		}
	}
	for( int i = 1; i <= iMaxRuleNo; i ++ ) {
		const _GraRule& ru = rs.GetRule(i);
		table.SetRule(i, ru.GetToken(0).GetID(), (uint)(ru.GetCount() - 1));
	}
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
