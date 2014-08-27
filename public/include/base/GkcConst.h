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
This file contains classes for const array.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_CONST_H__
#define __GKC_CONST_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_BASE_H__
	#error GkcConst.h requires GkcBase.h to be included first.
#endif

//const definition:
// h:   class C { ... public: static const T c_name; };
// cpp: const T C::c_name=...;

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//classes

// ConstArray<T>

template <typename T>
class ConstArray
{
public:
	typedef T  EType;
	typedef ArrayIterator<T>  Iterator;

public:
	//count
	static uintptr GetCount() throw()
	{
		return l_size;
	}

	//iterator
	static const Iterator GetBegin() throw()
	{
		return Iterator(RefPtr<T>(c_first));
	}

	static const Iterator GetEnd() throw()
	{
		return Iterator(RefPtr<T>(c_first + GetCount()));
	}
	static const ReverseIterator<Iterator> GetReverseBegin() throw()
	{
		return ReverseIterator<Iterator>(GetEnd());
	}
	static const ReverseIterator<Iterator> GetReverseEnd() throw()
	{
		return ReverseIterator<Iterator>(GetBegin());
	}

	static const Iterator GetAt(uintptr index) throw()
	{
		assert( index < GetCount() );
		return Iterator(RefPtr<T>(c_first + index));
	}

	//assignment, for special case
	static void SetPointer(const T* p, uintptr size) throw()
	{
		c_first = p;
		l_size  = size;
	}

private:
	static const T        c_array[];
	static const uintptr  c_size;
	static const T*       c_first;
	static uintptr        l_size;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////

// for definition in cpp
#define BEGIN_CONST_ARRAY(cls)    const typename GKC::cls::EType GKC::cls::c_array[] = {

#define CONST_ARRAY_ENTRY(x)           x,
#define CONST_ARRAY_ENTRY_LAST(y)      y

#define BEGIN_CONST_ARRAY_GROUP()      {
#define END_CONST_ARRAY_GROUP()        },
#define END_CONST_ARRAY_GROUP_LAST()   }

#define END_CONST_ARRAY(cls)   };  \
		const uintptr GKC::cls::c_size = sizeof(GKC::cls::c_array) / sizeof(GKC::cls::c_array[0]) - 1;  \
		const typename GKC::cls::EType* GKC::cls::c_first = GKC::cls::c_array;  \
		uintptr GKC::cls::l_size = GKC::cls::c_size;

////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_CONST_H__
////////////////////////////////////////////////////////////////////////////////
