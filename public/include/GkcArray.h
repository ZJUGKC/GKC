/*
** Copyright (c) 2013, Xin YUAN, courses of Zhejiang University
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
This file contains array classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_ARRAY_H__
#define __GKC_ARRAY_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_BASE_H__
	#error GkcArray.h requires GkcBase.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

// FixedArray<T, t_size>

template <typename T, uintptr t_size>
class FixedArray
{
public:
	static const uintptr c_size = t_size;  //!< The number of elements.

	typedef T EType;   //element type

private:
	typedef FixedArray<T, t_size>  thisClass;

public:
	//iterator
	class Iterator
	{
	public:
		Iterator() throw()
		{
		}
		Iterator(RefPtr<T>& element) throw() : m_element(element)
		{
		}
		Iterator(const Iterator& src) throw() : m_element(src.m_element)
		{
		}
		~Iterator() throw()
		{
		}
		Iterator& operator=(const Iterator& src) throw()
		{
			if( this != &src ) {
				m_element = src.m_element;
			}
			return *this;
		}

		T& get_Value() throw()
		{
			return m_element.Deref();
		}
		void set_Value(T& t)
		{
			m_element.Deref() = t;
		}

		//logical
		bool operator==(const Iterator& right) const throw()
		{
			return m_element == right.m_element;
		}
		bool operator!=(const Iterator& right) const throw()
		{
			return m_element != right.m_element;
		}

		//methods
		void MoveNext() throw()
		{
			m_element = &m_element.Deref() + 1;
		}
		void MovePrev() throw()
		{
			m_element = &m_element.Deref() - 1;
		}

	private:
		RefPtr<T> m_element;

		friend class thisClass;
	};

public:
	FixedArray() //may throw
	{
		assert( t_size > 0 );
	}
	FixedArray(const thisClass& src)
	{
		assert( t_size > 0 );
		copy_elements(src.m_data, m_data, t_size);
	}
	~FixedArray() throw()
	{
	}

	//operators
	thisClass& operator=(const thisClass& src)
	{
		assert( t_size > 0 );
		if( this != &src ) {
			copy_elements(src.m_data, m_data, t_size);
		}
		return *this;
	}

	Iterator operator[](uintptr index) throw()
	{
		return GetAt(index);
	}

	//methods
	Iterator GetBegin() throw()
	{
		return Iterator(RefPtr<T>(m_data));
	}
	Iterator GetEnd() throw()
	{
		return Iterator(RefPtr<T>(m_data + t_size));
	}
	ReverseIterator<Iterator> GetReverseBegin() throw()
	{
		return ReverseIterator<Iterator>(GetEnd());
	}
	ReverseIterator<Iterator> GetReverseEnd() throw()
	{
		return ReverseIterator<Iterator>(GetBegin());
	}

	Iterator GetAt(uintptr index) throw()
	{
		assert( index < t_size );
		return Iterator(refPtr<T>(m_data + index));
	}
	void SetAt(uintptr index, const T& t)
	{
		assert( index < t_size );
		m_data[index] = t;
	}

	//logical
	bool operator==(const thisClass& right) const throw()
	{
		for( uintptr i = 0; i < t_size; i ++ ) {
			if( LogicalOperators::IsNE(m_data[i], right.m_data[i]) )
				return false;
		}
		return true;
	}
	bool operator!=(const thisClass& right) const throw()
	{
		for( uintptr i = 0; i < t_size; i ++ ) {
			if( LogicalOperators::IsNE(m_data[i], right.m_data[i]) )
				return true;
		}
		return false;
	}

private:
	static void copy_elements(T* pSrc, T* pDest, uintptr size)
	{
		for( uintptr i = 0; i < size; i ++ ) {
			pDest[i] = pSrc[i];
		}
	}

private:
	T m_data[t_size];  //array
};

// FixedArrayEndian<T, t_size, t_bBigEndian>

template <typename T, uintptr t_size, bool t_bBigEndian>
class FixedArrayEndian : public FixedArray<T, t_size>
{
private:
	typedef FixedArrayEndian<T, t_size, t_bBigEndian>  thisClass;
	typedef FixedArray<T, t_size>  baseClass;

public:
	FixedArrayEndian()
	{
	}
	FixedArrayEndian(const thisClass& src) : baseClass(static_cast<baseClass&>(src))
	{
	}
	~FixedArrayEndian() throw()
	{
	}

	thisClass& operator=(const thisClass& src)
	{
		baseClass::operator=(static_cast<baseClass&>(src));
		return *this;
	}
};

//logical special
template <typename T, uintptr t_size, bool t_bBigEndian>
inline bool LogicalOperators::IsLT<FixedArrayEndian<T, t_size, t_bBigEndian>>(const FixedArrayEndian<T, t_size, t_bBigEndian>& t1, const FixedArrayEndian<T, t_size, t_bBigEndian>& t2) throw()
{
	if( t_bBigEndian ) {
		for( uintptr i = 0; i < t_size; i ++ ) {
			if( LogicalOperators::IsGT<T>(t1[i].get_Value(), t2[i].get_Value()) )
				return false;
			if( LogicalOperators::IsLT<T>(t1[i].get_Value(), t2[i].get_Value()) )
				return true;
		}
	}
	else {
		for( uintptr i = t_size; i > 0; i -- ) {
			if( LogicalOperators::IsGT<T>(t1[i - 1].get_Value(), t2[i - 1].get_Value()) )
				return false;
			if( LogicalOperators::IsLT<T>(t1[i - 1].get_Value(), t2[i - 1].get_Value()) )
				return true;
		}
	} //end if
	return false;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_ARRAY_H__
////////////////////////////////////////////////////////////////////////////////
