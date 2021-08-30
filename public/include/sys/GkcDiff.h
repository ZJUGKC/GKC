/*
** Copyright (c) 2021, Xin YUAN, courses of Zhejiang University
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
This file contains Comparison functions.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_DIFF_H__
#define __GKC_DIFF_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_SYS_H__
	#error GkcDiff.h requires GkcSys.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

/*
** Gao Chengxiang, 2021
*/

// myers algorithm

typedef Pair<int, int> myers_point;

//find middle point from start to end in edit graph (a, b)
template <class TIterator, class TCompareTrait = DefaultCompareTrait<typename TIterator::EType>>
inline void myers_find_middle(const typename TIterator::EType* a, const typename TIterator::EType* b,
							const myers_point& start, const myers_point& end,
							UniqueArray<int>& arrForward, UniqueArray<int>& arrBackward,
							myers_point& pt) noexcept
{
	/*
	start 1 2 ... n
	1
	2
	...
	m             end
	*/
	int n = end.get_First() - start.get_First();    //x
	int m = end.get_Second() - start.get_Second();  //y

	//initialize
	for(int i = 0; i <= n + m; ++ i)
		arrForward[i] = -1;
	arrForward[m + 1] = 0;
	for(int i = 0; i <= n + m; ++ i)
		arrBackward[i] = -1;
	arrBackward[n - 1] = n;

	for(int d = 0; d <= (n + m + 1) / 2; d++) {
		//forward
		for(int k = -m + ((d^m)&1), kInv = k + m; k <= n; k += 2, kInv = k + m) {
			int x, y;
			if((kInv == 0 || arrForward[kInv - 1] < 0) && (kInv == m+n || arrForward[kInv + 1] < 0)) {
				continue;
			}
			else if(kInv == 0 || arrForward[kInv - 1] < 0 || (kInv != n + m && arrForward[kInv + 1] >= 0 && arrForward[kInv - 1] < arrForward[kInv + 1])) {
				//now: move from top (k+1)
				x = arrForward[kInv + 1];
				y = x - k;
			}
			else {
				//now: move from left (k-1)
				x = arrForward[kInv - 1] + 1;
				y = x - k;
			}
			if(x > n || y > m || x < 0 || y < 0)
				continue;
			int endx = x, endy = y;
			while(endx < n && endy < m && TCompareTrait::IsEQ(a[endx + start.get_First()], b[endy + start.get_Second()]))
				endx++, endy++;
			arrForward[kInv] = endx;
			if(arrForward[kInv] >= 0 && arrBackward[kInv] >= 0 && arrForward[kInv] >= arrBackward[kInv]) {
				pt.set_First(x + start.get_First());
				pt.set_Second(y + start.get_Second());
				return ;
			}
		}

		//backward
		for(int k = -m + ((d^n)&1), kInv = k + m; k <= n; k += 2, kInv = k + m) {
			int x, y;
			if((kInv == 0 || arrBackward[kInv - 1] < 0) && (kInv == m+n || arrBackward[kInv + 1] < 0)) {
				continue;
			}
			else if(kInv == m + n || arrBackward[kInv + 1] < 0 || (kInv != 0 && arrBackward[kInv - 1] >= 0 && arrBackward[kInv - 1] < arrBackward[kInv + 1])) {
				//move from bottom
				x = arrBackward[kInv - 1];
				y = x - k;
			}
			else {
				//move from right
				x = arrBackward[kInv + 1] - 1;
				y = x - k;
			}
			if(x > n || y > m || x < 0 || y < 0)
				continue;
			while(x > 0 && y > 0 && TCompareTrait::IsEQ(a[x - 1 + start.get_First()], b[y - 1 + start.get_Second()]))
				x--, y--;
			arrBackward[kInv] = x;
			if(arrForward[kInv] >= 0 && arrBackward[kInv] >= 0 && arrForward[kInv] >= arrBackward[kInv]) {
				pt.set_First(x + start.get_First());
				pt.set_Second(y + start.get_Second());
				return ;
			}
		}
	}
	pt = end;
}

//Longest common sequence
struct LcsItem
{
	int iStart1;  //based 0
	int iEnd1;
	int iStart2;  //based 0
	int iEnd2;
};

//myers algorithm

template <typename TIterator, class TCompareTrait = DefaultCompareTrait<typename TIterator::EType>>
inline void MyersDiff(const TIterator& a_begin, const TIterator& a_end,
					const TIterator& b_begin, const TIterator& b_end,
					UniqueArray<LcsItem>& arrLCS)
{
	//length
    auto a_size = a_end.CalcDelta(a_begin);
    auto b_size = b_end.CalcDelta(b_begin);
	assert( a_size >= 0 && b_size >= 0 );
	if( a_size > Limits<int>::Max
		|| b_size > Limits<int>::Max )
		throw OverflowException();
	int n = (int)a_size;
	int m = (int)b_size;

	//V[k]: forward & backward
	UniqueArray<int> arrForward;
	UniqueArray<int> arrBackward;
	{
		//n+m+2
		int count = SafeOperators::AddThrow(n, m);  //may throw
		count = SafeOperators::AddThrow(count, (int)2);  //may throw
		arrForward.SetCount(count);  //may throw
		arrBackward.SetCount(count);  //may throw
	}

	//result
	arrLCS.RemoveAll();

	//add all or delete all
	if( n == 0 || m == 0 )
		return ;

	//division stack
	UniqueArray<myers_point> arrSearchStack;
	//edit path
	UniqueArray<myers_point> arrEditPath;

	//initialize
	myers_point start(0, 0);
	myers_point end(n, m);
	myers_point middle;

	//loop
	do {
		//same sequence at beginning
		while(start.get_First() != end.get_First() && start.get_Second() != end.get_Second()
			&& TCompareTrait::IsEQ(*(&(a_begin.get_Value())+start.get_First()), *(&(b_begin.get_Value())+start.get_Second()))) {
			start.set_First(start.get_First()+1);
			start.set_Second(start.get_Second()+1);
			//based 1
			arrEditPath.Add(start);  //may throw
		}
		//current n < current m
		while(start.get_First() == end.get_First() && start.get_Second() != end.get_Second()) {
			start.set_Second(start.get_Second()+1);
			arrEditPath.Add(start);  //may throw
		}
		//current n > current m
		while(start.get_First() != end.get_First() && start.get_Second() == end.get_Second()) {
			start.set_First(start.get_First()+1);
			arrEditPath.Add(start);  //may throw
		}

		//search
		if(start != end) {
			myers_find_middle<TIterator, TCompareTrait>(&(a_begin.get_Value()), &(b_begin.get_Value()), start, end, arrForward, arrBackward, middle);
			arrSearchStack.Add(end);  //may throw
			end = middle;
		}
		else {
			uintptr uCount = arrSearchStack.GetCount();
			if(uCount != 0) {
				end = arrSearchStack[uCount - 1];
				arrSearchStack.RemoveAt(uCount - 1);
			}
		}
	} while(start != end || !arrSearchStack.IsEmpty());

	//to LCS
	start.Set(0, 0);
	for( auto it(arrEditPath.GetBegin()); it != arrEditPath.GetEnd(); it.MoveNext() ) {
		//the same
        if(it.get_Value().get_First() == start.get_First() + 1 && it.get_Value().get_Second() == start.get_Second() + 1) {
			if(arrLCS.GetCount() != 0) {
				LcsItem& item(arrLCS[arrLCS.GetCount() - 1]);
				if(item.iEnd1 + 1 == start.get_First() && item.iEnd2 + 1 == start.get_Second()) {
					item.iEnd1 = start.get_First();
					item.iEnd2 = start.get_Second();
					start = it.get_Value();
					continue;
				}
			}
			LcsItem item;
			item.iStart1 = item.iEnd1 = start.get_First();
			item.iStart2 = item.iEnd2 = start.get_Second();
			arrLCS.Add(item);  //may throw
		}
		else {
			assert( (it.get_Value().get_First() == start.get_First() && it.get_Value().get_Second() == start.get_Second() + 1) // add start.get_Second()
					|| (it.get_Value().get_First() == start.get_First() + 1 && it.get_Value().get_Second() == start.get_Second()) // delete start.get_First()
				);
		}
		start = it.get_Value();
	}
}

//Edit types
enum {
	Diff_Add = 0,
	Diff_Delete,
	Diff_Revise
};

//Diff item
struct DiffItem
{
	int iType;
	int iStart1;  //based 0
	int iEnd1;
	int iStart2;  //based 0
	int iEnd2;
};

inline void LcsToDiff(const UniqueArray<LcsItem>& arrLCS, int n, int m, UniqueArray<DiffItem>& arrDiff)
{
	arrDiff.RemoveAll();
	if( n == 0 && m == 0 )
		return ;

	DiffItem ditem;
	ditem.iStart1 = 0;
	ditem.iStart2 = 0;
	if( n == 0 ) {
		ditem.iType = Diff_Add;
		ditem.iEnd1 = 0;
		ditem.iEnd2 = m - 1;
		arrDiff.Add(ditem);  //may throw
		return ;
	}
	if( m == 0 ) {
		ditem.iType = Diff_Delete;
		ditem.iEnd1 = n - 1;
		ditem.iEnd2 = 0;
		arrDiff.Add(ditem);  //may throw
		return ;
	}
	if( arrLCS.GetCount() == 0 ) {
		ditem.iType = Diff_Revise;
		ditem.iEnd1 = n - 1;
		ditem.iEnd2 = m - 1;
		arrDiff.Add(ditem);  //may throw
		return ;
	}

	//loop
	int iLast1 = -1;
	int iLast2 = -1;
	for( uintptr i = 0; i < arrLCS.GetCount(); i ++ ) {
		const LcsItem& sitem(arrLCS[i]);
		ditem.iStart1 = iLast1 + 1;
		ditem.iStart2 = iLast2 + 1;
		if( sitem.iStart1 == iLast1 + 1 ) {
			if( (i == 0 && sitem.iStart2 != 0) || i != 0 ) {
				assert( (i == 0) || (sitem.iStart2 != iLast2 + 1) );
				ditem.iType = Diff_Add;
				ditem.iEnd1 = iLast1 + 1;
				ditem.iEnd2 = sitem.iStart2 - 1;
				arrDiff.Add(ditem);  //may throw
			}
		}
		else {
			if( sitem.iStart2 == iLast2 + 1 ) {
				ditem.iType = Diff_Delete;
				ditem.iEnd1 = sitem.iStart1 - 1;
				ditem.iEnd2 = iLast2 + 1;
				arrDiff.Add(ditem);  //may throw
			}
			else {
				ditem.iType = Diff_Revise;
				ditem.iEnd1 = sitem.iStart1 - 1;
				ditem.iEnd2 = sitem.iStart2 - 1;
				arrDiff.Add(ditem);  //may throw
			}
		}
		iLast1 = sitem.iEnd1;
		iLast2 = sitem.iEnd2;
	}
	//tail
	ditem.iStart1 = iLast1 + 1;
	ditem.iStart2 = iLast2 + 1;
	if( iLast1 == n - 1 ) {
		if( iLast2 < m - 1 ) {
			ditem.iType = Diff_Add;
			ditem.iEnd1 = iLast1 + 1;
			ditem.iEnd2 = m - 1;
			arrDiff.Add(ditem);  //may throw
		}
	}
	else {
		if( iLast2 == m - 1 ) {
			ditem.iType = Diff_Delete;
			ditem.iEnd1 = n - 1;
			ditem.iEnd2 = iLast2 + 1;
			arrDiff.Add(ditem);  //may throw
		}
		else {
			ditem.iType = Diff_Revise;
			ditem.iEnd1 = n - 1;
			ditem.iEnd2 = m - 1;
			arrDiff.Add(ditem);  //may throw
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
