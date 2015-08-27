/*
** Copyright (c) 2014, Xin YUAN, courses of Zhejiang University
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
This file contains string classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_STRING_H__
#define __GKC_STRING_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_ARRAY_H__
	#error GkcString.h requires GkcArray.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

//------------------------------------------------------------------------------

// FixedStringT<Tchar, t_size>
//   Tchar: CharA CharH CharL, CharS CharW

template <typename Tchar, uintptr t_size>
class FixedStringT : public FixedArray<Tchar, t_size, DefaultCompareTrait<Tchar>>
{
private:
	typedef FixedStringT<Tchar, t_size>  thisClass;
	typedef FixedArray<Tchar, t_size, DefaultCompareTrait<Tchar>>  baseClass;

public:
	FixedStringT() throw() : m_uLength(0)
	{
		baseClass::m_data[0] = 0;
	}
	FixedStringT(const thisClass& src) throw() : m_uLength(src.m_uLength)
	{
		assert( m_uLength < t_size );
		mem_copy(src.m_data, sizeof(Tchar) * (m_uLength + 1), baseClass::m_data);
	}
	~FixedStringT() throw()
	{
	}

	//operators
	thisClass& operator=(const thisClass& src) throw()
	{
		if( this != &src ) {
			m_uLength = src.m_uLength;
			assert( m_uLength < t_size );
			mem_copy(src.m_data, sizeof(Tchar) * (m_uLength + 1), baseClass::m_data);
		}
		return *this;
	}

	uintptr GetLength() const throw()
	{
		return m_uLength;
	}
	void SetLength(uintptr uLength) throw()
	{
		assert( uLength < t_size );
		m_uLength = uLength;
		baseClass::m_data[m_uLength] = 0;
	}

	bool IsEmpty() const throw()
	{
		return GetLength() == 0;
	}

	//iterators
	const typename thisClass::Iterator GetEnd() const throw()
	{
		return Iterator(RefPtr<Tchar>(baseClass::m_data + m_uLength));
	}
	typename thisClass::Iterator GetEnd() throw()
	{
		return Iterator(RefPtr<Tchar>(baseClass::m_data + m_uLength));
	}
	const ReverseIterator<typename thisClass::Iterator> GetReverseBegin() const throw()
	{
		return ReverseIterator<typename thisClass::Iterator>(GetEnd());
	}
	ReverseIterator<typename thisClass::Iterator> GetReverseBegin() throw()
	{
		return ReverseIterator<typename thisClass::Iterator>(GetEnd());
	}

	//methods
	void RecalcLength() throw()
	{
		m_uLength = calc_string_length(baseClass::m_data);
	}

private:
	uintptr m_uLength;

private:
	//logical
	bool operator==(const thisClass& right) const throw();
	bool operator!=(const thisClass& right) const throw();
	bool operator<(const thisClass& right) const throw();
	bool operator>(const thisClass& right) const throw();
	bool operator<=(const thisClass& right) const throw();
	bool operator>=(const thisClass& right) const throw();
};

// FixedStringCompareTrait<T>

template <class T>
class FixedStringCompareTrait
{
public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) == 0;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) != 0;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) > 0;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) < 0;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) >= 0;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) <= 0;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value()));
	}
};

// FixedStringCaseIgnoreCompareTrait<T>

template <class T>
class FixedStringCaseIgnoreCompareTrait
{
public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) == 0;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) != 0;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) > 0;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) < 0;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) >= 0;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) <= 0;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value()));
	}
};

// FixedStringHashTrait<T>

template <class T>
class FixedStringHashTrait
{
public:
	static uintptr CalcHash(const T& t) throw()
	{
		uintptr uHash = 0;
		const typename T::EType* pch = FixedArrayHelper::GetInternalPointer(t);
		assert( pch != NULL );
		while( *pch != 0 ) {
			uHash = (uHash << 5) + uHash + (uintptr)(*pch);
			pch ++;
		}
		return uHash;
	}
};

// FixedStringCaseIgnoreHashTrait<T>

template <class T>
class FixedStringCaseIgnoreHashTrait
{
public:
	static uintptr CalcHash(const T& t) throw()
	{
		uintptr uHash = 0;
		const typename T::EType* pch = FixedArrayHelper::GetInternalPointer(t);
		assert( pch != NULL );
		while( *pch != 0 ) {
			uHash = (uHash << 5) + uHash + (uintptr)char_upper(*pch);
			pch ++;
		}
		return uHash;
	}
};

// MessageException<t_size>

template <uintptr t_size>
class MessageException : public Exception
{
public:
	typedef FixedStringT<CharS, t_size>  MessageBufferClass;

public:
	MessageException() throw() : Exception()
	{
	}
	MessageException(const MessageException<t_size>& src) throw() : Exception(static_cast<const Exception&>(src))
	{
		m_messageBuffer = src.m_messageBuffer;
	}
	MessageException& operator=(const MessageException<t_size>& src) throw()
	{
		Exception::operator=(static_cast<const Exception&>(src));
		m_messageBuffer = src.m_messageBuffer;  //no check self
		return *this;
	}

	const MessageBufferClass& GetMessageBuffer() const throw()
	{
		return m_messageBuffer;
	}
	MessageBufferClass& GetMessageBuffer() throw()
	{
		return m_messageBuffer;
	}

private:
	FixedStringT<CharS, t_size>  m_messageBuffer;
};

//------------------------------------------------------------------------------

// StringT<Tchar>
//   Tchar: CharA CharH CharL, CharS CharW
template <typename Tchar>
class StringT : public SharedArray<Tchar>
{
private:
	typedef StringT<Tchar>  thisClass;
	typedef SharedArray<Tchar>  baseClass;

public:
	StringT() throw()
	{
	}
	StringT(const thisClass& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	StringT(thisClass&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~StringT() throw()
	{
	}

	//operators
	thisClass& operator=(const thisClass& src) throw()
	{
		return static_cast<thisClass&>(baseClass::operator=(static_cast<const baseClass&>(src)));
	}
	thisClass& operator=(thisClass&& src) throw()
	{
		return static_cast<thisClass&>(baseClass::operator=(rv_forward(static_cast<baseClass&>(src))));
	}

	uintptr GetLength() const throw()
	{
		uintptr uCount;
		return (baseClass::IsNull()) ? 0 :
				( (uCount = ((SharedArrayBlock*)(baseClass::m_pB))->GetLength(), uCount == 0) ? 0 : uCount - 1 )
				;
	}
	bool IsEmpty() const throw()
	{
		return GetLength() == 0;
	}

	//iterators
	const typename thisClass::Iterator GetEnd() const throw()
	{
		return typename thisClass::Iterator(RefPtr<Tchar>(baseClass::get_array_address() + GetLength()));
	}
	typename thisClass::Iterator GetEnd() throw()
	{
		return typename thisClass::Iterator(RefPtr<Tchar>(baseClass::get_array_address() + GetLength()));
	}
	const typename thisClass::Iterator GetReverseBegin() const throw()
	{
		return ReverseIterator<typename thisClass::Iterator>(GetEnd());
	}
	typename thisClass::Iterator GetReverseBegin() throw()
	{
		return ReverseIterator<typename thisClass::Iterator>(GetEnd());
	}

	const typename thisClass::Iterator GetAt(uintptr index) const throw()
	{
		assert( index < GetLength() );
		return typename thisClass::Iterator(RefPtr<Tchar>(baseClass::get_array_address() + index));
	}
	typename thisClass::Iterator GetAt(uintptr index) throw()
	{
		assert( index < GetLength() );
		return typename thisClass::Iterator(RefPtr<Tchar>(baseClass::get_array_address() + index));
	}
	void SetAt(uintptr index, const Tchar& t)  //may throw
	{
		assert( index < GetLength() );
		baseClass::get_array_address()[index] = t;
	}
	void SetAt(uintptr index, Tchar&& t)  //may throw
	{
		assert( index < GetLength() );
		baseClass::get_array_address()[index] = rv_forward(t);
	}

	//methods
	void SetLength(uintptr uLength)
	{
		uintptr uSize = SafeOperators::AddThrow(uLength, (uintptr)1);
		baseClass::SetCount(uSize, 0);
		baseClass::get_array_address()[uLength] = 0;
	}
	void RecalcLength() throw()
	{
		assert( baseClass::m_pB != NULL );  //must have a block for allocation
		SharedArrayBlock* pB = (SharedArrayBlock*)baseClass::m_pB;
		pB->SetLength(calc_string_length(baseClass::get_array_address()) + 1);
	}

	//clear content and free array
	void Clear() throw()
	{
		baseClass::RemoveAll();
	}
};

// StringX
typedef StringT<CharA>  StringA;
typedef StringT<CharH>  StringH;
typedef StringT<CharL>  StringL;
typedef StringT<CharS>  StringS;
typedef StringT<CharW>  StringW;

// StringCompareTrait<T>

template <class T>
class StringCompareTrait
{
public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) == 0;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) != 0;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) > 0;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) < 0;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) >= 0;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) <= 0;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value()));
	}
};

// StringCaseIgnoreCompareTrait<T>

template <class T>
class StringCaseIgnoreCompareTrait
{
public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) == 0;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) != 0;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) > 0;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) < 0;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) >= 0;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) <= 0;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value()));
	}
};

// StringHashTrait<T>

template <class T>
class StringHashTrait
{
public:
	static uintptr CalcHash(const T& t) throw()
	{
		uintptr uHash = 0;
		const typename T::EType* pch = SharedArrayHelper::GetInternalPointer(t);
		assert( pch != NULL );
		while( *pch != 0 ) {
			uHash = (uHash << 5) + uHash + (uintptr)(*pch);
			pch ++;
		}
		return uHash;
	}
};

// StringCaseIgnoreHashTrait<T>

template <class T>
class StringCaseIgnoreHashTrait
{
public:
	static uintptr CalcHash(const T& t) throw()
	{
		uintptr uHash = 0;
		const typename T::EType* pch = SharedArrayHelper::GetInternalPointer(t);
		assert( pch != NULL );
		while( *pch != 0 ) {
			uHash = (uHash << 5) + uHash + (uintptr)char_upper(*pch);
			pch ++;
		}
		return uHash;
	}
};

//------------------------------------------------------------------------------

// StringUtilHelper

class StringUtilHelper
{
public:
	//To C-style string
	template <typename Tchar>
	static RefPtr<Tchar> To_C_Style(const ConstStringT<Tchar>& str) throw()
	{
		return RefPtr<Tchar>(ConstHelper::GetInternalPointer(str));
	}
	template <typename Tchar, uintptr t_size>
	static RefPtr<Tchar> To_C_Style(const FixedStringT<Tchar, t_size>& str) throw()
	{
		return RefPtr<Tchar>(FixedArrayHelper::GetInternalPointer(str));
	}
	template <typename Tchar>
	static RefPtr<Tchar> To_C_Style(const StringT<Tchar>& str) throw()
	{
		return RefPtr<Tchar>(SharedArrayHelper::GetInternalPointer(str));
	}

	//To ConstStringT object
	template <typename Tchar, uintptr t_size>
	static ConstStringT<Tchar> To_ConstString(const FixedStringT<Tchar, t_size>& str) throw()
	{
		return ConstStringT<Tchar>(FixedArrayHelper::GetInternalPointer(str), str.GetLength());
	}
	template <typename Tchar>
	static ConstStringT<Tchar> To_ConstString(const StringT<Tchar>& str) throw()
	{
		return ConstStringT<Tchar>(SharedArrayHelper::GetInternalPointer(str), str.GetLength());
	}

	//make empty string
	template <typename Tchar>
	static StringT<Tchar> MakeEmptyString(const RefPtr<IMemoryManager>& mgr)
	{
		StringT<Tchar> ret;
		static_cast<SharedArray<Tchar>&>(ret) = SharedArrayHelper::MakeSharedArray<Tchar>(mgr);
		return ret;
	}

	//make string
	template <typename Tchar, uintptr t_size>
	static uintptr MakeString(const ConstStringT<Tchar>& strSrc, FixedStringT<Tchar, t_size>& strDest) throw()
	{
		assert( !strSrc.IsNull() );
		uintptr uCount = strSrc.GetCount();
		if( uCount == 0 ) {
			strDest.SetLength(0);
			return 0;
		}
		if( uCount > t_size - 1 )
			uCount = t_size - 1;
		strDest.SetLength(uCount);
		mem_copy(&(strSrc.GetBegin().get_Value()), uCount * sizeof(Tchar), &(strDest.GetBegin().get_Value()));
		return uCount;
	}
	template <typename Tchar>
	static void MakeString(const ConstStringT<Tchar>& strSrc, StringT<Tchar>& strDest)
	{
		assert( !strSrc.IsNull() );
		uintptr uCount = strSrc.GetCount();
		strDest.SetLength(uCount);
		if( uCount == 0 ) {
			return ;
		}
		mem_copy(&(strSrc.GetBegin().get_Value()), uCount * sizeof(Tchar), &(strDest.GetBegin().get_Value()));
	}

	template <typename Tchar, uintptr t_sizeS, uintptr t_sizeD>
	static uintptr MakeString(const FixedStringT<Tchar, t_sizeS>& strSrc, FixedStringT<Tchar, t_sizeD>& strDest) throw()
	{
		assert( t_sizeS != t_sizeD );
		uintptr uCount = strSrc.GetLength();
		if( uCount == 0 ) {
			strDest.SetLength(0);
			return 0;
		}
		if( uCount > t_sizeD - 1 )
			uCount = t_sizeD - 1;
		strDest.SetLength(uCount);
		mem_copy(&(strSrc.GetBegin().get_Value()), uCount * sizeof(Tchar), &(strDest.GetBegin().get_Value()));
		return uCount;
	}
	template <typename Tchar, uintptr t_size>
	static void MakeString(const FixedStringT<Tchar, t_size>& strSrc, StringT<Tchar>& strDest)
	{
		uintptr uCount = strSrc.GetLength();
		strDest.SetLength(uCount);
		if( uCount == 0 ) {
			return ;
		}
		mem_copy(&(strSrc.GetBegin().get_Value()), uCount * sizeof(Tchar), &(strDest.GetBegin().get_Value()));
	}

	template <typename Tchar, uintptr t_size>
	static uintptr MakeString(const StringT<Tchar>& strSrc, FixedStringT<Tchar, t_size>& strDest) throw()
	{
		uintptr uCount = strSrc.GetLength();
		if( uCount == 0 ) {
			strDest.SetLength(0);
			return 0;
		}
		if( uCount > t_size - 1 )
			uCount = t_size - 1;
		strDest.SetLength(uCount);
		mem_copy(&(strSrc.GetBegin().get_Value()), uCount * sizeof(Tchar), &(strDest.GetBegin().get_Value()));
		return uCount;
	}

	//clone
	template <typename Tchar>
	static StringT<Tchar> Clone(const StringT<Tchar>& str)
	{
		StringT<Tchar> ret;
		static_cast<SharedArray<Tchar>&>(ret) = SharedArrayHelper::Clone(static_cast<const SharedArray<Tchar>&>(str));
		return ret;
	}

	//append
	//  return: the characters of strSrc are copied
	template <typename Tchar, uintptr t_size>
	static uintptr Append(const ConstStringT<Tchar>& strSrc, INOUT FixedStringT<Tchar, t_size>& strDest) throw()
	{
		assert( !strSrc.IsNull() );
		uintptr uCount1 = strSrc.GetCount();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount;
		CallResult cr = SafeOperators::Add(uCount1, uCount2, uCount);
		if( cr.IsFailed() || uCount > t_size - 1 )
			uCount = t_size - 1;
		strDest.SetLength(uCount);
		uCount1 = uCount - uCount2;
		if( uCount1 == 0 )
			return 0;
		mem_copy(&(strSrc.GetBegin().get_Value()), uCount1 * sizeof(Tchar), &(strDest.GetAt(uCount2).get_Value()));
		return uCount1;
	}
	template <typename Tchar>
	static void Append(const ConstStringT<Tchar>& strSrc, INOUT StringT<Tchar>& strDest)
	{
		assert( !strSrc.IsNull() );
		uintptr uCount1 = strSrc.GetCount();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount = SafeOperators::AddThrow(uCount1, uCount2);
		strDest.SetLength(uCount);
		if( uCount1 == 0 )
			return ;
		mem_copy(&(strSrc.GetBegin().get_Value()), uCount1 * sizeof(Tchar), &(strDest.GetAt(uCount2).get_Value()));
	}

	template <typename Tchar, uintptr t_sizeS, uintptr t_sizeD>
	static uintptr Append(const FixedStringT<Tchar, t_sizeS>& strSrc, INOUT FixedStringT<Tchar, t_sizeD>& strDest) throw()
	{
		assert( &strSrc != &strDest );
		uintptr uCount1 = strSrc.GetLength();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount;
		CallResult cr = SafeOperators::Add(uCount1, uCount2, uCount);
		if( cr.IsFailed() || uCount > t_sizeD - 1 )
			uCount = t_sizeD - 1;
		strDest.SetLength(uCount);
		uCount1 = uCount - uCount2;
		if( uCount1 == 0 )
			return 0;
		mem_copy(&(strSrc.GetBegin().get_Value()), uCount1 * sizeof(Tchar), &(strDest.GetAt(uCount2).get_Value()));
		return uCount1;
	}
	template <typename Tchar, uintptr t_size>
	static void Append(const FixedStringT<Tchar, t_size>& strSrc, INOUT StringT<Tchar>& strDest)
	{
		uintptr uCount1 = strSrc.GetLength();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount = SafeOperators::AddThrow(uCount1, uCount2);
		strDest.SetLength(uCount);
		if( uCount1 == 0 )
			return ;
		mem_copy(&(strSrc.GetBegin().get_Value()), uCount1 * sizeof(Tchar), &(strDest.GetAt(uCount2).get_Value()));
	}

	template <typename Tchar, uintptr t_size>
	static uintptr Append(const StringT<Tchar>& strSrc, INOUT FixedStringT<Tchar, t_size>& strDest) throw()
	{
		uintptr uCount1 = strSrc.GetLength();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount;
		CallResult cr = SafeOperators::Add(uCount1, uCount2, uCount);
		if( cr.IsFailed() || uCount > t_size - 1 )
			uCount = t_size - 1;
		strDest.SetLength(uCount);
		uCount1 = uCount - uCount2;
		if( uCount1 == 0 )
			return 0;
		mem_copy(&(strSrc.GetBegin().get_Value()), uCount1 * sizeof(Tchar), &(strDest.GetAt(uCount2).get_Value()));
		return uCount1;
	}
	template <typename Tchar>
	static void Append(const StringT<Tchar>& strSrc, INOUT StringT<Tchar>& strDest)
	{
		assert( (&strSrc != &strDest) && (strSrc.m_pT != strDest.m_pT) );
		uintptr uCount1 = strSrc.GetLength();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount = SafeOperators::AddThrow(uCount1, uCount2);
		strDest.SetLength(uCount);
		if( uCount1 == 0 )
			return ;
		mem_copy(&(strSrc.GetBegin().get_Value()), uCount1 * sizeof(Tchar), &(strDest.GetAt(uCount2).get_Value()));
	}
	//append character
	//  return: 0 or 1, whether the character is copied
	template <typename Tchar, uintptr t_size>
	uintptr Append(const Tchar& ch, INOUT FixedStringT<Tchar, t_size>& strDest) throw()
	{
		uintptr uCount = strDest.GetLength();
		if( uCount >= t_size - 1 )
			return 0;
		strDest.SetLength(uCount + 1);
		strDest.SetAt(uCount, ch);
		return 1;
	}
	template <typename Tchar>
	static void Append(const Tchar& ch, INOUT StringT<Tchar>& strDest)
	{
		uintptr uCount = strDest.GetLength();
		strDest.SetLength(uCount + 1);  //may throw
		strDest.SetAt(uCount, ch);
	}

	//replace
	template <typename Tchar, uintptr t_size>
	static uintptr Replace(const Tchar& chOld, const Tchar& chNew, INOUT FixedStringT<Tchar, t_size>& str) throw()
	{
		assert( chOld != 0 && chNew != 0 && chOld != chNew );
		if( str.IsEmpty() )
			return 0;
		return _replace(chOld, chNew, str.GetBegin(), str.GetEnd());
	}
	template <typename Tchar>
	static uintptr Replace(const Tchar& chOld, const Tchar& chNew, INOUT StringT<Tchar>& str) throw()
	{
		assert( chOld != 0 && chNew != 0 && chOld != chNew );
		if( str.IsEmpty() )
			return 0;
		return _replace(chOld, chNew, str.GetBegin(), str.GetEnd());
	}

private:
	//_replace
	template <class TIterator>
	static uintptr _replace(const typename TIterator::EType& tOld, const typename TIterator::EType& tNew,
							TIterator& iterB, TIterator& iterE) throw()
	{
		uintptr uCount = 0;
		//loop
		for( auto iter = iterB; iter != iterE; iter.MoveNext() ) {
			if( iter.get_Value() == tOld ) {
				iter.set_Value(tNew);
				uCount ++;
			}
		}
		return uCount;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_STRING_H__
////////////////////////////////////////////////////////////////////////////////
