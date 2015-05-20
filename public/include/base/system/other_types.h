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

#if defined(OS_WINDOWS)
	#include "Windows/other_types.h"
#elif defined(OS_LINUX)
	#include "Linux/other_types.h"
#else
	#error Error OS type!
#endif

//------------------------------------------------------------------------------
//functions

//memory
inline uintptr crt_alloc(const uintptr& uBytes) throw()
{
	return (uintptr)::malloc(uBytes);
}

inline uintptr crt_realloc(const uintptr& p, const uintptr& uBytes) throw()
{
	return (uintptr)::realloc((void*)p, uBytes);
}

inline void    crt_free(const uintptr& p) throw()
{
	::free((void*)p);
}

//------------------------------------------------------------------------------
//classes

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
#if defined(OS_WINDOWS)
	static CharH Swap(CharH x) throw()
	{
		return (CharH)Swap((ushort)x);
	}
#elif defined(OS_LINUX)
	//static CharH Swap(CharH x) throw()
	//  the same as : static ushort Swap(ushort x) throw()
#else
	#error Error OS type!
#endif

	static CharL Swap(CharL x) throw()
	{
		return (CharL)Swap((uint)x);
	}
	//Swap a float's byte order
	static float Swap(float x) throw()
	{
		uint v = Swap(*((uint*)&x));
		return *((float*)&v);
	}
	static double Swap(double x) throw()
	{
		uint64 v = Swap(*((uint64*)&x));
		return *((double*)&v);
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

////////////////////////////////////////////////////////////////////////////////
