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
Top-down table
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __R_TD_TABLE_H__
#define __R_TD_TABLE_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//special tokens
#define TK_EPSILON       ((uint)-10)

//rule table
typedef struct _tagRuleElement
{
	uint uToken;
	uint uAction;
} RULEELEMENT;

typedef struct _tagRuleItem
{
	const RULEELEMENT* pRule;
	uintptr uNum;
} RULEITEM;

// push-down table
class _RTdTable
{
private:
	struct _TableItem
	{
		int  iEpsilon;  // < 0 : -1 (start symbol) -2 ~ X (value+2 is the index of epsilon rule)
		//terminal id -> action
		RBMap<uint, int>  mapTerminal;
		//follow
		RBList<uint>  sFollow;

		_TableItem() noexcept : mapTerminal(MemoryHelper::GetCrtMemoryManager()),
								sFollow(MemoryHelper::GetCrtMemoryManager())
		{
		}
	};

public:
	_RTdTable() noexcept : m_map(MemoryHelper::GetCrtMemoryManager()),
						m_uStartNT(CPL_TK_NULL), m_uMaxTerminalID(CPL_TK_FIRST)
	{
	}
	_RTdTable(const _RTdTable&) = delete;
	_RTdTable& operator=(const _RTdTable&) = delete;
	~_RTdTable() noexcept
	{
	}

	bool Generate(const RULEELEMENT* pRules, uint uMaxTerminalID)
	{
		assert( pRules != NULL );
		m_map.RemoveAll();
		if( m_rules.IsBlockNull() )
			m_rules = ShareArrayHelper::MakeShareArray<RULEITEM>(MemoryHelper::GetCrtMemoryManager());  //may throw
		m_rules.RemoveAll();

		//rules
		const RULEELEMENT* p = pRules;
		while( p->uToken != CPL_TK_NULL ) {
			RULEITEM item;
			item.pRule = p;
			item.uNum  = 0;
			while( p->uToken != CPL_TK_NULL ) {
				(item.uNum) ++;
				p ++;
			}
			if( item.uNum == 1 )
				return false;
			//left part
			if( item.pRule->uToken <= uMaxTerminalID )
				return false;
			//left recursion
			if( item.pRule->uToken == item.pRule[1].uToken )
				return false;
			//add
			m_rules.Add(item);  //may throw
			p ++;
		}

		if( m_rules.IsEmpty() )
			return false;

		m_uStartNT = m_rules[0].pRule[0].uToken;

		//table
		if( !generate_first_set(uMaxTerminalID) )  //may throw
			return false;
		if( !add_follow_set(uMaxTerminalID) )  //may throw
			return false;

		m_uMaxTerminalID = uMaxTerminalID;

		return true;
	}

	//tools
	uint GetStartNT() const noexcept
	{
		return m_uStartNT;
	}
	uint GetMaxTerminalID() const noexcept
	{
		return m_uMaxTerminalID;
	}
	//return value: > 0 (push rule no) = 0 (error) < 0 (pop rule no)
	int Input(uint uNonterminal, uint uTerminal) const noexcept
	{
		auto iterN(m_map.Find(uNonterminal));
		if( iterN == m_map.GetEnd() )
			return 0;
		auto iterT(iterN.get_Value().get_Second().Deref().mapTerminal.Find(uTerminal));
		if( iterT != iterN.get_Value().get_Second().Deref().mapTerminal.GetEnd() )
			return iterT.get_Value().get_Second();
		if( iterN.get_Value().get_Second().Deref().iEpsilon < 0 ) {
			auto iterF(iterN.get_Value().get_Second().Deref().sFollow.Find(uTerminal));
			if( iterF != iterN.get_Value().get_Second().Deref().sFollow.GetEnd() )
				return iterN.get_Value().get_Second().Deref().iEpsilon;
		}
		return 0;
	}
	void GetRule(uintptr index, RULEITEM& item) const noexcept
	{
		assert( index < m_rules.GetCount() );
		item = m_rules[index];
	}

private:
	static bool index_to_action(uintptr index, bool bPop, int& iAct) noexcept
	{
		uintptr index2 = bPop ? index + 2 : index + 1;
		if( index2 < index )
			return false;
		if( bPop ) {
			if( index2 > (uintptr)(Limits<int>::Max) + 1 )
				return false;
			iAct = -((int)index2);
			return true;
		}
		if( index2 > (uintptr)(Limits<int>::Max) )
			return false;
		iAct = (int)index2;
		return true;
	}
	bool generate_first_set(uint uMaxTerminalID)
	{
		ShareArray<uintptr> vecIndex(ShareArrayHelper::MakeShareArray<uintptr>(MemoryHelper::GetCrtMemoryManager()));  //may throw

		//check eps & terminal
		uintptr index = 0;
		auto iter(m_rules.GetBegin());
		for( ; iter != m_rules.GetEnd(); iter.MoveNext(), ++ index ) {
			if( iter.get_Value().pRule[0].uToken == TK_EPSILON || iter.get_Value().pRule[0].uToken <= uMaxTerminalID )
				return false;
			bool bEps = (iter.get_Value().pRule[1].uToken == TK_EPSILON);
			if( bEps && iter.get_Value().uNum != 2 )
				return false;
			auto iterF(m_map.Find(iter.get_Value().pRule[0].uToken));
			if( iterF == m_map.GetEnd() ) {
				SharePtr<_TableItem> spItem(SharePtrHelper::MakeSharePtr<_TableItem>(MemoryHelper::GetCrtMemoryManager()));  //may throw
				spItem.Deref().iEpsilon = 0;
				if( bEps ) {
					if( !index_to_action(index, true, spItem.Deref().iEpsilon) )
						return false;
				}
				else {
					if( iter.get_Value().pRule[1].uToken <= uMaxTerminalID ) {
						int iAct;
						if( !index_to_action(index, false, iAct) )
							return false;
						spItem.Deref().mapTerminal.Insert(iter.get_Value().pRule[1].uToken, iAct);  //may throw
					}
					else {
						vecIndex.Add(index);  //may throw
					}
				}
				m_map.Insert(iter.get_Value().pRule[0].uToken, spItem);  //may throw
			}
			else {
				if( bEps ) {
					//two rules are the same.
					if( iterF.get_Value().get_Second().Deref().iEpsilon < 0 )
						return false;
					if( !index_to_action(index, true, iterF.get_Value().get_Second().Deref().iEpsilon) )
						return false;
				}
				else {
					if( iter.get_Value().pRule[1].uToken <= uMaxTerminalID ) {
						auto iterP(iterF.get_Value().get_Second().Deref().mapTerminal.Find(iter.get_Value().pRule[1].uToken));
						//two rules have the same terminal firstly.
						if( iterP != iterF.get_Value().get_Second().Deref().mapTerminal.GetEnd() )
							return false;
						int iAct;
						if( !index_to_action(index, false, iAct) )
							return false;
						iterF.get_Value().get_Second().Deref().mapTerminal.Insert(iter.get_Value().pRule[1].uToken, iAct);  //may throw
					}
					else {
						vecIndex.Add(index);  //may throw
					}
				}
			}
		}
		//closure
		uintptr uLastSize = vecIndex.GetCount();
		uintptr uVecIdx = 0;
		while( uVecIdx < vecIndex.GetCount() ) {
			index = vecIndex[uVecIdx];
			uint uNT = m_rules[index].pRule[1].uToken;
			//find
			bool bFound = false;
			for( auto iterV(vecIndex.GetBegin()); iterV != vecIndex.GetEnd(); iterV.MoveNext() ) {
				if( uNT == m_rules[iterV.get_Value()].pRule[0].uToken ) {
					bFound = true;
					break;
				}
			}
			if( bFound ) {
				//do next
				uVecIdx ++;
				if( uVecIdx == vecIndex.GetCount() ) {
					//dead loop
					if( uLastSize == vecIndex.GetCount() )
						return false;
					uLastSize = vecIndex.GetCount();
					uVecIdx = 0;
				}
				continue;
			}
			auto iterF(m_map.Find(uNT));
			//no rule
			if( iterF == m_map.GetEnd() )
				return false;
			//the first nonterminal of right part cannot derive epsilon
			if( iterF.get_Value().get_Second().Deref().iEpsilon < 0 )
				return false;
			auto iterP(m_map.Find(m_rules[index].pRule[0].uToken));
			assert( iterP != m_map.GetEnd() );
			//propagation
			/*
			terminals : left <-- (right, first item)
			*/
			for( auto iterF2 = iterF.get_Value().get_Second().Deref().mapTerminal.GetBegin();
				iterF2 != iterF.get_Value().get_Second().Deref().mapTerminal.GetEnd();
				iterF2.MoveNext() ) {
				//two rules have the same terminal firstly.
				auto iterP2(iterP.get_Value().get_Second().Deref().mapTerminal.Find(iterF2.get_Value().get_First()));
				if( iterP2 != iterP.get_Value().get_Second().Deref().mapTerminal.GetEnd() )
					return false;
				int iAct;
				if( !index_to_action(index, false, iAct) )
					return false;
				iterP.get_Value().get_Second().Deref().mapTerminal.Insert(iterF2.get_Value().get_First(), iAct);  //may throw
			}
			//remove
			vecIndex.RemoveAt(uVecIdx);
			if( uVecIdx == vecIndex.GetCount() ) {
				uLastSize = vecIndex.GetCount();
				uVecIdx = 0;
			}
		} //end while

		//check null rules
		for( auto iterF(m_map.GetBegin()); iterF != m_map.GetEnd(); iterF.MoveNext() ) {
			if( iterF.get_Value().get_Second().Deref().mapTerminal.GetCount() == 0 )
				return false;
		}

		return true;
	}
	bool add_follow_set(uint uMaxTerminalID)
	{
		typedef RBMap<uint, SharePtr<RBList<uint>>>  PropMap;

		//follow
		auto iter(m_rules.GetBegin());
		for( ; iter != m_rules.GetEnd(); iter.MoveNext() ) {
			if( iter.get_Value().uNum <= 2 )
				continue;
			for( uintptr i = 1; i < iter.get_Value().uNum - 1; i ++ ) {
				uint uToken = iter.get_Value().pRule[i].uToken;
				if( uToken == TK_EPSILON || uToken <= uMaxTerminalID )
					continue;
				//nonterminal
				auto iterF(m_map.Find(uToken));
				//no rule
				if( iterF == m_map.GetEnd() )
					return false;
				//next
				uintptr uNext = i + 1;
				while( uNext < iter.get_Value().uNum ) {
					uint uF = iter.get_Value().pRule[uNext].uToken;
					if( uF == TK_EPSILON )
						return false;
					//add terminal
					if( uF <= uMaxTerminalID ) {
						if( iterF.get_Value().get_Second().Deref().mapTerminal.Find(uF) == iterF.get_Value().get_Second().Deref().mapTerminal.GetEnd() ) {
							if( iterF.get_Value().get_Second().Deref().sFollow.Find(uF) == iterF.get_Value().get_Second().Deref().sFollow.GetEnd() )
								iterF.get_Value().get_Second().Deref().sFollow.Insert(uF);  //may throw
						}
						break;
					}
					//NT
					/*
					first set of NT --> (right, current item)
					*/
					auto iterP(m_map.Find(uF));
					if( iterP == m_map.GetEnd() )
						return false;
					for( auto iterF2 = iterP.get_Value().get_Second().Deref().mapTerminal.GetBegin();
						iterF2 != iterP.get_Value().get_Second().Deref().mapTerminal.GetEnd();
						iterF2.MoveNext() ) {
						if( iterF.get_Value().get_Second().Deref().mapTerminal.Find(iterF2.get_Value().get_First()) == iterF.get_Value().get_Second().Deref().mapTerminal.GetEnd() ) {
							if( iterF.get_Value().get_Second().Deref().sFollow.Find(iterF2.get_Value().get_First()) == iterF.get_Value().get_Second().Deref().sFollow.GetEnd() )
								iterF.get_Value().get_Second().Deref().sFollow.Insert(iterF2.get_Value().get_First());  //may throw
						}
					}
					//derive EPS
					if( !(iterP.get_Value().get_Second().Deref().iEpsilon < 0) )
						break;
					uNext ++;
				}
			}
		} //end for

		//EOE
		auto iterF(m_map.Find(m_uStartNT));
		assert( iterF != m_map.GetEnd() );
		iterF.get_Value().get_Second().Deref().iEpsilon = -1;  //special
		iterF.get_Value().get_Second().Deref().sFollow.Insert(CPL_TK_EOF);  //may throw

		//propagation
		/*
		construct propagation table
		*/
		PropMap mapProp(MemoryHelper::GetCrtMemoryManager());
		ShareArray<uint> vecToDo(ShareArrayHelper::MakeShareArray<uint>(MemoryHelper::GetCrtMemoryManager()));  //may throw
		iter = m_rules.GetBegin();
		for( ; iter != m_rules.GetEnd(); iter.MoveNext() ) {
			if( iter.get_Value().uNum <= 1 )
				continue;
			auto iterP(mapProp.Find(iter.get_Value().pRule[0].uToken));
			for( uintptr i = iter.get_Value().uNum - 1; i > 0; i -- ) {
				uint uToken = iter.get_Value().pRule[i].uToken;
				if( uToken == TK_EPSILON || uToken <= uMaxTerminalID )
					break;
				iterF = m_map.Find(uToken);
				assert( iterF != m_map.GetEnd() );
				//left --> (right, current item)
				if( uToken != iter.get_Value().pRule[0].uToken ) {
					if( iterP == mapProp.GetEnd() ) {
						SharePtr<RBList<uint>> spItem(SharePtrHelper::MakeSharePtr<RBList<uint>>(MemoryHelper::GetCrtMemoryManager(), MemoryHelper::GetCrtMemoryManager()));  //may throw
						iterP = mapProp.Insert(iter.get_Value().pRule[0].uToken, spItem);  //may throw
						vecToDo.Add(iter.get_Value().pRule[0].uToken);  //may throw
					}
					if( iterP.get_Value().get_Second().Deref().Find(uToken) == iterP.get_Value().get_Second().Deref().GetEnd() )
						iterP.get_Value().get_Second().Deref().Insert(uToken);  //may throw
				}
				//derive EPS
				if( !(iterF.get_Value().get_Second().Deref().iEpsilon < 0) )
					break;
			}
		}
		//loop
		while( !vecToDo.IsEmpty() ) {
			uint uToken = vecToDo[0];
			vecToDo.RemoveAt(0);
			auto iterP(mapProp.Find(uToken));
			if( iterP == mapProp.GetEnd() )
				continue;
			iterF = m_map.Find(uToken);
			assert( iterF != m_map.GetEnd() );
			for( auto iterP2 = iterP.get_Value().get_Second().Deref().GetBegin();
				iterP2 != iterP.get_Value().get_Second().Deref().GetEnd();
				iterP2.MoveNext() ) {
				uint uNT = iterP2.get_Value();
				auto iterF2(m_map.Find(uNT));
				assert( iterF2 != m_map.GetEnd() );
				//left --> (right, current item)
				bool bChanged = false;
				for( auto iterF3 = iterF.get_Value().get_Second().Deref().sFollow.GetBegin();
					iterF3 != iterF.get_Value().get_Second().Deref().sFollow.GetEnd();
					iterF3.MoveNext() ) {
					if( iterF2.get_Value().get_Second().Deref().mapTerminal.Find(iterF3.get_Value()) == iterF2.get_Value().get_Second().Deref().mapTerminal.GetEnd() ) {
						if( iterF2.get_Value().get_Second().Deref().sFollow.Find(iterF3.get_Value()) == iterF2.get_Value().get_Second().Deref().sFollow.GetEnd() ) {
							iterF2.get_Value().get_Second().Deref().sFollow.Insert(iterF3.get_Value());  //may throw
							bChanged = true;
						}
					}
				}
				if( bChanged ) {
					bool bFound = false;
					for( auto iterV = vecToDo.GetBegin(); iterV != vecToDo.GetEnd(); iterV.MoveNext() ) {
						if( uNT == iterV.get_Value() ) {
							bFound = true;
							break;
						}
					}
					if( !bFound )
						vecToDo.Add(uNT);  //may throw
				}
			}
		} //end while

		return true;
	}

private:
	//Nonterminal -> item
	RBMap<uint, SharePtr<_TableItem>> m_map;
	//rules
	ShareArray<RULEITEM> m_rules;
	//start symbol
	uint m_uStartNT;
	//maximum terminal ID
	uint m_uMaxTerminalID;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
