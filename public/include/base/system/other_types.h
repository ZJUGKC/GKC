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
//functions

// memory

inline void* crt_alloc(uintptr uBytes) throw()
{
	return ::malloc(uBytes);
}

inline void* crt_realloc(void* p, uintptr uBytes) throw()
{
	return ::realloc(p, uBytes);
}

inline void    crt_free(void* p) throw()
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
	ENUM_VALUE_ENTRY(OutOfMemory, CR_OUTOFMEMORY)
	ENUM_VALUE_ENTRY(Overflow, CR_OVERFLOW)
	ENUM_VALUE_ENTRY(SABad, CR_SABAD)
	ENUM_VALUE_ENTRY(Invalid, CR_INVALID)
	ENUM_VALUE_ENTRY(NotImpl, CR_NOTIMPL)
	ENUM_VALUE_ENTRY(NameTooLong, CR_NAMETOOLONG)
	ENUM_VALUE_ENTRY(DiskFull, CR_DISKFULL)
	ENUM_VALUE_ENTRY(FDBad, CR_FDBAD)
	ENUM_VALUE_ENTRY(Corrupt, CR_CORRUPT)
	ENUM_VALUE_ENTRY(NoAccess, CR_NOACCESS)
END_ENUM()

// byte_order_helper

class byte_order_helper
{
public:
	//Check the host system's endian
	static bool IsBigEndianHost() throw()
	{
		uint u = 1;
		return *((byte*)&u) != 0x01;
	}
	//Swap an integer's byte order
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
		uint* p1 = (uint*)&x;
		uint v = Swap(*p1);
		float* p2 = (float*)&v;
		return *p2;
	}
	static double Swap(double x) throw()
	{
		uint64* p1 = (uint64*)&x;
		uint64 v = Swap(*p1);
		double* p2 = (double*)&v;
		return *p2;
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
		if( &src != this ) {
			m_result = src.m_result;
		}
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

// -----number-----

// limits_base<T>

template <typename T>
class limits_base;

//special
template <>
class limits_base<char>
{
public:
	static const char Lowest = SCHAR_MIN;
	static const char Min = SCHAR_MIN;
	static const char Max = SCHAR_MAX;
};
template <>
class limits_base<byte>
{
public:
	static const byte Lowest = 0;
	static const byte Min = 0;
	static const byte Max = UCHAR_MAX;
};

template <>
class limits_base<short>
{
public:
	static const short Lowest = SHRT_MIN;
	static const short Min = SHRT_MIN;
	static const short Max = SHRT_MAX;
};
template <>
class limits_base<ushort>
{
public:
	static const ushort Lowest = 0;
	static const ushort Min = 0;
	static const ushort Max = USHRT_MAX;
};

template <>
class limits_base<int>
{
public:
	static const int Lowest = INT_MIN;
	static const int Min = INT_MIN;
	static const int Max = INT_MAX;
};
template <>
class limits_base<uint>
{
public:
	static const uint Lowest = 0;
	static const uint Min = 0;
	static const uint Max = UINT_MAX;
};

template <>
class limits_base<int64>
{
public:
	static const int64 Lowest = LLONG_MIN;
	static const int64 Min = LLONG_MIN;
	static const int64 Max = LLONG_MAX;
};
template <>
class limits_base<uint64>
{
public:
	static const uint64 Lowest = 0;
	static const uint64 Min = 0;
	static const uint64 Max = ULLONG_MAX;
};

template <>
class limits_base<float>
{
public:
	static const float Lowest;
	static const float Min;
	static const float Max;
};
template <>
class limits_base<double>
{
public:
	static const double Lowest;
	static const double Min;
	static const double Max;
};

// -----basic operators-----

// safe_operators

class safe_operators
{
public:
	template <typename T>
	static call_result Add(IN const T& left, IN const T& right, OUT T& result) throw()
	{
		if( limits_base<T>::Max - left < right ) {
			return call_result(system_call_results::Overflow);
		}
		result = left + right;
		return call_result(system_call_results::OK);
	}

	template <typename T>
	static call_result Multiply(IN const T& left, IN const T& right, OUT T& result) throw()
	{
		//avoid divide 0
		if( left == 0 ) {
			result = 0;
			return call_result(system_call_results::OK);
		}
		if( limits_base<T>::Max / left < right ) {
			return call_result(system_call_results::Overflow);
		}
		result = left * right;
		return call_result(system_call_results::OK);
	}

	//throw version
	template <typename T>
	static T AddThrow(IN const T& left, IN const T& right)
	{
		T result;
		call_result cr = Add(left, right, result);
		if( cr.IsFailed() ) {
			throw exception_base(cr);
		}
		return result;
	}
	template <typename T>
	static T MultiplyThrow(IN const T& left, IN const T& right)
	{
		T result;
		call_result cr = Multiply(left, right, result);
		if( cr.IsFailed() ) {
			throw exception_base(cr);
		}
		return result;
	}
};

//special versions
template <>
inline call_result safe_operators::Multiply<int>(IN const int& left, IN const int& right, OUT int& result) throw()
{
	int64 result64 = static_cast<int64>(left) * static_cast<int64>(right);
	if( result64 > limits_base<int>::Max || result64 < limits_base<int>::Min ) {
		return call_result(system_call_results::Overflow);
	}
	result = static_cast<int>(result64);
	return call_result(system_call_results::OK);
}

template <>
inline call_result safe_operators::Multiply<uint>(IN const uint& left, IN const uint& right, OUT uint& result) throw()
{
	uint64 result64 = static_cast<uint64>(left) * static_cast<uint64>(right);
	if( result64 > limits_base<uint>::Max ) {
		return call_result(system_call_results::Overflow);
	}
	result = static_cast<uint>(result64);
	return call_result(system_call_results::OK);
}

// -----traits-----

// default_compare_trait<T>

template <typename T>
class default_compare_trait
{
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
		if( IsLT(t1, t2 ) )
			return -1;
		if( IsEQ(t1, t2) )
			return 0;
		assert( IsGT(t1, t2) );
		return 1;
	}
};

//special versions
#define _DECLARE_INT_COMPARE_TRAIT(T)  \
	template <> class default_compare_trait<T> {  \
	public:  \
	static bool IsEQ(const T& t1, const T& t2) throw() { return t1 == t2; }  \
	static bool IsNE(const T& t1, const T& t2) throw() { return t1 != t2; }  \
	static bool IsGT(const T& t1, const T& t2) throw() { return t1 > t2; }  \
	static bool IsLT(const T& t1, const T& t2) throw() { return t1 < t2; }  \
	static bool IsGE(const T& t1, const T& t2) throw() { return t1 >= t2; }  \
	static bool IsLE(const T& t1, const T& t2) throw() { return t1 <= t2; }  \
	static int Compare(const T& t1, const T& t2) throw() { return (int)(t1 - t2); }  \
	};

_DECLARE_INT_COMPARE_TRAIT(char)
_DECLARE_INT_COMPARE_TRAIT(byte)
_DECLARE_INT_COMPARE_TRAIT(short)
_DECLARE_INT_COMPARE_TRAIT(ushort)
_DECLARE_INT_COMPARE_TRAIT(int)
_DECLARE_INT_COMPARE_TRAIT(uint)
_DECLARE_INT_COMPARE_TRAIT(int64)
_DECLARE_INT_COMPARE_TRAIT(uint64)
_DECLARE_INT_COMPARE_TRAIT(bool)

template <>
class default_compare_trait<float>
{
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

// -----memory-----

#pragma pack(push, 1)

// i_memory_manager

class NOVTABLE i_memory_manager
{
public:
	virtual uintptr Allocate(const uintptr& uBytes) throw() = 0;
	// p == NULL : the same as Allocate
	// uBytes == 0 : free p, return NULL
	// return NULL, failed, and p unchanged
	virtual uintptr Reallocate(const uintptr& p, const uintptr& uBytes) throw() = 0;
	virtual void    Free(const uintptr& p) throw() = 0;
};

// i_memory_allocator_ref_32

class NOVTABLE i_memory_allocator_ref_32
{
public:
	virtual uint    Allocate(const uint& uBytes) throw() = 0;
	virtual uintptr ToPointer(const uint& p) throw() = 0;
};

// i_memory_allocator_ref_32_full

class NOVTABLE i_memory_allocator_ref_32_full : public i_memory_allocator_ref_32
{
public:
	// p == 0 : the same as Allocate
	// uBytes == 0 : free p, return 0
	// return 0, failed, and p unchanged
	virtual uint Reallocate(const uint& p, const uint& uBytes) throw() = 0;
	virtual void Free(const uint& p) throw() = 0;
};

// i_memory_allocator_ref_64

class NOVTABLE i_memory_allocator_ref_64
{
public:
	virtual int64   Allocate(const int64& uBytes) throw() = 0;
	virtual uintptr ToPointer(const int64& p) throw() = 0;
};

// i_memory_allocator_ref_64_full

class NOVTABLE i_memory_allocator_ref_64_full : public i_memory_allocator_ref_64
{
public:
	// p == 0 : the same as Allocate
	// uBytes == 0 : free p, return 0
	// return 0, failed, and p unchanged
	virtual int64 Reallocate(const int64& p, const int64& uBytes) throw() = 0;
	virtual void  Free(const int64& p) throw() = 0;
};

#pragma pack(pop)

// memory_allocator_ref_to_object<T, TAllocator, TPointer>
//   TAllocator : i_memory_allocator_ref_*
//   TPointer : uint or int64
template <typename T, class TAllocator, typename TPointer>
inline T& memory_allocator_ref_to_object(const ref_ptr<TAllocator>& allocator, const TPointer& p) throw()
{
	return *((T*)(const_cast<TAllocator&>(allocator.Deref()).ToPointer(p)));
}

// fixed_element_memory_pool

class fixed_element_memory_pool
{
public:
	fixed_element_memory_pool() throw() : m_pMgr(NULL), m_pHead(NULL)
	{
	}
	explicit fixed_element_memory_pool(i_memory_manager* pMgr) throw() : m_pMgr(pMgr), m_pHead(NULL)
	{
	}
	~fixed_element_memory_pool() throw()
	{
		FreeDataChain();
	}

//methods

	i_memory_manager* GetMemoryManager() const throw()
	{
		return m_pMgr;
	}
	void SetMemoryManager(i_memory_manager* pMgr) throw()
	{
		assert( pMgr != NULL && m_pMgr == NULL );
		m_pMgr = pMgr;
	}

	// free data links
	void FreeDataChain() throw()
	{
		if( m_pMgr == NULL ) {
			assert( m_pHead == NULL );
			return ;
		}
		void* pPlex = m_pHead;
		while( pPlex != NULL ) {
			void* pNext = get_next_block(pPlex);
			m_pMgr->Free((uintptr)pPlex);
			pPlex = pNext;
		}
		m_pHead = NULL;
	}

	// create block
	void* CreateBlock(uintptr uMinElements, uintptr uMaxElements, uintptr uElementSize, uintptr& uActElements)  //may throw
	{
		assert( uMinElements > 0 && uMaxElements > 0 && uMinElements <= uMaxElements && uElementSize > 0 );
		assert( m_pMgr != NULL );

		void* pPlex = NULL;
		uintptr uBytes = 0;
		uintptr uElements = uMaxElements;
		uintptr uLinkSize = sizeof(void*);

		uActElements = uElements;
		while( uElements >= uMinElements ) {
			//try
			if( safe_operators::Multiply<uintptr>(uElements, uElementSize, uBytes).IsFailed()
				|| safe_operators::Add<uintptr>(uBytes, uLinkSize, uBytes).IsFailed() ) {
				uElements --;
				continue;
			}
			break;
		}
		if( uElements < uMinElements )
			throw overflow_exception();

		while( uElements >= uMinElements ) {
			//no overflow
			uBytes = uElements * uElementSize + sizeof(void*);
			pPlex = (void*)(m_pMgr->Allocate(uBytes));
			if( pPlex == NULL ) {
				uElements --;
				continue;
			}
			break;
		}
		if( pPlex == NULL )
			throw outofmemory_exception();
		uActElements = uElements;

		get_next_block(pPlex) = m_pHead;
		m_pHead = pPlex;

		return get_data(pPlex);
	}

private:
	static void*& get_next_block(void* pBlock) throw()
	{
		assert( pBlock != NULL );
		return *((void**)pBlock);
	}
	static void* get_data(void* pBlock) throw()
	{
		return (void*)(((void**)pBlock) + 1);
	}

private:
	i_memory_manager* m_pMgr;
	void* m_pHead;

private:
	//non-copyable
	fixed_element_memory_pool(const fixed_element_memory_pool& src) throw();
	fixed_element_memory_pool& operator=(const fixed_element_memory_pool& src) throw();
};

// free_list<TNode>
//  TNode : has a member named m_pNext (TNode*)

template <class TNode>
class free_list
{
public:
	explicit free_list(i_memory_manager* pMgr = NULL, uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
					: m_pool(pMgr), m_pFree(NULL), m_uMinBlockElements(uMinElements), m_uMaxBlockElements(uMaxElements)
	{
		assert( uMinElements > 0 && uMaxElements > 0 && uMinElements <= uMaxElements );
	}
	~free_list() throw()
	{
	}

//methods

	i_memory_manager* GetMemoryManager() const throw()
	{
		return m_pool.GetMemoryManager();
	}
	void SetMemoryManager(i_memory_manager* pMgr) throw()
	{
		m_pool.SetMemoryManager(pMgr);
	}

	void Clear() throw()
	{
		m_pFree = NULL;
		m_pool.FreeDataChain();
	}
	//get a free node
	TNode* GetFreeNode()  //may throw
	{
		if( m_pFree == NULL ) {
			uintptr uActElements;
			//may throw
			TNode* pNode = (TNode*)m_pool.CreateBlock(m_uMinBlockElements, m_uMaxBlockElements, sizeof(TNode), uActElements);
			pNode += (uActElements - 1);
			for( uintptr uBlock = uActElements; uBlock > 0; uBlock -- ) {
				pNode->m_pNext = m_pFree;
				m_pFree = pNode;
				pNode --;
			}
		}
		assert( m_pFree != NULL );
		return m_pFree;
	}
	//pick free node
	void PickFreeNode() throw()
	{
		if( m_pFree != NULL )
			m_pFree = m_pFree->m_pNext;
	}
	void PutFreeNode(TNode* pNode) throw()
	{
		pNode->m_pNext = m_pFree;
		m_pFree = pNode;
	}

private:
	//pool
	fixed_element_memory_pool  m_pool;
	TNode*  m_pFree;  //free list
	uintptr m_uMinBlockElements, m_uMaxBlockElements;

private:
	//noncopyable
	free_list(const free_list&) throw();
	free_list& operator=(const free_list&) throw();
};

// node_pair_helper<TNode, TPair>

template <typename TNode, typename TPair>
class node_pair_helper
{
public:
	//tools
	static TNode* ConstructNode(free_list<TNode>& fl)  //may throw
	{
		TNode* pNewNode = fl.GetFreeNode();
		call_constructor(*pNewNode);  //may throw
		fl.PickFreeNode();
		return pNewNode;
	}
	template <typename TKey>
	static TNode* ConstructNode(free_list<TNode>& fl, const TKey& key)  //may throw
	{
		TNode* pNewNode = fl.GetFreeNode();
		call_constructor(*pNewNode, key);  //may throw
		fl.PickFreeNode();
		return pNewNode;
	}
	template <typename TKey>
	static TNode* ConstructNode(free_list<TNode>& fl, TKey&& key)  //may throw
	{
		TNode* pNewNode = fl.GetFreeNode();
		call_constructor(*pNewNode, rv_forward(key));  //may throw
		fl.PickFreeNode();
		return pNewNode;
	}
	template <typename TKey, typename TValue>
	static TNode* ConstructNode(free_list<TNode>& fl, const TKey& key, const TValue& val)  //may throw
	{
		TNode* pNewNode = fl.GetFreeNode();
		call_constructor(*pNewNode, key, val);  //may throw
		fl.PickFreeNode();
		return pNewNode;
	}
	template <typename TKey, typename TValue>
	static TNode* ConstructNode(free_list<TNode>& fl, TKey&& key, TValue&& val)  //may throw
	{
		TNode* pNewNode = fl.GetFreeNode();
		call_constructor(*pNewNode, rv_forward(key), rv_forward(val));  //may throw
		fl.PickFreeNode();
		return pNewNode;
	}
	static TNode* ConstructNode(free_list<TNode>& fl, const TPair& pair)  //may throw
	{
		TNode* pNewNode = fl.GetFreeNode();
		call_constructor(*pNewNode, pair);  //may throw
		fl.PickFreeNode();
		return pNewNode;
	}
	static TNode* ConstructNode(free_list<TNode>& fl, TPair&& pair)  //may throw
	{
		TNode* pNewNode = fl.GetFreeNode();
		call_constructor(*pNewNode, rv_forward(pair));  //may throw
		fl.PickFreeNode();
		return pNewNode;
	}
	static void DestructNode(free_list<TNode>& fl, TNode* pNode, uintptr& uElements) throw()
	{
		assert( pNode != NULL );
		pNode->~TNode();
		fl.PutFreeNode(pNode);
		assert( uElements > 0 );
		uElements --;
		if( uElements == 0 )
			fl.Clear();
	}
};

// fixed_size_memory_pool<t_size>

template <uintptr t_size>
class fixed_size_memory_pool
{
private:
	struct _Node
	{
		_Node* m_pNext;
		char   m_data[t_size];
	};

public:
	explicit fixed_size_memory_pool(i_memory_manager* pMgr = NULL, uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
									: m_uElements(0), m_freelist(pMgr, uMinElements, uMaxElements)
	{
	}
	~fixed_size_memory_pool() throw()
	{
	}

//methods

	i_memory_manager* GetMemoryManager() const throw()
	{
		return m_freelist.GetMemoryManager();
	}
	void SetMemoryManager(i_memory_manager* pMgr) throw()
	{
		m_freelist.SetMemoryManager(pMgr);
	}

	void* Allocate() //may throw
	{
		_Node* pNode = m_freelist.GetFreeNode();  //may throw
		m_freelist.PickFreeNode();

		m_uElements ++;
		assert( m_uElements > 0 );

		return (void*)(pNode->m_data);  //data
	}
	void Free(void* p) throw()
	{
		//back to node
		_Node* pNode = (_Node*)((byte*)p - (intptr)((byte*)(((_Node*)(0x128))->m_data) - (byte*)(0x128)));
		m_freelist.PutFreeNode(pNode);

		assert( m_uElements > 0 );
		m_uElements --;
		if( m_uElements == 0 )
			m_freelist.Clear();
	}

	void Clear() throw()
	{
		m_freelist.Clear();
		m_uElements = 0;
	}

private:
	uintptr m_uElements;

	free_list<_Node>  m_freelist;

private:
	//noncopyable
	fixed_size_memory_pool(const fixed_size_memory_pool&) throw();
	fixed_size_memory_pool& operator=(const fixed_size_memory_pool&) throw();
};

// for share object

#pragma pack(push, 1)

// share_block_base

class share_block_base
{
public:
	share_block_base() throw() : m_pMgr(NULL), m_p(NULL), m_shareCount(1), m_weakCount(1)
	{
	}
	~share_block_base() throw()
	{
	}

	//methods
	i_memory_manager* GetMemoryManager() const throw()
	{
		return m_pMgr;
	}
	void SetMemoryManager(i_memory_manager* pMgr) throw()
	{
		m_pMgr = pMgr;
	}
	//object pointer
	void* GetAddress() const throw()
	{
		return m_p;
	}
	void SetAddress(void* p) throw()
	{
		m_p = p;
	}
	bool IsNull() const throw()
	{
		return m_p == NULL;
	}

	int AddRefCopy() throw()
	{
		return atomic_increment((int&)m_shareCount);
	}
	bool AddRefLock() throw()
	{
		for( ; ; ) {
			int tmp = m_shareCount;
			if( tmp <= 0 )
				return false;
			if( atomic_compare_exchange((int&)m_shareCount, tmp, tmp + 1) == tmp )
				return true;
		}
		return false;
	}
	int Release() throw()
	{
		return atomic_decrement((int&)m_shareCount);
	}

	int WeakAddRef() throw()
	{
		return atomic_increment((int&)m_weakCount);
	}
	int WeakRelease() throw()
	{
		return atomic_decrement((int&)m_weakCount);
	}

	int GetShareCount() const throw()
	{
		return m_shareCount;
	}
	int GetWeakCount() const throw()
	{
		return m_weakCount;
	}

protected:
	i_memory_manager* m_pMgr;
	void* m_p;
	volatile int m_shareCount;
	volatile int m_weakCount;

private:
	//noncopyable
	share_block_base(const share_block_base&) throw();
	share_block_base& operator=(const share_block_base&) throw();
};

typedef void  (* share_object_destruction_func)(void* p);

// share_ptr_block

class share_ptr_block : public share_block_base
{
public:
	share_ptr_block() throw() : m_pDestruction(NULL)
	{
	}
	~share_ptr_block() throw()
	{
	}

	void SetDestructionFunc(share_object_destruction_func pFunc) throw()
	{
		m_pDestruction = pFunc;
	}

	//destroy
	void DestroyObject() throw()
	{
		assert( m_pDestruction != NULL );
		assert( m_pMgr != NULL );
		if( m_p != NULL ) {
			//destruction
			m_pDestruction(m_p);
			//free
			m_pMgr->Free((uintptr)m_p);
			m_p = NULL;
		}
	}

protected:
	share_object_destruction_func m_pDestruction;

private:
	//noncopyable
	share_ptr_block(const share_ptr_block&) throw();
	share_ptr_block& operator=(const share_ptr_block&) throw();
};

// share_array_block

class share_array_block : public share_block_base
{
public:
	share_array_block() throw() : m_uLength(0), m_uAllocLength(0)
	{
	}
	~share_array_block() throw()
	{
	}

	uintptr GetLength() const throw()
	{
		return m_uLength;
	}
	void SetLength(uintptr uLength) throw()
	{
		m_uLength = uLength;
	}
	uintptr GetAllocLength() const throw()
	{
		return m_uAllocLength;
	}
	void SetAllocLength(uintptr uAllocLength) throw()
	{
		m_uAllocLength = uAllocLength;
	}

	//destroy
	template <typename T>
	void DestroyArray() throw()
	{
		assert( m_pMgr != NULL );
		if( m_p == NULL ) {
			assert( m_uLength == 0 && m_uAllocLength == 0 );
			return ;
		}
		//destruction
		T* pt = (T*)m_p;
		for( uintptr i = 0; i < m_uLength; i ++ ) {
			pt->~T();
			++ pt;
		}
		//free
		m_pMgr->Free((uintptr)m_p);
		m_p = NULL;
		m_uLength = 0;
		m_uAllocLength = 0;
	}

protected:
	uintptr m_uLength;
	uintptr m_uAllocLength;

private:
	//noncopyable
	share_array_block(const share_array_block&) throw();
	share_array_block& operator=(const share_array_block&) throw();
};

typedef void* (* share_object_typecast_func)(void* p, const guid& iid);
typedef void* (* share_object_connection_func)(void* p, const guid& iid);

// share_com_block

class share_com_block : public share_ptr_block
{
public:
	share_com_block() throw() : m_pTypeCast(NULL), m_pConnection(NULL)
	{
	}
	~share_com_block() throw()
	{
	}

	share_object_typecast_func GetTypeCastFunc() const throw()
	{
		return m_pTypeCast;
	}
	void SetTypeCastFunc(share_object_typecast_func pFunc) throw()
	{
		m_pTypeCast = pFunc;
	}
	share_object_connection_func GetConnectionFunc() const throw()
	{
		return m_pConnection;
	}
	void SetConnectionFunc(share_object_connection_func pFunc) throw()
	{
		m_pConnection = pFunc;
	}

protected:
	share_object_typecast_func m_pTypeCast;
	share_object_connection_func m_pConnection;

private:
	share_com_block(const share_com_block&) throw();
	share_com_block& operator=(const share_com_block&) throw();
};

#pragma pack(pop)

// -----File-----

// file_open_types

BEGIN_ENUM(file_open_types)
	ENUM_VALUE_ENTRY(Read,       0x00000000)
	ENUM_VALUE_ENTRY(Write,      0x00000001)
	ENUM_VALUE_ENTRY(ReadWrite,  0x00000002)
END_ENUM()

// file_creation_types
//   they can combine with <or> operation
BEGIN_ENUM(file_creation_types)
	ENUM_VALUE_ENTRY(Create,     0x00001000)
	ENUM_VALUE_ENTRY(NoTruncate, 0x00002000)
END_ENUM()

#pragma pack(push, 1)

// storage_status

struct storage_status
{
	int64        iSize;     //storage size in bytes (such as file, memory)
	system_time  tmAccess;  //time of last access
	system_time  tmModify;  //time of last modification
	system_time  tmCreate;  //time of creation
};

#pragma pack(pop)

// -----collection-----

// coll_replace_elements<TIterator, TCompareTrait>

template <class TIterator, class TCompareTrait = default_compare_trait<typename TIterator::EType>>
inline uintptr coll_replace_elements(const typename TIterator::EType& tOld, const typename TIterator::EType& tNew,
									TIterator& iterB, TIterator& iterE)
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
	const_string_t(const Tchar* p, uintptr size) throw() : baseClass(p, size)
	{
	}
	const_string_t(const thisClass& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	explicit const_string_t(const const_array_base<Tchar>& src) throw() : baseClass(src)
	{
	}
	~const_string_t() throw()
	{
	}

	//operators
	thisClass& operator=(const thisClass& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
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
static const char_type array_name[] = x;  \
static const uintptr len_name = sizeof(array_name) / sizeof(char_type) - 1;

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

// const_string_helper

class const_string_helper
{
public:
	//To C-style string
	template <typename Tchar>
	static ref_ptr<Tchar> To_C_Style(const const_string_t<Tchar>& str, uintptr uStart = 0) throw()
	{
		assert( uStart <= str.GetCount() );
		return ref_ptr<Tchar>(const_array_helper::GetInternalPointer(str) + uStart);
	}

	//find (return value : check null)
	template <typename Tchar>
	static typename const_string_t<Tchar>::Iterator Find(const const_string_t<Tchar>& str, const Tchar& ch, uintptr uStart) throw()
	{
		assert( uStart <= str.GetCount() );
		assert( !str.IsNull() );
		return typename const_string_t<Tchar>::Iterator(ref_ptr<Tchar>(find_string_char(const_array_helper::GetInternalPointer(str) + uStart, ch)));
	}
};

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

// -----unique pointers-----

// unique_ptr<T>

template <typename T>
class unique_ptr
{
public:
	unique_ptr() throw() : m_p(NULL)
	{
	}
	unique_ptr(unique_ptr<T>&& src) throw()
	{
		m_p = src.m_p;
		src.m_p = NULL;
	}
	~unique_ptr() throw()
	{
		Release();
	}

	unique_ptr<T>& operator=(unique_ptr<T>&& src) throw()
	{
		if( this != &src ) {
			if( m_p == src.m_p ) {
				assert( m_p == NULL );
			}
			else {
				Release();
				m_p = src.m_p;
				src.m_p = NULL;
			}
		}
		return *this;
	}

	void Release() throw()
	{
		if( m_p != NULL ) {
			m_p->~T();
			crt_free(m_p);
			m_p = NULL;
		}
	}

	bool IsNull() const throw()
	{
		return m_p == NULL;
	}

	const T& Deref() const throw()
	{
		assert( !IsNull() );
		return *m_p;
	}
	T& Deref() throw()
	{
		assert( !IsNull() );
		return *m_p;
	}

protected:
	T* m_p;

private:
	unique_ptr(const unique_ptr&) throw();
	unique_ptr& operator=(const unique_ptr&) throw();

private:
	friend class unique_ptr_helper;
};

// unique_ptr_helper

class unique_ptr_helper
{
public:
	template <typename T, typename... Args>
	static unique_ptr<T> MakeUniquePtr(Args&&... args)
	{
		T* p = (T*)crt_alloc(sizeof(T));
		if( p == NULL )
			throw outofmemory_exception();
		try {
			call_constructor(*p, rv_forward<Args>(args)...);
		}
		catch(...) {
			crt_free(p);
			throw;
		}
		unique_ptr<T> ret;
		ret.m_p = p;
		return ret;
	}

	template <typename T>
	static T* GetInternalPointer(const unique_ptr<T>& sp) throw()
	{
		return const_cast<T*>(sp.m_p);
	}
};

// unique_array<T>

template <typename T>
class unique_array
{
public:
	typedef array_position  Position;
	typedef array_iterator<T>  Iterator;

public:
	unique_array() throw() : m_p(NULL), m_uCount(0)
	{
	}
	unique_array(unique_array<T>&& src) throw()
	{
		m_p = src.m_p;
		src.m_p = NULL;
		m_uCount = src.m_uCount;
		src.m_uCount = 0;
	}
	~unique_array() throw()
	{
		Free();
	}

	unique_array<T>& operator=(unique_array<T>&& src) throw()
	{
		if( this != &src ) {
			if( m_p == src.m_p ) {
				assert( m_p == NULL );
			}
			else {
				Free();
				m_p = src.m_p;
				src.m_p = NULL;
				m_uCount = src.m_uCount;
				src.m_uCount = 0;
			}
		}
		return *this;
	}

	const Iterator operator[](uintptr uIndex) const throw()
	{
		return GetAt(uIndex);
	}
	Iterator operator[](uintptr uIndex) throw()
	{
		return GetAt(uIndex);
	}

	uintptr GetCount() const throw()
	{
		return m_p == NULL ? 0 : m_uCount;
	}
	bool IsNull() const throw()
	{
		return m_p == NULL;
	}

	void Free() throw()
	{
		if( m_p != NULL ) {
			call_destructors(m_p, m_uCount);
			crt_free(m_p);
			m_p = NULL;
		}
	}

	//position
	Position GetStartPosition() const throw()
	{
		return Position(0);
	}
	Position GetTailPosition() const throw()
	{
		return Position(GetCount() - 1);
	}
	const Iterator GetAtPosition(const Position& pos) const throw()
	{
		return Iterator(ref_ptr<T>(m_p + pos.GetIndex()));
	}
	Iterator GetAtPosition(const Position& pos) throw()
	{
		return Iterator(ref_ptr<T>(m_p + pos.GetIndex()));
	}
	Position ToPosition(const Iterator& iter) const throw()
	{
		return Position(ref_ptr_helper::GetInternalPointer(iter.get_Ref()) - m_p);
	}

	//iterator
	const Iterator GetBegin() const throw()
	{
		return Iterator(ref_ptr<T>(m_p));
	}
	Iterator GetBegin() throw()
	{
		return Iterator(ref_ptr<T>(m_p));
	}
	const Iterator GetEnd() const throw()
	{
		return Iterator(ref_ptr<T>(m_p + m_uCount));
	}
	Iterator GetEnd() throw()
	{
		return Iterator(ref_ptr<T>(m_p + m_uCount));
	}

	const Iterator GetAt(uintptr uIndex) const throw()
	{
		assert( uIndex < GetCount() );
		return Iterator(ref_ptr<T>(m_p + uIndex));
	}
	Iterator GetAt(uintptr uIndex) throw()
	{
		assert( uIndex < GetCount() );
		return Iterator(ref_ptr<T>(m_p + uIndex));
	}
	void SetAt(uintptr uIndex, const T& t)
	{
		assert( uIndex < GetCount() );
		m_p[uIndex] = t;  //may throw
	}
	void SetAt(uintptr uIndex, T&& t)
	{
		assert( uIndex < GetCount() );
		m_p[uIndex] = rv_forward(t);  //may throw
	}

protected:
	static void call_destructors(T* p, uintptr uCount) throw()
	{
		T* pt = p;
		for( uintptr i = 0; i < uCount; i ++ ) {
			pt->~T();
			++ pt;
		}
	}

protected:
	T*       m_p;
	uintptr  m_uCount;

private:
	unique_array(const unique_array&) throw();
	unique_array& operator=(const unique_array&) throw();

private:
	friend class unique_array_helper;
};

// unique_array_helper

class unique_array_helper
{
public:
	template <typename T, typename... Args>
	static unique_array<T> MakeUniqueArray(uintptr uCount, Args&&... args)
	{
		assert( uCount > 0 );
		uintptr uBytes = safe_operators::MultiplyThrow(uCount, (uintptr)sizeof(T));
		T* p = (T*)crt_alloc(uBytes);
		if( p == NULL )
			throw outofmemory_exception();
		uintptr i;
		try {
			for( i = 0; i < uCount; i ++ ) {
				call_constructor(*(p + i), rv_forward<Args>(args)...);
			}
		}
		catch(...) {
			while( i > 0 ) {
				i --;
				p[i].~T();
			}
			crt_free(p);
			throw;  //re-throw
		}
		unique_array<T> ret;
		ret.m_p = p;
		ret.m_uCount = uCount;
		return ret;
	}

	template <typename T>
	static T* GetInternalPointer(const unique_array<T>& sp) throw()
	{
		return const_cast<T*>(sp.m_p);
	}
};

////////////////////////////////////////////////////////////////////////////////
