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

// FixedStringT<Tchar, t_size>
//   Tchar: CharA CharH CharL, CharS CharW

template <typename Tchar, uintptr t_size>
class FixedStringT : public FixedArray<Tchar, t_size, DefaultCompareTrait<Tchar>>
{
private:
	typedef FixedStringT<Tchar, t_size>  thisClass;
	typedef FixedArray<Tchar, t_size, DefaultCompareTrait<Tchar>>  baseClass;

public:
	FixStringT() throw() : m_uLength(0)
	{
		m_data[0] = 0;
	}
	FixedStringT(const thisClass& src) throw() : m_uLength(src.m_uLength)
	{
		assert( m_uLength < t_size );
		mem_copy(src.m_data, sizeof(Tchar) * (m_uLength + 1), m_data);
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
			mem_copy(src.m_data, sizeof(Tchar) * (m_uLength + 1), m_data);
		}
		return *this;
	}

	uintptr GetLength() const throw()
	{
		return m_uLength;
	}

	//iterators
	const Iterator GetEnd() const throw()
	{
		return Iterator(RefPtr<Tchar>(m_data + m_uLength));
	}
	Iterator GetEnd() throw()
	{
		return Iterator(RefPtr<Tchar>(m_data + m_uLength));
	}
	const ReverseIterator<Iterator> GetReverseBegin() const throw()
	{
		return ReverseIterator<Iterator>(GetEnd());
	}
	ReverseIterator<Iterator> GetReverseBegin() throw()
	{
		return ReverseIterator<Iterator>(GetEnd());
	}

	//methods
	void RecalcLength() throw()
	{
		m_uLength = calc_string_length(m_data);
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

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_STRING_H__
////////////////////////////////////////////////////////////////////////////////
