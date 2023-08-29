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

// ForEach
/*! \brief The procedure for each element.

The procedure for each element.
\tparam TIterator, TIterator1, TIterator2 The iterator class.
\tparam TFunctor The functor class.
\param iterBegin, iterBegin1, iterBegin2 [in] The beginning iterator.
\param iterEnd, iterEnd1, iterEnd2 [in] The end iterator.
\param func [in] The functor.
*/
template <class TIterator, class TFunctor>
inline void ForEach(const TIterator& iterBegin, const TIterator& iterEnd, const TFunctor& func)
{
	for ( TIterator iter(iterBegin); iter != iterEnd; iter.MoveNext() )
		func(iter.get_Value());
}
template <class TIterator1, class TIterator2, class TFunctor>
inline void ForEach(const TIterator1& iterBegin1, const TIterator1& iterEnd1,
				const TIterator2& iterBegin2, const TIterator2& iterEnd2,
				const TFunctor& func)
{
	TIterator1 iter1(iterBegin1);
	TIterator2 iter2(iterBegin2);
	for ( ; iter1 != iterEnd1 && iter2 != iterEnd2;
		iter1.MoveNext(), iter2.MoveNext() )
		func(iter1.get_Value(), iter2.get_Value());
}

// ForEachFunc
/*! \brief The function for each element.

The function for each element.
\tparam TIterator, TIterator1, TIterator2 The iterator class.
\tparam TFunctor The functor class.
\param iterBegin1, iterBegin2, iterBegin [in] The beginning iterator.
\param iterEnd1, iterEnd2, iterEnd [in] The end iterator.
\param func [in] The functor.
*/
template <class TIterator1, class TIterator2, class TFunctor>
inline void ForEachFunc(const TIterator1& iterBegin1, const TIterator1& iterEnd1,
				const TIterator2& iterBegin2, const TIterator2& iterEnd2,
				const TFunctor& func)
{
	TIterator1 iter1(iterBegin1);
	TIterator2 iter2(iterBegin2);
	for( ; iter1 != iterEnd1 && iter2 != iterEnd2;
		iter1.MoveNext(), iter2.MoveNext() )
		iter2.get_Value() = func(iter1.get_Value());
}
template <class TIterator1, class TIterator2, class TIterator, class TFunctor>
inline void ForEachFunc(const TIterator1& iterBegin1, const TIterator1& iterEnd1,
						const TIterator2& iterBegin2, const TIterator2& iterEnd2,
						const TIterator& iterBegin, const TIterator& iterEnd,
						const TFunctor& func)
{
	TIterator1 iter1(iterBegin1);
	TIterator2 iter2(iterBegin2);
	TIterator iter(iterBegin);
	for( ; iter1 != iterEnd1 && iter2 != iterEnd2 && iter != iterEnd;
		iter1.MoveNext(), iter2.MoveNext(), iter.MoveNext() )
		iter.get_Value() = func(iter1.get_Value(), iter2.get_Value());
}

//------------------------------------------------------------------------------
// ordered

// binary search
template <class TIterator, class TCompareTrait = DefaultCompareTrait<typename TIterator::EType>>
inline bool BinarySearch(const TIterator& iterBegin, const TIterator& iterEnd, const typename TIterator::EType& val, TIterator& iterResult) noexcept
{
	iterResult = iterBegin;
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
	iterResult.MoveDelta(left);
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

// classes

// PlusFunctor<T>
/*! \brief A PLUS functor class.

A PLUS functor class.
\tparam T The type.
*/
template <typename T>
struct PlusFunctor
{
	T operator()(const T& left, const T& right) const
	{
		return left + right;
	}
};
// MinusFunctor<T>
/*! \brief A MINUS functor class.

A MINUS functor class.
\tparam T The type.
*/
template <typename T>
struct MinusFunctor
{
	T operator()(const T& left, const T& right) const
	{
		return left - right;
	}
};
// MultipliesFunctor<T>
/*! \brief A MULTIPLIES functor class.

A MULTIPLIES functor class.
\tparam T The type.
*/
template <typename T>
struct MultipliesFunctor
{
	T operator()(const T& left, const T& right) const
	{
		return left * right;
	}
};
// DividesFunctor<T>
/*! \brief A DIVIDES functor class.

A DIVIDES functor class.
\tparam T The type.
*/
template <typename T>
struct DividesFunctor
{
	T operator()(const T& left, const T& right) const
	{
		return left / right;
	}
};
// ModulusFunctor<T>
/*! \brief A MODULUS functor class.

A MODULUS functor class.
\tparam T The type.
*/
template <typename T>
struct ModulusFunctor
{
	T operator()(const T& left, const T& right) const
	{
		return left % right;
	}
};
// NegateFunctor<T>
/*! \brief A NEGATE functor class.

A NEGATE functor class.
\tparam T The type.
*/
template <typename T>
struct NegateFunctor
{
	T operator()(const T& left) const
	{
		return -left;
	}
};

// EqualToFunctor<T, TCompareTrait>
/*! \brief A EQUAL_TO functor class.

A EQUAL_TO functor class.
\tparam T The type.
\tparam TCompareTrait A trait type for comparing.
*/
template <typename T, class TCompareTrait = DefaultCompareTrait<T>>
struct EqualToFunctor
{
	bool operator()(const T& left, const T& right) const
	{
		return TCompareTrait::IsEQ(left, right);
	}
};
// NotEqualToFunctor<T, TCompareTrait>
/*! \brief A NOT_EQUAL_TO functor class.

A NOT_EQUAL_TO functor class.
\tparam T The type.
\tparam TCompareTrait A trait type for comparing.
*/
template <typename T, class TCompareTrait = DefaultCompareTrait<T>>
struct NotEqualToFunctor
{
	bool operator()(const T& left, const T& right) const
	{
		return TCompareTrait::IsNE(left, right);
	}
};
// GreaterFunctor<T, TCompareTrait>
/*! \brief A GREATER functor class.

A GREATER functor class.
\tparam T The type.
\tparam TCompareTrait A trait type for comparing.
*/
template <typename T, class TCompareTrait = DefaultCompareTrait<T>>
struct GreaterFunctor
{
	bool operator()(const T& left, const T& right) const
	{
		return TCompareTrait::IsGT(left, right);
	}
};
// LessFunctor<T, TCompareTrait>
/*! \brief A LESS functor class.

A LESS functor class.
\tparam T The type.
\tparam TCompareTrait A trait type for comparing.
*/
template <typename T, class TCompareTrait = DefaultCompareTrait<T>>
struct LessFunctor
{
	bool operator()(const T& left, const T& right) const
	{
		return TCompareTrait::IsLT(left, right);
	}
};
// GreaterEqualFunctor<T, TCompareTrait>
/*! \brief A GREATER_EQUAL functor class.

A GREATER_EQUAL functor class.
\tparam T The type.
\tparam TCompareTrait A trait type for comparing.
*/
template <typename T, class TCompareTrait = DefaultCompareTrait<T>>
struct GreaterEqualFunctor
{
	bool operator()(const T& left, const T& right) const
	{
		return TCompareTrait::IsGE(left, right);
	}
};
// LessEqualFunctor<T, TCompareTrait>
/*! \brief A LESS_EQUAL functor class.

A LESS_EQUAL functor class.
\tparam T The type.
\tparam TCompareTrait A trait type for comparing.
*/
template <typename T, class TCompareTrait = DefaultCompareTrait<T>>
struct LessEqualFunctor
{
	bool operator()(const T& left, const T& right) const
	{
		return TCompareTrait::IsLE(left, right);
	}
};

// LogicalAndFunctor<T>
/*! \brief A LOGICAL_AND functor class.

A LOGICAL_AND functor class.
\tparam T The type.
*/
template <typename T>
struct LogicalAndFunctor
{
	bool operator()(const T& left, const T& right) const
	{
		return left && right;
	}
};
// LogicalOrFunctor<T>
/*! \brief A LOGICAL_OR functor class.

A LOGICAL_OR functor class.
\tparam T The type.
*/
template <typename T>
struct LogicalOrFunctor
{
	bool operator()(const T& left, const T& right) const
	{
		return left || right;
	}
};
// LogicalNotFunctor<T>
/*! \brief A LOGICAL_NOT functor class.

A LOGICAL_NOT functor class.
\tparam T The type.
*/
template <typename T>
struct LogicalNotFunctor
{
	bool operator()(const T& left) const
	{
		return !left;
	}
};

// BitNotFunctor<T>
/*! \brief A BIT_NOT functor class.

A BIT_NOT functor class.
\tparam T The type.
*/
template <typename T>
struct BitNotFunctor
{
	T operator()(const T& left) const
	{
		return ~left;
	}
};
// BitAndFunctor<T>
/*! \brief A BIT_AND functor class.

A BIT_AND functor class.
\tparam T The type.
*/
template <typename T>
struct BitAndFunctor
{
	T operator()(const T& left, const T& right) const
	{
		return left & right;
	}
};
// BitOrFunctor<T>
/*! \brief A BIT_OR functor class.

A BIT_OR functor class.
\tparam T The type.
*/
template <typename T>
struct BitOrFunctor
{
	T operator()(const T& left, const T& right) const
	{
		return left | right;
	}
};
// BitXorFunctor<T>
/*! \brief A BIT_XOR functor class.

A BIT_XOR functor class.
\tparam T The type.
*/
template <typename T>
struct BitXorFunctor
{
	T operator()(const T& left, const T& right) const
	{
		return left ^ right;
	}
};
// BitLeftShiftFunctor<T>
/*! \brief A BIT_LEFT_SHIFT functor class.

A BIT_LEFT_SHIFT functor class.
\tparam T The type.
*/
template <typename T>
struct BitLeftShiftFunctor
{
	T operator()(const T& left, const T& right) const
	{
		return left << right;
	}
};
// BitRightShiftFunctor<T>
/*! \brief A BIT_RIGHT_SHIFT functor class.

A BIT_RIGHT_SHIFT functor class.
\tparam T The type.
*/
template <typename T>
struct BitRightShiftFunctor
{
	T operator()(const T& left, const T& right) const
	{
		return left >> right;
	}
};

// PlusSelfFunctor<T>
/*! \brief A PLUS_SELF functor class.

A PLUS_SELF functor class.
\tparam T The type.
*/
template <typename T>
struct PlusSelfFunctor
{
	void operator()(T& left, const T& right) const
	{
		left += right;
	}
};
// MinusSelfFunctor<T>
/*! \brief A MINUS_SELF functor class.

A MINUS_SELF functor class.
\tparam T The type.
*/
template <typename T>
struct MinusSelfFunctor
{
	void operator()(T& left, const T& right) const
	{
		left -= right;
	}
};
// MultipliesSelfFunctor<T>
/*! \brief A MULTIPLIES_SELF functor class.

A MULTIPLIES_SELF functor class.
\tparam T The type.
*/
template <typename T>
struct MultipliesSelfFunctor
{
	void operator()(T& left, const T& right) const
	{
		left *= right;
	}
};
// DividesSelfFunctor<T>
/*! \brief A DIVIDES_SELF functor class.

A DIVIDES_SELF functor class.
\tparam T The type.
*/
template <typename T>
struct DividesSelfFunctor
{
	void operator()(T& left, const T& right) const
	{
		left /= right;
	}
};
// ModulusSelfFunctor<T>
/*! \brief A MODULUS_SELF functor class.

A MODULUS_SELF functor class.
\tparam T The type.
*/
template <typename T>
struct ModulusSelfFunctor
{
	void operator()(T& left, const T& right) const
	{
		left %= right;
	}
};

// BitAndSelfFunctor<T>
/*! \brief A BIT_AND_SELF functor class.

A BIT_AND_SELF functor class.
\tparam T The type.
*/
template <typename T>
struct BitAndSelfFunctor
{
	void operator()(T& left, const T& right) const
	{
		left &= right;
	}
};
// BitOrSelfFunctor<T>
/*! \brief A BIT_OR_SELF functor class.

A BIT_OR_SELF functor class.
\tparam T The type.
*/
template <typename T>
struct BitOrSelfFunctor
{
	void operator()(T& left, const T& right) const
	{
		left |= right;
	}
};
// BitXorSelfFunctor<T>
/*! \brief A BIT_XOR_SELF functor class.

A BIT_XOR_SELF functor class.
\tparam T The type.
*/
template <typename T>
struct BitXorSelfFunctor
{
	void operator()(T& left, const T& right) const
	{
		left ^= right;
	}
};
// BitLeftShiftSelfFunctor<T>
/*! \brief A BIT_LEFT_SHIFT_SELF functor class.

A BIT_LEFT_SHIFT_SELF functor class.
\tparam T The type.
*/
template <typename T>
struct BitLeftShiftSelfFunctor
{
	void operator()(T& left, const T& right) const
	{
		left <<= right;
	}
};
// BitRightShiftSelfFunctor<T>
/*! \brief A BIT_RIGHT_SHIFT_SELF functor class.

A BIT_RIGHT_SHIFT_SELF functor class.
\tparam T The type.
*/
template <typename T>
struct BitRightShiftSelfFunctor
{
	void operator()(T& left, const T& right) const
	{
		left >>= right;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_ALG_H__
////////////////////////////////////////////////////////////////////////////////
