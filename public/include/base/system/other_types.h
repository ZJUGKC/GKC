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

////////////////////////////////////////////////////////////////////////////////
// internal header
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//OS

#if defined(GKC_OS_WINDOWS)
	#include "Windows/other_types.h"
#elif defined(GKC_OS_LINUX)
	#include "Linux/other_types.h"
#else
	#error Error OS type!
#endif

//------------------------------------------------------------------------------
//types

typedef uint  char_f;  //full character type

//------------------------------------------------------------------------------
//functions

// memory operators

inline int mem_compare(const void* p1, const void* p2, uintptr count) noexcept
{
	return ::memcmp(p1, p2, count);
}
inline int mem_compare(const char_w* p1, const char_w* p2, uintptr count) noexcept
{
	return ::wmemcmp(p1, p2, count);
}

// memory

inline void* crt_alloc(uintptr uBytes) throw()
{
	return ::malloc(uBytes);
}

inline void* crt_realloc(void* p, uintptr uBytes) throw()
{
	return ::realloc(p, uBytes);
}

inline void  crt_free(void* p) throw()
{
	::free(p);
}

//------------------------------------------------------------------------------
//classes

// system_call_results

BEGIN_ENUM(system_call_results)
	ENUM_VALUE_ENTRY(S_EOF, CR_S_EOF)
	ENUM_VALUE_ENTRY(S_False, CR_S_FALSE)
	ENUM_VALUE_ENTRY(OK, CR_OK)
	ENUM_VALUE_ENTRY(Fail, CR_FAIL)
	ENUM_VALUE_ENTRY(Unexpected, CR_UNEXPECTED)
	ENUM_VALUE_ENTRY(OutOfMemory, CR_OUTOFMEMORY)
	ENUM_VALUE_ENTRY(BadAddress, CR_BADADDRESS)
	ENUM_VALUE_ENTRY(Overflow, CR_OVERFLOW)
	ENUM_VALUE_ENTRY(SABad, CR_SABAD)
	ENUM_VALUE_ENTRY(Invalid, CR_INVALID)
	ENUM_VALUE_ENTRY(NotImpl, CR_NOTIMPL)
	ENUM_VALUE_ENTRY(NameTooLong, CR_NAMETOOLONG)
	ENUM_VALUE_ENTRY(DiskFull, CR_DISKFULL)
	ENUM_VALUE_ENTRY(FDBad, CR_FDBAD)
	ENUM_VALUE_ENTRY(Corrupt, CR_CORRUPT)
	ENUM_VALUE_ENTRY(NoAccess, CR_NOACCESS)
	ENUM_VALUE_ENTRY(Abort, CR_ABORT)
	ENUM_VALUE_ENTRY(Canceled, CR_CANCELED)
END_ENUM()

// byte_order_helper

class byte_order_helper
{
public:
	/*! \brief Check the host system's endian.

	Check the host system's endian.
	\return true for big endian, false for little endian.
	*/
	static bool IsBigEndianHost() throw()
	{
		uint u = 1;
		return *((byte*)&u) != 0x01;
	}
	/*! \brief Swap an integer's byte order.

	Swap an integer's byte order.
	\param x [in] The input value.
	\return The swapped value.
	*/
	static ushort Swap(ushort x) throw()
	{
		return ((x & 0x00FF) << 8) | ((x >> 8) & 0x00FF);
	}
	static short Swap(short x) throw()
	{
		return (short)Swap((ushort)x);
	}
	static uint Swap(uint x) throw()
	{
		return ((x & 0x000000FF) << 24)
			| ((x & 0x0000FF00) << 8)
			| ((x & 0x00FF0000) >> 8)
			| ((x & 0xFF000000) >> 24);
	}
	static int Swap(int x) throw()
	{
		return (int)Swap((uint)x);
	}
	static uint64 Swap(uint64 x) throw()
	{
		return ((x & (uint64)0x00FF) << 56)
			| ((x & (((uint64)0x00FF) << 8)) << 40)
			| ((x & (((uint64)0x00FF) << 16)) << 24)
			| ((x & (((uint64)0x00FF) << 24)) << 8)
			| ((x & (((uint64)0x00FF) << 32)) >> 8)
			| ((x & (((uint64)0x00FF) << 40)) >> 24)
			| ((x & (((uint64)0x00FF) << 48)) >> 40)
			| ((x & (((uint64)0x00FF) << 56)) >> 56);
	}
	static int64 Swap(int64 x) throw()
	{
		return (int64)Swap((uint64)x);
	}

	//Swap a character's byte order
#if defined(GKC_OS_WINDOWS)
	static char_h Swap(char_h x) throw()
	{
		return (char_h)Swap((ushort)x);
	}
#elif defined(GKC_OS_LINUX)
	//static char_h Swap(char_h x) throw()
	//  the same as : static ushort Swap(ushort x) throw()
#else
	#error Error OS type!
#endif

	static char_l Swap(char_l x) throw()
	{
		return (char_l)Swap((uint)x);
	}
	//Swap a float's byte order
	static float Swap(float x) throw()
	{
		union_pair<float, uint> up{x};
		up.v2 = Swap(up.v2);
		return up.v1;
	}
	static double Swap(double x) throw()
	{
		union_pair<double, uint64> up{x};
		up.v2 = Swap(up.v2);
		return up.v1;
	}

	//GUID
	static void Swap(const guid& id_in, guid& id_out) throw()
	{
		assert( &id_in != &id_out );
		const byte* ps = (byte*)(&id_in);
		byte* pd = (byte*)(&id_out);
		//4 bytes
		*((uint*)pd) = Swap(*((uint*)ps));
		ps += sizeof(uint);
		pd += sizeof(uint);
		//2 bytes
		*((ushort*)pd) = Swap(*((ushort*)ps));
		ps += sizeof(ushort);
		pd += sizeof(ushort);
		//2 bytes
		*((ushort*)pd) = Swap(*((ushort*)ps));
		ps += sizeof(ushort);
		pd += sizeof(ushort);
		//8 bytes (big endian)
		mem_copy(ps, 8, pd);
	}
};

#pragma pack(push, 1)

// se_type<T>
//   T: short ushort int uint int64 uint64 float double
template <typename T>
class se_type
{
public:
	se_type() throw()
	{
	}
	se_type(const se_type<T>& src) throw() : m_t(src.m_t)
	{
	}
	~se_type() throw()
	{
	}
	se_type<T>& operator=(const se_type<T>& src) throw()
	{
		m_t = src.m_t;
		return *this;
	}

	const T get_Value() const throw()
	{
		return m_t;
	}
	void set_Value(const T& t) throw()
	{
		m_t = t;
	}

private:
	T m_t;
};

// le_type<T>
//   T: short ushort int uint int64 uint64 float double
template <typename T>
class le_type
{
public:
	le_type() throw()
	{
	}
	le_type(const le_type<T>& src) throw() : m_t(src.m_t)
	{
	}
	~le_type() throw()
	{
	}
	le_type<T>& operator=(const le_type<T>& src) throw()
	{
		m_t = src.m_t;
		return *this;
	}

	const T get_Value() const throw()
	{
		return ( byte_order_helper::IsBigEndianHost() ) ? byte_order_helper::Swap(m_t) : m_t;
	}
	void set_Value(const T& t) throw()
	{
		m_t = ( byte_order_helper::IsBigEndianHost() ) ? byte_order_helper::Swap(t) : t;
	}

private:
	T m_t;
};

// be_type<T>
//   T: short ushort int uint int64 uint64 float double
template <typename T>
class be_type
{
public:
	be_type() throw()
	{
	}
	be_type(const be_type<T>& src) throw() : m_t(src.m_t)
	{
	}
	~be_type() throw()
	{
	}
	be_type<T>& operator=(const be_type<T>& src) throw()
	{
		m_t = src.m_t;
		return *this;
	}

	const T get_Value() const throw()
	{
		return ( !byte_order_helper::IsBigEndianHost() ) ? byte_order_helper::Swap(m_t) : m_t;
	}
	void set_Value(const T& t) throw()
	{
		m_t = ( !byte_order_helper::IsBigEndianHost() ) ? byte_order_helper::Swap(t) : t;
	}

private:
	T m_t;
};

#pragma pack(pop)

// -----exception-----

// exception_base

class exception_base
{
public:
	exception_base() throw()
	{
	}
	explicit exception_base(const call_result& res) throw() : m_result(res)
	{
	}
	exception_base(const exception_base& src) throw() : m_result(src.m_result)
	{
	}
	~exception_base() throw()
	{
	}

	//operators
	exception_base& operator=(const exception_base& src) throw()
	{
		m_result = src.m_result;
		return *this;
	}

	//methods
	call_result GetResult() const throw()
	{
		return m_result;
	}
	void SetResult(const call_result& res) throw()
	{
		m_result = res;
	}

protected:
	call_result m_result;
};

// exception_t<t_cr>

template <int t_cr>
class exception_t : public exception_base
{
public:
	exception_t() throw() : exception_base(call_result(t_cr))
	{
	}
	exception_t(const exception_t<t_cr>& src) throw() : exception_base(static_cast<const exception_base&>(src))
	{
	}
	exception_t<t_cr>& operator=(const exception_t<t_cr>& src) throw()
	{
		exception_base::operator=(static_cast<const exception_base&>(src));
		return *this;
	}
	~exception_t() throw()
	{
	}
};

// outofmemory_exception
typedef exception_t<system_call_results::OutOfMemory>  outofmemory_exception;

// overflow_exception
typedef exception_t<system_call_results::Overflow>  overflow_exception;

// -----traits-----

// compare_trait_base<T>

template <typename T>
class compare_trait_base
{
public:
	typedef T  EType;

public:
	//common versions
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return t1 == t2;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return t1 != t2;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return t1 > t2;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return t1 < t2;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return t1 >= t2;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return t1 <= t2;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return IsLT(t1, t2) ? -1 : (IsEQ(t1, t2) ? 0 : (assert( IsGT(t1, t2) ), 1));
	}
};

// default_compare_trait<T>

template <typename T>
class default_compare_trait : public compare_trait_base<T>
{
};

//special versions
#define _DECLARE_INT_COMPARE_TRAIT(T)  \
	template <> class default_compare_trait<T> : public compare_trait_base<T> {  \
	public:  \
	static int Compare(const T& t1, const T& t2) throw() { return (int)t1 - (int)t2; }  \
	};

// sizeof(T) must be less than sizeof(int) or sizeof(unsigned int)
_DECLARE_INT_COMPARE_TRAIT(char)
_DECLARE_INT_COMPARE_TRAIT(byte)
_DECLARE_INT_COMPARE_TRAIT(short)
_DECLARE_INT_COMPARE_TRAIT(ushort)
_DECLARE_INT_COMPARE_TRAIT(bool)

template <>
class default_compare_trait<float>
{
public:
	typedef float  EType;

public:
	static bool IsEQ(const float& t1, const float& t2) throw()
	{
		return ::fabsf(t1 - t2) < FLT_EPSILON;
	}
	static bool IsNE(const float& t1, const float& t2) throw()
	{
		return ::fabsf(t1 - t2) >= FLT_EPSILON;
	}
	static bool IsGT(const float& t1, const float& t2) throw()
	{
		return IsLT(t2, t1);
	}
	static bool IsLT(const float& t1, const float& t2) throw()
	{
		return t1 + FLT_EPSILON <= t2;
	}
	static bool IsGE(const float& t1, const float& t2) throw()
	{
		return !IsLT(t1, t2);
	}
	static bool IsLE(const float& t1, const float& t2) throw()
	{
		return !IsGT(t1, t2);
	}
	static int Compare(const float& t1, const float& t2) throw()
	{
		return IsLT(t1, t2) ? -1 : IsGT(t1, t2) ? 1 : 0;
	}
};

template <>
class default_compare_trait<double>
{
public:
	typedef double  EType;

public:
	static bool IsEQ(const double& t1, const double& t2) throw()
	{
		return ::fabs(t1 - t2) < DBL_EPSILON;
	}
	static bool IsNE(const double& t1, const double& t2) throw()
	{
		return ::fabs(t1 - t2) >= DBL_EPSILON;
	}
	static bool IsGT(const double& t1, const double& t2) throw()
	{
		return IsLT(t2, t1);
	}
	static bool IsLT(const double& t1, const double& t2) throw()
	{
		return t1 + DBL_EPSILON <= t2;
	}
	static bool IsGE(const double& t1, const double& t2) throw()
	{
		return !IsLT(t1, t2);
	}
	static bool IsLE(const double& t1, const double& t2) throw()
	{
		return !IsGT(t1, t2);
	}
	static int Compare(const double& t1, const double& t2) throw()
	{
		return IsLT(t1, t2) ? -1 : IsGT(t1, t2) ? 1 : 0;
	}
};

// default_hash_trait<T>

template <typename T>
class default_hash_trait
{
public:
	static uintptr CalcHash(const T& t) throw()
	{
		return (uintptr)t;
	}
};

//special versions
template <>
class default_hash_trait<float>
{
public:
	static uintptr CalcHash(const float& t) throw()
	{
		return (uintptr)(*((uint*)(&t)));
	}
};

template <>
class default_hash_trait<double>
{
public:
	static uintptr CalcHash(const double& t) throw()
	{
		uint* p = (uint*)(&t);
		return (uintptr)(p[0] ^ p[1]);
	}
};

// -----collection-----

//relocate
template <typename T>
inline void relocate_array_elements(const T* pSrc, T* pDest, uintptr size) throw()
{
	mem_move(pSrc, size * sizeof(T), pDest);
}

// coll_replace_elements<TIterator, TCompareTrait>

template <class TIterator, class TCompareTrait = default_compare_trait<typename TIterator::EType>>
inline uintptr coll_replace_elements(const typename TIterator::EType& tOld, const typename TIterator::EType& tNew,
									TIterator iterB, TIterator iterE)
{
	uintptr uCount = 0;
	//loop
	for( auto iter(iterB); iter != iterE; iter.MoveNext() ) {
		if( TCompareTrait::IsEQ(iter.get_Value(), tOld) ) {
			iter.set_Value(tNew);  //may throw
			uCount ++;
		}
	}
	return uCount;
}

// -----constant string-----

#pragma pack(push, 1)

// const_string_t<Tchar>
//   Tchar : char_a char_h char_l, char_s char_w

template <typename Tchar>
class const_string_t : public const_array<Tchar>
{
private:
	typedef const_array<Tchar>     baseClass;
	typedef const_string_t<Tchar>  thisClass;

public:
	const_string_t() throw()
	{
	}
	constexpr const_string_t(const Tchar* p, uintptr size) throw() : baseClass(p, size)
	{
	}
	const_string_t(const thisClass& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	explicit const_string_t(const const_array_base<Tchar>& src) throw() : baseClass(src)
	{
	}
	//use default destructor

	//operators
	thisClass& operator=(const thisClass& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}

	bool IsEmpty() const throw()
	{
		return baseClass::GetCount() == 0;
	}
};

#pragma pack(pop)

// const_string_*
typedef const_string_t<char_a>  const_string_a;   //ansi or UTF8
typedef const_string_t<char_h>  const_string_h;   //UTF16
typedef const_string_t<char_l>  const_string_l;   //UTF32
typedef const_string_t<char_s>  const_string_s;   //system
typedef const_string_t<char_w>  const_string_w;   //wide version

// macros

// --<header file>--

// define constant string by constant array in a method body

#define DECLARE_LOCAL_CONST_STRING(char_type, array_name, len_name, x)  \
const char_type array_name[] = x;  \
const uintptr len_name = sizeof(array_name) / sizeof(char_type) - 1;

#define DECLARE_LOCAL_STATIC_CONST_STRING(char_type, array_name, len_name, x)  \
static constexpr const char_type array_name[] = x;  \
static constexpr const uintptr len_name = sizeof(array_name) / sizeof(char_type) - 1;

// define temporary constant string object by constant array in a method body

#define DECLARE_TEMP_CONST_STRING(const_string_type, x)  \
const_string_type(x, sizeof(x) / sizeof(const_string_type::EType) - 1)

// define static constant string

#define DECLARE_STATIC_CONST_STRING(cls, char_type)  \
	DECLARE_STATIC_CONST_ARRAY(cls, char_type)

// define constant string member

#define DECLARE_CONST_STRING_STRUCT_MEMBER_TYPE(char_type)  const_array_base<char_type>

#define DECLARE_CONST_STRING_STRUCT_MEMBER(v_name, char_type)  \
DECLARE_CONST_STRING_STRUCT_MEMBER_TYPE(char_type)  v_name;

#define DECLARE_STATIC_CONST_STRING_MEMBER(c_name, char_type)  \
static const DECLARE_CONST_STRING_STRUCT_MEMBER_TYPE(char_type)  c_name;

//DECLARE_CONST_STRING_ARRAY_TYPE(char_type)
#define DECLARE_CONST_STRING_ARRAY_TYPE(char_type)  const_array<DECLARE_CONST_STRING_STRUCT_MEMBER_TYPE(char_type)>

// --<.h end>--

// --<source file>--

// static constant string in .cpp
//   STATIC_CONST_STRING_ENTRY(x) can be used repeatedly with or without CRLF
//   "some" or L"some" or _S("some")

#define BEGIN_STATIC_CONST_STRING(cls)   const cls::EType cls::c_array[] =
#define STATIC_CONST_STRING_ENTRY(x)     x
#define END_STATIC_CONST_STRING(cls)     ;  \
		const uintptr cls::c_size = sizeof(cls::c_array) / sizeof(cls::c_array[0]) - 1;

//constant string member
#define IMPLEMENT_CONST_STRING_ENTRY(char_type, x)   { x, sizeof(x) / sizeof(char_type) - 1 }  //this macro can be used for implementing constant string member

#define IMPLEMENT_STATIC_CONST_STRING_MEMBER(cls, c_name, char_type, x)  \
const DECLARE_CONST_STRING_STRUCT_MEMBER_TYPE(char_type) cls::c_name =  \
IMPLEMENT_CONST_STRING_ENTRY(char_type, x) ;

// --<.cpp end>--

// calc_sub_string_act_length
//   for substring and deletion

inline uintptr calc_sub_string_act_length(uintptr uSrcLength, uintptr uStart, uintptr uLength) throw()
{
	if( uStart >= uSrcLength )
		return 0;
	uintptr uRet = uSrcLength - uStart;
	if( uRet > uLength )
		uRet = uLength;
	return uRet;
}

#pragma pack(push, 1)

// const string with prefix length
template <typename Tchar>
class const_prefix_string : public const_prefix_array<Tchar>
{
private:
	typedef const_prefix_array<Tchar>  baseClass;

public:
	explicit constexpr const_prefix_string(const Tchar* p = NULL) noexcept : baseClass(p)
	{
	}
	uintptr GetLength() const noexcept
	{
		uintptr uCount;
		return baseClass::m_data == NULL ? 0 : ((uCount = *((const uintptr*)(baseClass::m_data) - 1), uCount == 0) ? 0 : uCount - 1);
	}
	const_string_t<Tchar> To_ConstString() const noexcept
	{
		return const_string_t<Tchar>(baseClass::GetAddress(), GetLength());
	}
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////
