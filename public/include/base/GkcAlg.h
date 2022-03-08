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
This file contains algorithms for collection classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_ALG_H__
#define __GKC_ALG_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_DEF_H__
	#error GkcAlg.h requires GkcDef.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// functions

template <class TIterator, class TFunctor>
inline void ForEach(const TIterator& iterBegin, const TIterator& iterEnd, const TFunctor& func)
{
	for ( TIterator iter(iterBegin); iter != iterEnd; iter.MoveNext() )
		func(iter.get_Value());
}

// classes

//------------------------------------------------------------------------------
// ordered

// binary search
template <class TIterator, class TCompareTrait = DefaultCompareTrait<typename TIterator::EType>>
inline bool BinarySearch(const TIterator& iterBegin, const TIterator& iterEnd, const typename TIterator::EType& val, TIterator& iterResult) noexcept
{
	// [iterBegin, iterEnd)
	uintptr right = iterEnd.CalcDelta(iterBegin);
	if ( right == 0 )
		return false;
	uintptr left = 0;
	while ( left < right ) {
		uintptr mid = left + (right - left) / 2;  // if we use (left + right) / 2, it may overflow
		TIterator iter(iterBegin);
		iter.MoveDelta(mid);
		if ( TCompareTrait::IsEQ(iter.get_Value(), val) ) {
			iterResult = iter;
			return true;
		}
		if ( TCompareTrait::IsLT(iter.get_Value(), val) )
			left = mid + 1;
		else
			right = mid;
	}
	return false;
}
template <class TIterator, class TCompareTrait = DefaultCompareTrait<typename TIterator::EType>>
inline bool BinarySearchFirst(const TIterator& iterBegin, const TIterator& iterEnd, const typename TIterator::EType& val, TIterator& iterResult) noexcept
{
	if ( !BinarySearch(iterBegin, iterEnd, val, iterResult) )
		return false;
	TIterator iter(iterResult);
	while ( iter != iterBegin ) {
		iter.MovePrev();
		if ( TCompareTrait::IsNE(iter.get_Value(), val) )
			break;
		iterResult = iter;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_ALG_H__
////////////////////////////////////////////////////////////////////////////////
