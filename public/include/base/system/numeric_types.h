/*
** Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
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
//number

// limits_base<T>

template <typename T>
class limits_base;

/*
signed : (((T)1)<<(bits-1)) --- ~(((T)1)<<(bits-1))
unsigned : 0 --- ~((T)0)
*/

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

//------------------------------------------------------------------------------
//float

// float_helper

class float_helper
{
public:
	//T: float or double
	template <typename T>
	static bool IsNan(T t) throw()
	{
		return ::fpclassify(t) == FP_NAN;
	}
	template <typename T>
	static bool IsInf(T t) throw()
	{
		return ::fpclassify(t) == FP_INFINITE;
	}
	template <typename T>
	static bool IsFinite(T t) throw()
	{
		int c = ::fpclassify(t);
		return c != FP_NAN && c != FP_INFINITE;
	}

	static float Ceil(float t) throw()
	{
		return ::ceilf(t);
	}
	static double Ceil(double t) throw()
	{
		return ::ceil(t);
	}
	static float Floor(float t) throw()
	{
		return ::floorf(t);
	}
	static double Floor(double t) throw()
	{
		return ::floor(t);
	}
	static float Round(float t) throw()
	{
		return ::roundf(t);
	}
	static double Round(double t) throw()
	{
		return ::round(t);
	}
	static float Trunc(float t) throw()
	{
		return ::truncf(t);
	}
	static double Trunc(double t) throw()
	{
		return ::trunc(t);
	}
};

//------------------------------------------------------------------------------
//max & min

// numeric_max

template <typename T>
inline T numeric_max(T t1, T t2) throw()
{
	return t1 > t2 ? t1 : t2;
}

template <>
inline float numeric_max<float>(float t1, float t2) throw()
{
	return ::fmaxf(t1, t2);
}
template <>
inline double numeric_max<double>(double t1, double t2) throw()
{
	return ::fmax(t1, t2);
}

// numeric_min

template <typename T>
inline T numeric_min(T t1, T t2) throw()
{
	return t1 < t2 ? t1 : t2;
}

template <>
inline float numeric_min<float>(float t1, float t2) throw()
{
	return ::fminf(t1, t2);
}
template <>
inline double numeric_min<double>(double t1, double t2) throw()
{
	return ::fmin(t1, t2);
}

//------------------------------------------------------------------------------
//cast

// numeric_cast

template <typename T, typename U>
inline call_result numeric_cast(T t, U& u) throw();

template <typename T, typename U>
inline call_result _numeric_cast_direct(T t, U& u) throw()
{
	u = (U)t;
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_cast_lt_zero(T t, U& u) throw()
{
	if( t < (T)0 )
		return call_result(system_call_results::Overflow);
	u = (U)t;
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_cast_gt_max(T t, U& u) throw()
{
	if( t > (T)(limits_base<U>::Max) )
		return call_result(system_call_results::Overflow);
	u = (U)t;
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_cast_signed_minmax(T t, U& u) throw()
{
	if( t > (T)(limits_base<U>::Max) || t < (T)(limits_base<U>::Lowest) )
		return call_result(system_call_results::Overflow);
	u = (U)t;
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_cast_unsigned_minmax(T t, U& u) throw()
{
	if( t > (T)(limits_base<U>::Max) || t < (T)(limits_base<U>::Min) )
		return call_result(system_call_results::Overflow);
	u = (U)t;
	return call_result();
}

#define _DECLARE_FUNC_NUMERIC_CAST_DIRECT(src_type, dest_type)  \
	template <> inline call_result numeric_cast<src_type, dest_type>(src_type t, dest_type& u) throw()  \
	{ return _numeric_cast_direct<src_type, dest_type>(t, u); }
#define _DECLARE_FUNC_NUMERIC_CAST_LT_ZERO(src_type, dest_type)  \
	template <> inline call_result numeric_cast<src_type, dest_type>(src_type t, dest_type& u) throw()  \
	{ return _numeric_cast_lt_zero<src_type, dest_type>(t, u); }
#define _DECLARE_FUNC_NUMERIC_CAST_GT_MAX(src_type, dest_type)  \
	template <> inline call_result numeric_cast<src_type, dest_type>(src_type t, dest_type& u) throw()  \
	{ return _numeric_cast_gt_max<src_type, dest_type>(t, u); }
#define _DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(src_type, dest_type)  \
	template <> inline call_result numeric_cast<src_type, dest_type>(src_type t, dest_type& u) throw()  \
	{ return _numeric_cast_signed_minmax<src_type, dest_type>(t, u); }
#define _DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(src_type, dest_type)  \
	template <> inline call_result numeric_cast<src_type, dest_type>(src_type t, dest_type& u) throw()  \
	{ return _numeric_cast_unsigned_minmax<src_type, dest_type>(t, u); }

//char
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(byte, char)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(short, char)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(ushort, char)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(int, char)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(uint, char)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(int64, char)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(uint64, char)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(float, char)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(double, char)
//byte
_DECLARE_FUNC_NUMERIC_CAST_LT_ZERO(char, byte)
_DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(short, byte)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(ushort, byte)
_DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(int, byte)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(uint, byte)
_DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(int64, byte)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(uint64, byte)
_DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(float, byte)
_DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(double, byte)
//short
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(char, short)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(byte, short)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(ushort, short)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(int, short)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(uint, short)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(int64, short)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(uint64, short)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(float, short)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(double, short)
//ushort
_DECLARE_FUNC_NUMERIC_CAST_LT_ZERO(char, ushort)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(byte, ushort)
_DECLARE_FUNC_NUMERIC_CAST_LT_ZERO(short, ushort)
_DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(int, ushort)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(uint, ushort)
_DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(int64, ushort)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(uint64, ushort)
_DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(float, ushort)
_DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(double, ushort)
//int
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(char, int)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(byte, int)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(short, int)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(ushort, int)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(uint, int)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(int64, int)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(uint64, int)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(float, int)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(double, int)
//uint
_DECLARE_FUNC_NUMERIC_CAST_LT_ZERO(char, uint)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(byte, uint)
_DECLARE_FUNC_NUMERIC_CAST_LT_ZERO(short, uint)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(ushort, uint)
_DECLARE_FUNC_NUMERIC_CAST_LT_ZERO(int, uint)
_DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(int64, uint)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(uint64, uint)
_DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(float, uint)
_DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(double, uint)
//int64
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(char, int64)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(byte, int64)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(short, int64)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(ushort, int64)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(int, int64)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(uint, int64)
_DECLARE_FUNC_NUMERIC_CAST_GT_MAX(uint64, int64)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(float, int64)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(double, int64)
//uint64
_DECLARE_FUNC_NUMERIC_CAST_LT_ZERO(char, uint64)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(byte, uint64)
_DECLARE_FUNC_NUMERIC_CAST_LT_ZERO(short, uint64)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(ushort, uint64)
_DECLARE_FUNC_NUMERIC_CAST_LT_ZERO(int, uint64)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(uint, uint64)
_DECLARE_FUNC_NUMERIC_CAST_LT_ZERO(int64, uint64)
_DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(float, uint64)
_DECLARE_FUNC_NUMERIC_CAST_UNSIGNED_MINMAX(double, uint64)
//float
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(char, float)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(byte, float)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(short, float)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(ushort, float)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(int, float)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(uint, float)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(int64, float)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(uint64, float)
_DECLARE_FUNC_NUMERIC_CAST_SIGNED_MINMAX(double, float)
//double
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(char, double)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(byte, double)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(short, double)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(ushort, double)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(int, double)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(uint, double)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(int64, double)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(uint64, double)
_DECLARE_FUNC_NUMERIC_CAST_DIRECT(float, double)

//------------------------------------------------------------------------------
//arithmetic operators

// absolute

template <typename T, typename U>
inline U numeric_absolute(T t) throw();

template <typename T, typename U>
inline U _numeric_absolute_u(T t) throw()
{
	return t;
}
template <typename T, typename U>
inline U _numeric_absolute_s(T t) throw()
{
	return t >= 0 ? (U)t : (U)(-t);
}
template <typename T, typename U>
inline U _numeric_absolute_f(T t) throw()
{
	return (U)::fabsf(t);
}
template <typename T, typename U>
inline U _numeric_absolute_d(T t) throw()
{
	return (U)::fabs(t);
}

#define _DECLARE_FUNC_NUMERIC_ABSOLUTE_U(type1, type2)  \
	template <> inline type2 numeric_absolute<type1, type2>(type1 t) throw()  \
	{ return _numeric_absolute_u<type1, type2>(t); }
#define _DECLARE_FUNC_NUMERIC_ABSOLUTE_S(type1, type2)  \
	template <> inline type2 numeric_absolute<type1, type2>(type1 t) throw()  \
	{ return _numeric_absolute_s<type1, type2>(t); }
#define _DECLARE_FUNC_NUMERIC_ABSOLUTE_F(type1, type2)  \
	template <> inline type2 numeric_absolute<type1, type2>(type1 t) throw()  \
	{ return _numeric_absolute_f<type1, type2>(t); }
#define _DECLARE_FUNC_NUMERIC_ABSOLUTE_D(type1, type2)  \
	template <> inline type2 numeric_absolute<type1, type2>(type1 t) throw()  \
	{ return _numeric_absolute_d<type1, type2>(t); }

_DECLARE_FUNC_NUMERIC_ABSOLUTE_S(char, byte)
_DECLARE_FUNC_NUMERIC_ABSOLUTE_U(byte, byte)
_DECLARE_FUNC_NUMERIC_ABSOLUTE_S(short, ushort)
_DECLARE_FUNC_NUMERIC_ABSOLUTE_U(ushort, ushort)
_DECLARE_FUNC_NUMERIC_ABSOLUTE_S(int, uint)
_DECLARE_FUNC_NUMERIC_ABSOLUTE_U(uint, uint)
_DECLARE_FUNC_NUMERIC_ABSOLUTE_S(int64, uint64)
_DECLARE_FUNC_NUMERIC_ABSOLUTE_U(uint64, uint64)
_DECLARE_FUNC_NUMERIC_ABSOLUTE_F(float, float)
_DECLARE_FUNC_NUMERIC_ABSOLUTE_D(double, double)

// negative to positive

template <typename T, typename U>
inline U numeric_n2p(T t) throw();

template <typename T, typename U>
inline U _numeric_n2p_i(T t) throw()
{
	assert( t < 0 );
	return (U)(-t);
}
template <typename T, typename U>
inline U _numeric_n2p_f_d(T t) throw()
{
	assert( default_compare_trait<T>::IsLT(t, (T)0) );
	return (U)(-t);
}

#define _DECLARE_FUNC_NUMERIC_N2P_I(type1, type2)  \
	template <> inline type2 numeric_n2p<type1, type2>(type1 t) throw()  \
	{ return _numeric_n2p_i<type1, type2>(t); }
#define _DECLARE_FUNC_NUMERIC_N2P_F_D(type1, type2)  \
	template <> inline type2 numeric_n2p<type1, type2>(type1 t) throw()  \
	{ return _numeric_n2p_f_d<type1, type2>(t); }

_DECLARE_FUNC_NUMERIC_N2P_I(char, byte)
_DECLARE_FUNC_NUMERIC_N2P_I(short, ushort)
_DECLARE_FUNC_NUMERIC_N2P_I(int, uint)
_DECLARE_FUNC_NUMERIC_N2P_I(int64, uint64)
_DECLARE_FUNC_NUMERIC_N2P_F_D(float, float)
_DECLARE_FUNC_NUMERIC_N2P_F_D(double, double)

// negation

template <typename T>
inline call_result numeric_negative(T t, T& r) throw()
{
	//signed
	if( t == (T)(limits_base<T>::Lowest) )
		return call_result(system_call_results::Overflow);
	r = -t;
	return call_result();
}

template <typename T>
inline call_result _numeric_negative_direct(T t, T& r) throw()
{
	r = -t;
	return call_result();
}
template <typename T>
inline call_result _numeric_negative_unsigned(T t, T& r) throw()
{
	return call_result(system_call_results::Overflow);
}

#define _DECLARE_FUNC_NUMERIC_NEGATIVE_DIRECT(type)  \
	template <> inline call_result numeric_negative<type>(type t, type& r) throw()  \
	{ return _numeric_negative_direct<type>(t, r); }
#define _DECLARE_FUNC_NUMERIC_NEGATIVE_UNSIGNED(type)  \
	template <> inline call_result numeric_negative<type>(type t, type& r) throw()  \
	{ return _numeric_negative_unsigned<type>(t, r); }

_DECLARE_FUNC_NUMERIC_NEGATIVE_UNSIGNED(byte)
_DECLARE_FUNC_NUMERIC_NEGATIVE_UNSIGNED(ushort)
_DECLARE_FUNC_NUMERIC_NEGATIVE_UNSIGNED(uint)
_DECLARE_FUNC_NUMERIC_NEGATIVE_UNSIGNED(uint64)
_DECLARE_FUNC_NUMERIC_NEGATIVE_DIRECT(float)
_DECLARE_FUNC_NUMERIC_NEGATIVE_DIRECT(double)

// modulus

template <typename T, typename U>
inline call_result numeric_modulus(T t, U u, T& r) throw();

template <typename T, typename U>
inline call_result _numeric_modulus_uu_s4u2(T t, U u, T& r) throw()
{
	if( u == 0 )
		return call_result(system_call_results::Invalid);
	r = (T)(t % u);
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_modulus_ss_u2s4(T t, U u, T& r) throw()
{
	if( u == 0 )
		return call_result(system_call_results::Invalid);
	if( u == (U)(-1) ) {
		r = 0;
		return call_result();
	}
	r = (T)(t % u);
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_modulus_s08u4_s4u04(T t, U u, T& r) throw()
{
	if( u == 0 )
		return call_result(system_call_results::Invalid);
	r = (T)((int64)t % (int64)u);
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_modulus_u4s08_u04s4(T t, U u, T& r) throw()
{
	if( u == 0 )
		return call_result(system_call_results::Invalid);
	if( u == (U)(-1) ) {
		r = 0;
		return call_result();
	}
	r = (T)((int64)t % (int64)u);
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_modulus_u08s8(T t, U u, T& r) throw()
{
	if( u == 0 )
		return call_result(system_call_results::Invalid);
	if( u < 0 )
		r = (T)(t % (uint64)numeric_n2p(u));
	else
		r = (T)(t % (uint64)u);
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_modulus_s8u08(T t, U u, T& r) throw()
{
	if( u == 0 )
		return call_result(system_call_results::Invalid);
	if( t < 0 )
		r = -(T)((uint64)numeric_n2p(t) % u);
	else
		r = (T)((uint64)t % u);
	return call_result();
}

template <typename T, typename U>
inline call_result _numeric_modulus_f(T t, U u, T& r) throw()
{
	if( default_compare_trait<U>::IsEQ(u, (U)0) )
		return call_result(system_call_results::Invalid);
	r = (T)::fmodf((float)t, (float)u);
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_modulus_d(T t, U u, T& r) throw()
{
	if( default_compare_trait<U>::IsEQ(u, (U)0) )
		return call_result(system_call_results::Invalid);
	r = (T)::fmod((double)t, (double)u);
	return call_result();
}

#define _DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(type1, type2)  \
	template <> inline call_result numeric_modulus<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_modulus_uu_s4u2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(type1, type2)  \
	template <> inline call_result numeric_modulus<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_modulus_ss_u2s4<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MODULUS_S08U4_S4U04(type1, type2)  \
	template <> inline call_result numeric_modulus<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_modulus_s08u4_s4u04<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MODULUS_U4S08_U04S4(type1, type2)  \
	template <> inline call_result numeric_modulus<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_modulus_u4s08_u04s4<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MODULUS_U08S8(type1, type2)  \
	template <> inline call_result numeric_modulus<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_modulus_u08s8<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MODULUS_S8U08(type1, type2)  \
	template <> inline call_result numeric_modulus<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_modulus_s8u08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MODULUS_F(type1, type2)  \
	template <> inline call_result numeric_modulus<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_modulus_f<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MODULUS_D(type1, type2)  \
	template <> inline call_result numeric_modulus<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_modulus_d<type1, type2>(t, u, r); }

//char
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(char, char)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(byte, char)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(short, char)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(ushort, char)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(int, char)
_DECLARE_FUNC_NUMERIC_MODULUS_U4S08_U04S4(uint, char)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(int64, char)
_DECLARE_FUNC_NUMERIC_MODULUS_U08S8(uint64, char)
_DECLARE_FUNC_NUMERIC_MODULUS_F(float, char)
_DECLARE_FUNC_NUMERIC_MODULUS_D(double, char)
//byte
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(char, byte)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(byte, byte)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(short, byte)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(ushort, byte)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(int, byte)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(uint, byte)
_DECLARE_FUNC_NUMERIC_MODULUS_S08U4_S4U04(int64, byte)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(uint64, byte)
_DECLARE_FUNC_NUMERIC_MODULUS_F(float, byte)
_DECLARE_FUNC_NUMERIC_MODULUS_D(double, byte)
//short
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(char, short)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(byte, short)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(short, short)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(ushort, short)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(int, short)
_DECLARE_FUNC_NUMERIC_MODULUS_U4S08_U04S4(uint, short)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(int64, short)
_DECLARE_FUNC_NUMERIC_MODULUS_U08S8(uint64, short)
_DECLARE_FUNC_NUMERIC_MODULUS_F(float, short)
_DECLARE_FUNC_NUMERIC_MODULUS_D(double, short)
//ushort
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(char, ushort)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(byte, ushort)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(short, ushort)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(ushort, ushort)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(int, ushort)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(uint, ushort)
_DECLARE_FUNC_NUMERIC_MODULUS_S08U4_S4U04(int64, ushort)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(uint64, ushort)
_DECLARE_FUNC_NUMERIC_MODULUS_F(float, ushort)
_DECLARE_FUNC_NUMERIC_MODULUS_D(double, ushort)
//int
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(char, int)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(byte, int)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(short, int)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(ushort, int)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(int, int)
_DECLARE_FUNC_NUMERIC_MODULUS_U4S08_U04S4(uint, int)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(int64, int)
_DECLARE_FUNC_NUMERIC_MODULUS_U08S8(uint64, int)
_DECLARE_FUNC_NUMERIC_MODULUS_F(float, int)
_DECLARE_FUNC_NUMERIC_MODULUS_D(double, int)
//uint
_DECLARE_FUNC_NUMERIC_MODULUS_S08U4_S4U04(char, uint)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(byte, uint)
_DECLARE_FUNC_NUMERIC_MODULUS_S08U4_S4U04(short, uint)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(ushort, uint)
_DECLARE_FUNC_NUMERIC_MODULUS_S08U4_S4U04(int, uint)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(uint, uint)
_DECLARE_FUNC_NUMERIC_MODULUS_S08U4_S4U04(int64, uint)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(uint64, uint)
_DECLARE_FUNC_NUMERIC_MODULUS_F(float, uint)
_DECLARE_FUNC_NUMERIC_MODULUS_D(double, uint)
//int64
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(char, int64)
_DECLARE_FUNC_NUMERIC_MODULUS_U4S08_U04S4(byte, int64)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(short, int64)
_DECLARE_FUNC_NUMERIC_MODULUS_U4S08_U04S4(ushort, int64)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(int, int64)
_DECLARE_FUNC_NUMERIC_MODULUS_U4S08_U04S4(uint, int64)
_DECLARE_FUNC_NUMERIC_MODULUS_SS_U2S4(int64, int64)
_DECLARE_FUNC_NUMERIC_MODULUS_U08S8(uint64, int64)
_DECLARE_FUNC_NUMERIC_MODULUS_F(float, int64)
_DECLARE_FUNC_NUMERIC_MODULUS_D(double, int64)
//uint64
_DECLARE_FUNC_NUMERIC_MODULUS_S8U08(char, uint64)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(byte, uint64)
_DECLARE_FUNC_NUMERIC_MODULUS_S8U08(short, uint64)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(ushort, uint64)
_DECLARE_FUNC_NUMERIC_MODULUS_S8U08(int, uint64)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(uint, uint64)
_DECLARE_FUNC_NUMERIC_MODULUS_S8U08(int64, uint64)
_DECLARE_FUNC_NUMERIC_MODULUS_UU_S4U2(uint64, uint64)
_DECLARE_FUNC_NUMERIC_MODULUS_F(float, uint64)
_DECLARE_FUNC_NUMERIC_MODULUS_D(double, uint64)
//float
_DECLARE_FUNC_NUMERIC_MODULUS_F(char, float)
_DECLARE_FUNC_NUMERIC_MODULUS_F(byte, float)
_DECLARE_FUNC_NUMERIC_MODULUS_F(short, float)
_DECLARE_FUNC_NUMERIC_MODULUS_F(ushort, float)
_DECLARE_FUNC_NUMERIC_MODULUS_F(int, float)
_DECLARE_FUNC_NUMERIC_MODULUS_F(uint, float)
_DECLARE_FUNC_NUMERIC_MODULUS_F(int64, float)
_DECLARE_FUNC_NUMERIC_MODULUS_F(uint64, float)
_DECLARE_FUNC_NUMERIC_MODULUS_F(float, float)
_DECLARE_FUNC_NUMERIC_MODULUS_D(double, float)
//double
_DECLARE_FUNC_NUMERIC_MODULUS_D(char, double)
_DECLARE_FUNC_NUMERIC_MODULUS_D(byte, double)
_DECLARE_FUNC_NUMERIC_MODULUS_D(short, double)
_DECLARE_FUNC_NUMERIC_MODULUS_D(ushort, double)
_DECLARE_FUNC_NUMERIC_MODULUS_D(int, double)
_DECLARE_FUNC_NUMERIC_MODULUS_D(uint, double)
_DECLARE_FUNC_NUMERIC_MODULUS_D(int64, double)
_DECLARE_FUNC_NUMERIC_MODULUS_D(uint64, double)
_DECLARE_FUNC_NUMERIC_MODULUS_D(float, double)
_DECLARE_FUNC_NUMERIC_MODULUS_D(double, double)

// division

template <typename T, typename U>
inline call_result numeric_divide(T t, U u, T& r) throw();

template <typename T, typename U>
inline call_result _numeric_divide_uu_s4u2_s08u4(T t, U u, T& r) throw()
{
	if( u == 0 )
		return call_result(system_call_results::Invalid);
	r = (T)(t / u);
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_divide_us(T t, U u, T& r) throw()
{
	if( u > (U)0 ) {
		r = (T)(t / u);
		return call_result();
	}
	if( u == 0 )
		return call_result(system_call_results::Invalid);
	if( numeric_n2p(u) > t ) {
		r = 0;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_divide_s4u04(T t, U u, T& r) throw()
{
	if( u == 0 )
		return call_result(system_call_results::Invalid);
	if( t > 0 )
		r = (T)(t / u);
	else
		r = (T)((int64)t / (int64)u);
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_divide_s8u08(T t, U u, T& r) throw()
{
	if( u == 0 )
		return call_result(system_call_results::Invalid);
	if( u <= (uint64)(limits_base<T>::Max) ) {
		if( sizeof(T) < sizeof(int64) )
			r = (T)((int)t / (int)u);
		else
			r = (T)((int64)t / (int64)u);
	}
	else { //corner case
		if( t == limits_base<T>::Lowest && u == (uint64)(limits_base<T>::Max) + 1 )
			r = -1;
		else
			r = 0;
	}
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_divide_ss(T t, U u, T& r) throw()
{
	if( u == 0 )
		return call_result(system_call_results::Invalid);
	//corner case
	if( t == limits_base<T>::Lowest && u == (U)-1 )
		return call_result(system_call_results::Overflow);
	r = (T)(t / u);
	return call_result();
}

template <typename T, typename U>
inline call_result _numeric_divide_xf_xd(T t, U u, T& r) throw()
{
	if( default_compare_trait<U>::IsEQ(u, (U)0) )
		return call_result(system_call_results::Invalid);
	U tmp = (U)t / u;
	if( !float_helper::IsFinite(tmp) )
		return call_result(system_call_results::Overflow);
	return numeric_cast<U, T>(tmp, r);
}
template <typename T, typename U>
inline call_result _numeric_divide_fx_dx(T t, U u, T& r) throw()
{
	if( default_compare_trait<U>::IsEQ(u, (U)0) )
		return call_result(system_call_results::Invalid);
	r = (T)(t / (T)u);
	if( float_helper::IsFinite(r) )
		return call_result();
	return call_result(system_call_results::Overflow);
}

#define _DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(type1, type2)  \
	template <> inline call_result numeric_divide<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_divide_uu_s4u2_s08u4<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_DIVIDE_US(type1, type2)  \
	template <> inline call_result numeric_divide<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_divide_us<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_DIVIDE_S4U04(type1, type2)  \
	template <> inline call_result numeric_divide<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_divide_s4u04<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_DIVIDE_S8U08(type1, type2)  \
	template <> inline call_result numeric_divide<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_divide_s8u08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_DIVIDE_SS(type1, type2)  \
	template <> inline call_result numeric_divide<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_divide_ss<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(type1, type2)  \
	template <> inline call_result numeric_divide<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_divide_xf_xd<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(type1, type2)  \
	template <> inline call_result numeric_divide<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_divide_fx_dx<type1, type2>(t, u, r); }

//char
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(char, char)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(byte, char)
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(short, char)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(ushort, char)
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(int, char)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(uint, char)
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(int64, char)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(uint64, char)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(float, char)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(double, char)
//byte
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(char, byte)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(byte, byte)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(short, byte)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(ushort, byte)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(int, byte)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(uint, byte)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(int64, byte)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(uint64, byte)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(float, byte)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(double, byte)
//short
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(char, short)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(byte, short)
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(short, short)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(ushort, short)
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(int, short)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(uint, short)
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(int64, short)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(uint64, short)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(float, short)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(double, short)
//ushort
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(char, ushort)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(byte, ushort)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(short, ushort)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(ushort, ushort)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(int, ushort)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(uint, ushort)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(int64, ushort)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(uint64, ushort)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(float, ushort)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(double, ushort)
//int
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(char, int)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(byte, int)
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(short, int)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(ushort, int)
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(int, int)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(uint, int)
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(int64, int)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(uint64, int)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(float, int)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(double, int)
//uint
_DECLARE_FUNC_NUMERIC_DIVIDE_S4U04(char, uint)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(byte, uint)
_DECLARE_FUNC_NUMERIC_DIVIDE_S4U04(short, uint)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(ushort, uint)
_DECLARE_FUNC_NUMERIC_DIVIDE_S4U04(int, uint)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(uint, uint)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(int64, uint)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(uint64, uint)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(float, uint)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(double, uint)
//int64
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(char, int64)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(byte, int64)
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(short, int64)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(ushort, int64)
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(int, int64)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(uint, int64)
_DECLARE_FUNC_NUMERIC_DIVIDE_SS(int64, int64)
_DECLARE_FUNC_NUMERIC_DIVIDE_US(uint64, int64)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(float, int64)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(double, int64)
//uint64
_DECLARE_FUNC_NUMERIC_DIVIDE_S8U08(char, uint64)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(byte, uint64)
_DECLARE_FUNC_NUMERIC_DIVIDE_S8U08(short, uint64)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(ushort, uint64)
_DECLARE_FUNC_NUMERIC_DIVIDE_S8U08(int, uint64)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(uint, uint64)
_DECLARE_FUNC_NUMERIC_DIVIDE_S8U08(int64, uint64)
_DECLARE_FUNC_NUMERIC_DIVIDE_UU_S4U2_S08U4(uint64, uint64)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(float, uint64)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(double, uint64)
//float
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(char, float)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(byte, float)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(short, float)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(ushort, float)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(int, float)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(uint, float)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(int64, float)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(uint64, float)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(float, float)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(double, float)
//double
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(char, double)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(byte, double)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(short, double)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(ushort, double)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(int, double)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(uint, double)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(int64, double)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(uint64, double)
_DECLARE_FUNC_NUMERIC_DIVIDE_XF_XD(float, double)
_DECLARE_FUNC_NUMERIC_DIVIDE_FX_DX(double, double)

//large int region multiply
inline call_result _numeric_large_int_region_multiply(uint64 a, uint64 b, uint64& r) throw()
{
	// Consider that a*b can be broken up into:
	// (aHigh * 2^32 + aLow) * (bHigh * 2^32 + bLow)
	// => (aHigh * bHigh * 2^64) + (aLow * bHigh * 2^32) + (aHigh * bLow * 2^32) + (aLow * bLow)
	// Note - same approach applies for 128 bit math on a 64-bit system
	uint aHigh = (uint)(a >> 32);
	uint aLow  = (uint)a;
	uint bHigh = (uint)(b >> 32);
	uint bLow  = (uint)b;

	r = 0;

	if( aHigh == 0 ) {
		if( bHigh != 0 )
			r = (uint64)aLow * (uint64)bHigh;
	}
	else if( bHigh == 0 ) {
		if( aHigh != 0 )
			r = (uint64)aHigh * (uint64)bLow;
	}
	else {
		return call_result(system_call_results::Overflow);
	}

	if( r != 0 ) {
		if( (uint)(r >> 32) != 0 )
			return call_result(system_call_results::Overflow);

		r <<= 32;
		uint64 tmp = (uint64)aLow * (uint64)bLow;
		r += tmp;

		if( r < tmp )
			return call_result(system_call_results::Overflow);

		return call_result();
	}

	r = (uint64)aLow * (uint64)bLow;
	return call_result();
}
inline call_result _numeric_large_int_region_multiply(uint64 a, uint b, uint64& r) throw()
{
	// Consider that a*b can be broken up into:
	// (aHigh * 2^32 + aLow) * b
	// => (aHigh * b * 2^32) + (aLow * b)
	uint aHigh = (uint)(a >> 32);
	uint aLow  = (uint)a;

	r = 0;
	if( aHigh != 0 ) {
		r = (uint64)aHigh * (uint64)b;
		if( (uint)(r >> 32) != 0 )
			return call_result(system_call_results::Overflow);

		r <<= 32;
		uint64 tmp = (uint64)aLow * (uint64)b;
		r += tmp;

		if( r < tmp )
			return call_result(system_call_results::Overflow);

		return call_result();
	}

	r = (uint64)aLow * (uint64)b;
	return call_result();
}
inline call_result _numeric_large_int_region_multiply(uint a, uint64 b, uint& r) throw()
{
	// Consider that a*b can be broken up into:
	// (bHigh * 2^32 + bLow) * a
	// => (bHigh * a * 2^32) + (bLow * a)
	// In this case, the result must fit into 32-bits
	// If bHigh != 0 && a != 0, immediate error.
	if( (uint)(b >> 32) != 0 && a != 0 )
		return call_result(system_call_results::Overflow);
	uint64 tmp = b * (uint64)a;
	if( (uint)(tmp >> 32) != 0 ) // overflow
		return call_result(system_call_results::Overflow);
	r = (uint)tmp;
	return call_result();
}
inline call_result _numeric_large_int_region_multiply(uint64 a, int b, uint64& r) throw()
{
	if( b < 0 && a != 0 )
		return call_result(system_call_results::Overflow);
	return _numeric_large_int_region_multiply(a, (uint)b, r);
}
inline call_result _numeric_large_int_region_multiply(uint a, int64 b, uint& r) throw()
{
	if( b < 0 && a != 0 )
		return call_result(system_call_results::Overflow);
	return _numeric_large_int_region_multiply(a, (uint64)b, r);
}
inline call_result _numeric_large_int_region_multiply(uint64 a, int64 b, uint64& r) throw()
{
	if( b < 0 && a != 0 )
		return call_result(system_call_results::Overflow);
	return _numeric_large_int_region_multiply(a, (uint64)b, r);
}
inline call_result _numeric_large_int_region_multiply(int64 a, int64 b, int64& r) throw()
{
	bool aNegative = false;
	bool bNegative = false;

	int64 a1 = a;
	int64 b1 = b;
	if( a1 < 0 ) {
		aNegative = true;
		a1 = -a1;
	}
	if( b1 < 0 ) {
		bNegative = true;
		b1 = -b1;
	}

	uint64 tmp;
	if( _numeric_large_int_region_multiply((uint64)a1, (uint64)b1, tmp).IsSucceeded() ) {
		if( aNegative ^ bNegative ) {
			// Result must be negative
			if( tmp <= (uint64)(limits_base<int64>::Lowest) ) {
				r = -(int64)tmp;
				return call_result();
			}
		}
		else {
			// Result must be positive
			if( tmp <= (uint64)(limits_base<int64>::Max) ) {
				r = (int64)tmp;
				return call_result();
			}
		}
	}

	return call_result(system_call_results::Overflow);
}
inline call_result _numeric_large_int_region_multiply(int64 a, int b, int64& r) throw()
{
	bool aNegative = false;
	bool bNegative = false;

	int64 a1 = a;
	int64 b1 = b;
	if( a1 < 0 ) {
		aNegative = true;
		a1 = -a1;
	}
	if( b1 < 0 ) {
		bNegative = true;
		b1 = -b1;
	}

	uint64 tmp;
	if( _numeric_large_int_region_multiply((uint64)a1, (uint)b1, tmp).IsSucceeded() ) {
		if( aNegative ^ bNegative ) {
			// Result must be negative
			if( tmp <= (uint64)(limits_base<int64>::Lowest) ) {
				r = -(int64)tmp;
				return call_result();
			}
		}
		else {
			// Result must be positive
			if( tmp <= (uint64)(limits_base<int64>::Max) ) {
				r = (int64)tmp;
				return call_result();
			}
		}
	}

	return call_result(system_call_results::Overflow);
}
inline call_result _numeric_large_int_region_multiply(int a, int64 b, int& r) throw()
{
	bool aNegative = false;
	bool bNegative = false;

	int64 b1 = b;
	if( a < 0 ) {
		aNegative = true;
		a = -a;
	}
	if( b1 < 0 ) {
		bNegative = true;
		b1 = -b1;
	}

	uint tmp;
	if( _numeric_large_int_region_multiply((uint)a, (uint64)b1, tmp).IsSucceeded() ) {
		if( aNegative ^ bNegative ) {
			// Result must be negative
			if( tmp <= (uint)(limits_base<int>::Lowest) ) {
				r = -tmp;
				return call_result();
			}
		}
		else {
			// Result must be positive
			if( tmp <= (uint)(limits_base<int>::Max) ) {
				r = (int)tmp;
				return call_result();
			}
		}
	}

	return call_result(system_call_results::Overflow);
}
inline call_result _numeric_large_int_region_multiply(int64 a, uint b, int64& r) throw()
{
	bool aNegative = false;

	int64 a1 = a;
	if( a1 < 0 ) {
		aNegative = true;
		a1 = -a1;
	}

	uint64 tmp;
	if( _numeric_large_int_region_multiply((uint64)a1, b, tmp).IsSucceeded() ) {
		if( aNegative ) {
			// Result must be negative
			if( tmp <= (uint64)(limits_base<int64>::Lowest) ) {
				r = -(int64)tmp;
				return call_result();
			}
		}
		else {
			// Result must be positive
			if( tmp <= (uint64)(limits_base<int64>::Max) ) {
				r = (int64)tmp;
				return call_result();
			}
		}
	}

	return call_result(system_call_results::Overflow);
}
inline call_result _numeric_large_int_region_multiply(int a, uint64 b, int& r) throw()
{
	// Consider that a*b can be broken up into:
	// a * (bHigh * 2^32 + bLow)
	// => (a * bHigh * 2^32) + (a * bLow)
	uint bHigh = (uint)(b >> 32);
	uint bLow  = (uint)b;

	if( bHigh != 0 && a != 0 )
		return call_result(system_call_results::Overflow);

	bool aNegative = false;
	if( a < 0 ) {
		aNegative = true;
		a = -a;
	}

	uint64 tmp = (uint)a * (uint64)bLow;
	if( !aNegative ) {
		// Result must be positive
		if( tmp <= (uint64)(limits_base<int>::Max) ) {
			r = (int)tmp;
			return call_result();
		}
	}
	else {
		// Result must be negative
		if( tmp <= (uint64)(uint)(limits_base<int>::Lowest) ) {
			r = -(int)tmp;
			return call_result();
		}
	}

	return call_result(system_call_results::Overflow);
}
inline call_result _numeric_large_int_region_multiply(int64 a, uint64 b, int64& r) throw()
{
	bool aNegative = false;

	int64 a1 = a;
	if( a1 < 0 ) {
		aNegative = true;
		a1 = -a1;
	}

	uint64 tmp;
	if( _numeric_large_int_region_multiply((uint64)a1, b, tmp).IsSucceeded() ) {
		if( aNegative ) {
			// Result must be negative
			if( tmp <= (uint64)(limits_base<int64>::Lowest) ) {
				r = -(int64)tmp;
				return call_result();
			}
		}
		else {
			// Result must be positive
			if( tmp <= (uint64)(limits_base<int64>::Max) ) {
				r = (int64)tmp;
				return call_result();
			}
		}
	}

	return call_result(system_call_results::Overflow);
}

// multiplication

template <typename T, typename U>
inline call_result numeric_multiply(T t, U u, T& r) throw();

template <typename T, typename U>
inline call_result _numeric_multiply_u2u2(T t, U u, T& r) throw()
{
	uint tmp = t * u;
	if( tmp > (uint)(limits_base<T>::Max) )
		return call_result(system_call_results::Overflow);
	r = (T)tmp;
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_multiply_u04u4_u2u04(T t, U u, T& r) throw()
{
	uint64 tmp = (uint64)t * (uint64)u;
	if( tmp > (uint64)(limits_base<T>::Max) )
		return call_result(system_call_results::Overflow);
	r = (T)tmp;
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_multiply_u08u08(T t, U u, T& r) throw()
{
	return _numeric_large_int_region_multiply(t, u, r);
}
template <typename T, typename U>
inline call_result _numeric_multiply_u08u4(T t, U u, T& r) throw()
{
	return _numeric_large_int_region_multiply(t, (uint)u, r);
}
template <typename T, typename U>
inline call_result _numeric_multiply_u4u08(T t, U u, T& r) throw()
{
	uint tmp;
	if(_numeric_large_int_region_multiply((uint)t, u, tmp).IsSucceeded()
		&& numeric_cast<uint, T>(tmp, r).IsSucceeded() )
		return call_result();
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_multiply_u2s2_s2s2_s2u2(T t, U u, T& r) throw()
{
	int tmp = t * u;
	if( tmp > limits_base<T>::Max || tmp < limits_base<T>::Lowest )
		return call_result(system_call_results::Overflow);
	r = (T)tmp;
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_multiply_u04s4_u2s04_s04s4_s2s04_s04u2_s4u04(T t, U u, T& r) throw()
{
	int64 tmp = (int64)t * (int64)u;
	if( tmp > (int64)(limits_base<T>::Max) || tmp < (int64)(limits_base<T>::Lowest) )
		return call_result(system_call_results::Overflow);
	r = (T)tmp;
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_multiply_u08s4(T t, U u, T& r) throw()
{
	return _numeric_large_int_region_multiply(t, (int)u, r);
}
template <typename T, typename U>
inline call_result _numeric_multiply_u4s08(T t, U u, T& r) throw()
{
	uint tmp;
	if( _numeric_large_int_region_multiply((uint)t, u, tmp).IsSucceeded()
		&& numeric_cast<uint, T>(tmp, r).IsSucceeded() )
		return call_result();
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_multiply_u08s08(T t, U u, T& r) throw()
{
	return _numeric_large_int_region_multiply(t, u, r);
}
template <typename T, typename U>
inline call_result _numeric_multiply_s08s08(T t, U u, T& r) throw()
{
	return _numeric_large_int_region_multiply(t, u, r);
}
template <typename T, typename U>
inline call_result _numeric_multiply_s08s4(T t, U u, T& r) throw()
{
	return _numeric_large_int_region_multiply(t, (int)u, r);
}
template <typename T, typename U>
inline call_result _numeric_multiply_s4s08(T t, U u, T& r) throw()
{
	int tmp;
	if( _numeric_large_int_region_multiply((int)t, u, tmp).IsSucceeded()
		&& numeric_cast<int, T>(tmp, r).IsSucceeded() )
		return call_result();
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_multiply_s08u4(T t, U u, T& r) throw()
{
	return _numeric_large_int_region_multiply(t, (uint)u, r);
}
template <typename T, typename U>
inline call_result _numeric_multiply_s4u08(T t, U u, T& r) throw()
{
	int tmp;
	if( _numeric_large_int_region_multiply((int)t, u, tmp).IsSucceeded()
		&& numeric_cast<int, T>(tmp, r).IsSucceeded() )
		return call_result();
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_multiply_s08u08(T t, U u, T& r) throw()
{
	return _numeric_large_int_region_multiply(t, u, r);
}

template <typename T, typename U>
inline call_result _numeric_multiply_xf_xd(T t, U u, T& r) throw()
{
	U tmp = (U)t * u;
	if( !float_helper::IsFinite(tmp) )
		return call_result(system_call_results::Overflow);
	return numeric_cast<U, T>(tmp, r);
}
template <typename T, typename U>
inline call_result _numeric_multiply_fx_dx(T t, U u, T& r) throw()
{
	r = (T)(t * (T)u);
	if( float_helper::IsFinite(r) )
		return call_result();
	return call_result(system_call_results::Overflow);
}

#define _DECLARE_FUNC_NUMERIC_MULTIPLY_U2U2(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_u2u2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_U04U4_U2U04(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_u04u4_u2u04<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_U08U08(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_u08u08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_U08U4(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_u08u4<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_U4U08(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_u4u08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_U2S2_S2S2_S2U2(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_u2s2_s2s2_s2u2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_u04s4_u2s04_s04s4_s2s04_s04u2_s4u04<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_U08S4(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_u08s4<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_U4S08(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_u4s08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_U08S08(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_u08s08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_S08S08(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_s08s08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_S08S4(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_s08s4<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_S4S08(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_s4s08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_S08U4(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_s08u4<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_S4U08(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_s4u08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_S08U08(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_s08u08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_xf_xd<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(type1, type2)  \
	template <> inline call_result numeric_multiply<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_multiply_fx_dx<type1, type2>(t, u, r); }

//char
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2S2_S2S2_S2U2(char, char)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2S2_S2S2_S2U2(byte, char)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2S2_S2S2_S2U2(short, char)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2S2_S2S2_S2U2(ushort, char)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(int, char)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(uint, char)
_DECLARE_FUNC_NUMERIC_MULTIPLY_S08S4(int64, char)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U08S4(uint64, char)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(float, char)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(double, char)
//byte
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2S2_S2S2_S2U2(char, byte)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2U2(byte, byte)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2S2_S2S2_S2U2(short, byte)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2U2(ushort, byte)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(int, byte)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04U4_U2U04(uint, byte)
_DECLARE_FUNC_NUMERIC_MULTIPLY_S08U4(int64, byte)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U08U4(uint64, byte)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(float, byte)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(double, byte)
//short
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2S2_S2S2_S2U2(char, short)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2S2_S2S2_S2U2(byte, short)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2S2_S2S2_S2U2(short, short)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2S2_S2S2_S2U2(ushort, short)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(int, short)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(uint, short)
_DECLARE_FUNC_NUMERIC_MULTIPLY_S08S4(int64, short)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U08S4(uint64, short)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(float, short)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(double, short)
//ushort
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2S2_S2S2_S2U2(char, ushort)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2U2(byte, ushort)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2S2_S2S2_S2U2(short, ushort)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U2U2(ushort, ushort)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(int, ushort)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04U4_U2U04(uint, ushort)
_DECLARE_FUNC_NUMERIC_MULTIPLY_S08U4(int64, ushort)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U08U4(uint64, ushort)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(float, ushort)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(double, ushort)
//int
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(char, int)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(byte, int)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(short, int)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(ushort, int)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(int, int)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(uint, int)
_DECLARE_FUNC_NUMERIC_MULTIPLY_S08S4(int64, int)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U08S4(uint64, int)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(float, int)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(double, int)
//uint
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(char, uint)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04U4_U2U04(byte, uint)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(short, uint)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04U4_U2U04(ushort, uint)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04(int, uint)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U04U4_U2U04(uint, uint)
_DECLARE_FUNC_NUMERIC_MULTIPLY_S08U4(int64, uint)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U08U4(uint64, uint)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(float, uint)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(double, uint)
//int64
_DECLARE_FUNC_NUMERIC_MULTIPLY_S4S08(char, int64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U4S08(byte, int64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_S4S08(short, int64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U4S08(ushort, int64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_S4S08(int, int64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U4S08(uint, int64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_S08S08(int64, int64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U08S08(uint64, int64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(float, int64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(double, int64)
//uint64
_DECLARE_FUNC_NUMERIC_MULTIPLY_S4U08(char, uint64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U4U08(byte, uint64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_S4U08(short, uint64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U4U08(ushort, uint64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_S4U08(int, uint64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U4U08(uint, uint64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_S08U08(int64, uint64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_U08U08(uint64, uint64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(float, uint64)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(double, uint64)
//float
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(char, float)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(byte, float)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(short, float)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(ushort, float)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(int, float)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(uint, float)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(int64, float)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(uint64, float)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(float, float)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(double, float)
//double
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(char, double)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(byte, double)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(short, double)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(ushort, double)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(int, double)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(uint, double)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(int64, double)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(uint64, double)
_DECLARE_FUNC_NUMERIC_MULTIPLY_XF_XD(float, double)
_DECLARE_FUNC_NUMERIC_MULTIPLY_FX_DX(double, double)

// subtraction

template <typename T, typename U>
inline call_result numeric_subtract(T t, U u, T& r) throw();

template <typename T, typename U>
inline call_result _numeric_subtract_uu(T t, U u, T& r) throw()
{
	if( u <= t ) {
		r = (T)(t - u);
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_u2s2_s2s2(T t, U u, T& r) throw()
{
	int tmp = t - u;
	if( numeric_cast<int, T>(tmp, r).IsSucceeded() ) {
		r = (T)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_s2u2(T t, U u, T& r) throw()
{
	int tmp = t - u;
	if( tmp >= (int)(limits_base<T>::Lowest) ) {
		r = (T)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_u04s4_u2s04_s04s4_s2s04(T t, U u, T& r) throw()
{
	int64 tmp = (int64)t - (int64)u;
	return numeric_cast<int64, T>(tmp, r);
}
template <typename T, typename U>
inline call_result _numeric_subtract_s04u2_s4u04(T t, U u, T& r) throw()
{
	int64 tmp = (int64)t - (int64)u;
	if( tmp >= (int64)(limits_base<T>::Lowest) ) {
		r = (T)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_u08s8(T t, U u, T& r) throw()
{
	if( u >= 0 ) {
		if( (uint64)u <= t ) {
			r = (T)(t - (uint64)u);
			return call_result();
		}
	}
	else {
		// we're now effectively adding
		T tmp = t + (T)numeric_n2p(u);
		if( tmp >= t ) {
			r = tmp;
			return call_result();
		}
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_u4s08(T t, U u, T& r) throw()
{
	if( u >= 0 ) {
		if( (uint64)u <= t ) {
			r = (T)(t - (T)u);
			return call_result();
		}
	}
	else {
		// we're now effectively adding
		// since t is 32-bit, and u cannot exceed 2^63
		// this addition cannot overflow
		uint64 tmp = t + (uint64)(-u); // negation safe
		if( tmp <= (uint64)(limits_base<T>::Max) ) {
			r = (T)tmp;
			return call_result();
		}
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_s08s8(T t, U u, T& r) throw()
{
	int64 tmp = t - u;
	if( (t >= 0 && u < 0 && tmp < t)
		|| (u >= 0 && tmp > t) )
		return call_result(system_call_results::Overflow);
	r = (T)tmp;
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_subtract_s4s08(T t, U u, T& r) throw()
{
	int64 tmp = (int64)t - u;
	if( t >= 0 ) {
		if( u >= 0 ) {
			if( tmp >= (int64)(limits_base<T>::Lowest) ) {
				r = (T)tmp;
				return call_result();
			}
		}
		else {
			if( tmp >= t && tmp <= (int64)(limits_base<T>::Max) ) {
				r = (T)tmp;
				return call_result();
			}
		}
	}
	else {
		if( u >= 0 ) {
			if( tmp <= t && tmp >= (int64)(limits_base<T>::Lowest) ) {
				r = (T)tmp;
				return call_result();
			}
		}
		else {
			if( tmp <= (int64)(limits_base<T>::Max) ) {
				r = (T)tmp;
				return call_result();
			}
		}
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_s08u4(T t, U u, T& r) throw()
{
	int64 tmp = t - (int64)u;
	if( tmp <= t ) {
		r = (T)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_s4u08(T t, U u, T& r) throw()
{
	const uint64 c_AbsMinIntT = (uint64)(limits_base<T>::Max) + 1;
	if( t < 0 ) {
		if( u <= c_AbsMinIntT - (uint64)numeric_n2p(t) ) {
			r = (T)(t - u);
			return call_result();
		}
	}
	else {
		if( u <= c_AbsMinIntT + (uint64)t ) {
			r = (T)(t - u);
			return call_result();
		}
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_s08u08(T t, U u, T& r) throw()
{
	int64 tmp = t - (int64)u;
	if( tmp <= t ) {
		r = tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}

template <typename T, typename U>
inline call_result _numeric_subtract_xf_xd(T t, U u, T& r) throw()
{
	U tmp = (U)t - u;
	if( !float_helper::IsFinite(tmp) )
		return call_result(system_call_results::Overflow);
	return numeric_cast<U, T>(tmp, r);
}
template <typename T, typename U>
inline call_result _numeric_subtract_fx_dx(T t, U u, T& r) throw()
{
	r = (T)(t - (T)u);
	if( float_helper::IsFinite(r) )
		return call_result();
	return call_result(system_call_results::Overflow);
}

#define _DECLARE_FUNC_NUMERIC_SUBTRACT_UU(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_subtract_uu<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_subtract_u2s2_s2s2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_S2U2(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_subtract_s2u2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_subtract_u04s4_u2s04_s04s4_s2s04<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_S04U2_S4U04(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_subtract_s04u2_s4u04<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_U08S8(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_subtract_u08s8<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_U4S08(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_subtract_u4s08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_S08S8(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_subtract_s08s8<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_S4S08(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_subtract_s4s08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_S08U4(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_subtract_s08u4<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_S4U08(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_subtract_s4u08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_S08U08(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_subtract_s08u08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_subtract_xf_xd<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_subtract_fx_dx<type1, type2>(t, u, r); }

//char
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2(char, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2(byte, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2(short, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2(ushort, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04(int, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04(uint, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08S8(int64, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U08S8(uint64, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(float, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(double, char)
//byte
_DECLARE_FUNC_NUMERIC_SUBTRACT_S2U2(char, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(byte, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S2U2(short, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(ushort, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S04U2_S4U04(int, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(uint, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08U4(int64, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(uint64, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(float, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(double, byte)
//short
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2(char, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2(byte, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2(short, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2(ushort, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04(int, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04(uint, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08S8(int64, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U08S8(uint64, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(float, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(double, short)
//ushort
_DECLARE_FUNC_NUMERIC_SUBTRACT_S2U2(char, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(byte, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S2U2(short, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(ushort, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S04U2_S4U04(int, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(uint, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08U4(int64, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(uint64, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(float, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(double, ushort)
//int
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04(char, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04(byte, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04(short, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04(ushort, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04(int, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04(uint, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08S8(int64, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U08S8(uint64, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(float, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(double, int)
//uint
_DECLARE_FUNC_NUMERIC_SUBTRACT_S04U2_S4U04(char, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(byte, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S04U2_S4U04(short, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(ushort, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S04U2_S4U04(int, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(uint, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08U4(int64, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(uint64, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(float, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(double, uint)
//int64
_DECLARE_FUNC_NUMERIC_SUBTRACT_S4S08(char, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U4S08(byte, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S4S08(short, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U4S08(ushort, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S4S08(int, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U4S08(uint, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08S8(int64, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U08S8(uint64, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(float, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(double, int64)
//uint64
_DECLARE_FUNC_NUMERIC_SUBTRACT_S4U08(char, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(byte, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S4U08(short, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(ushort, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S4U08(int, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(uint, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08U08(int64, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU(uint64, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(float, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(double, uint64)
//float
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(char, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(byte, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(short, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(ushort, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(int, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(uint, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(int64, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(uint64, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(float, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(double, float)
//double
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(char, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(byte, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(short, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(ushort, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(int, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(uint, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(int64, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(uint64, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD(float, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX(double, double)

// version 2

template <typename T, typename U>
inline call_result numeric_subtract(T t, U u, U& r) throw();

template <typename T, typename U>
inline call_result _numeric_subtract_uu_2(T t, U u, U& r) throw()
{
	if( u <= t ) {
		T tmp = (T)(t - u);
		return numeric_cast<T, U>(tmp, r);
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_u2s2_s2s2_s2u2_2(T t, U u, U& r) throw()
{
	int tmp = t - u;
	return numeric_cast<int, U>(tmp, r);
}
template <typename T, typename U>
inline call_result _numeric_subtract_u04s4_u2s04_s04s4_s2s04_s04u2_s4u04_2(T t, U u, U& r) throw()
{
	int64 tmp = (int64)t - (int64)u;
	return numeric_cast<int64, U>(tmp, r);
}
template <typename T, typename U>
inline call_result _numeric_subtract_u08s8_2(T t, U u, U& r) throw()
{
	if( u < 0 ) {
		uint64 tmp;
		tmp = t + (uint64)numeric_n2p(u);
		if( tmp >= t && tmp <= (uint64)(limits_base<U>::Max) ) {
			r = (U)tmp;
			return call_result();
		}
	}
	else if( (uint64)u > t ) {
		r = (U)t - u;
		return call_result();
	}
	else {
		uint64 tmp = (uint64)t - (uint64)u;
		if( tmp <= (uint64)(limits_base<U>::Max) ) {
			r = (U)tmp;
			return call_result();
		}
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_u4s08_2(T t, U u, U& r) throw()
{
	if( u >= 0 ) {
		// overflow not possible
		r = (U)((int64)t - u);
		return call_result();
	}
	else {
		// we effectively have an addition which cannot overflow internally
		uint64 tmp = (uint64)t + (uint64)(-u);
		if( tmp <= (uint64)(limits_base<U>::Max) ) {
			r = (U)tmp;
			return call_result();
		}
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_s08s8_2(T t, U u, U& r) throw()
{
	int64 tmp = t - u;
	if( t >= 0 ) {
		if( sizeof(U) < 8 && tmp > limits_base<U>::Max )
			|| (u < 0 && tmp < t) )
			return call_result(system_call_results::Overflow);
	}
	else {
		if( sizeof(U) < 8 && tmp < (int64)(limits_base<U>::Lowest) )
			|| (u >= 0 && tmp > t) )
			return call_result(system_call_results::Overflow);
	}
	r = (U)tmp;
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_subtract_s4s08_2(T t, U u, U& r) throw()
{
	int64 tmp = (int64)t - u;
	if( (t >= 0 && u < 0 && tmp < t )
		 || (u >= 0 && tmp > t ) )
		return call_result(system_call_results::Overflow);
	r = (U)tmp;
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_subtract_s08u4_2(T t, U u, U& r) throw()
{
	int64 tmp = t - (int64)u;
	if( tmp <= limits_base<U>::Max && tmp >= (int64)(limits_base<U>::Min) ) {
		r = (U)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_s4u08_2(T t, U u, U& r) throw()
{
	if( t >= 0 && (U)t >= u ) {
		r = (U)((T)t - (T)u);
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_s08u08_2(T t, U u, U& r) throw()
{
	if( t >= 0 && (uint64)t >= u ) {
		r = (uint64)t - u;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}

template <typename T, typename U>
inline call_result _numeric_subtract_xf_xd_2(T t, U u, U& r) throw()
{
	r = (U)t - u;
	if( float_helper::IsFinite(r) )
		return call_result();
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_subtract_fx_dx_2(T t, U u, U& r) throw()
{
	T tmp = t - (T)u;
	if( !float_helper::IsFinite(tmp) )
		return call_result(system_call_results::Overflow);
	return numeric_cast<T, U>(tmp, r);
}

#define _DECLARE_FUNC_NUMERIC_SUBTRACT_UU_2(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type2& r) throw()  \
	{ return _numeric_subtract_uu_2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2_S2U2_2(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type2& r) throw()  \
	{ return _numeric_subtract_u2s2_s2s2_s2u2_2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type2& r) throw()  \
	{ return _numeric_subtract_u04s4_u2s04_s04s4_s2s04_s04u2_s4u04_2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_U08S8_2(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type2& r) throw()  \
	{ return _numeric_subtract_u08s8_2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_U4S08_2(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type2& r) throw()  \
	{ return _numeric_subtract_u4s08_2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_S08S8_2(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type2& r) throw()  \
	{ return _numeric_subtract_s08s8_2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_S4S08_2(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type2& r) throw()  \
	{ return _numeric_subtract_s4s08_2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_S08U4_2(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type2& r) throw()  \
	{ return _numeric_subtract_s08u4_2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_S4U08_2(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type2& r) throw()  \
	{ return _numeric_subtract_s4u08_2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_S08U08_2(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type2& r) throw()  \
	{ return _numeric_subtract_s08u08_2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type2& r) throw()  \
	{ return _numeric_subtract_xf_xd_2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(type1, type2)  \
	template <> inline call_result numeric_subtract<type1, type2>(type1 t, type2 u, type2& r) throw()  \
	{ return _numeric_subtract_fx_dx_2<type1, type2>(t, u, r); }

//char
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2_S2U2_2(byte, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2_S2U2_2(short, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2_S2U2_2(ushort, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(int, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(uint, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08S8_2(int64, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U08S8_2(uint64, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(float, char)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(double, char)
//byte
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2_S2U2_2(char, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2_S2U2_2(short, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU_2(ushort, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(int, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU_2(uint, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08U4_2(int64, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU_2(uint64, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(float, byte)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(double, byte)
//short
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2_S2U2_2(char, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2_S2U2_2(byte, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2_S2U2_2(ushort, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(int, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(uint, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08S8_2(int64, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U08S8_2(uint64, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(float, short)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(double, short)
//ushort
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2_S2U2_2(char, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU_2(byte, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U2S2_S2S2_S2U2_2(short, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(int, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU_2(uint, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08U4_2(int64, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU_2(uint64, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(float, ushort)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(double, ushort)
//int
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(char, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(byte, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(short, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(ushort, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(uint, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08S8_2(int64, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U08S8_2(uint64, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(float, int)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(double, int)
//uint
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(char, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU_2(byte, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(short, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU_2(ushort, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U04S4_U2S04_S04S4_S2S04_S04U2_S4U04_2(int, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08U4_2(int64, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU_2(uint64, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(float, uint)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(double, uint)
//int64
_DECLARE_FUNC_NUMERIC_SUBTRACT_S4S08_2(char, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U4S08_2(byte, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S4S08_2(short, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U4S08_2(ushort, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S4S08_2(int, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U4S08_2(uint, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_U08S8_2(uint64, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(float, int64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(double, int64)
//uint64
_DECLARE_FUNC_NUMERIC_SUBTRACT_S4U08_2(char, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU_2(byte, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S4U08_2(short, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU_2(ushort, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S4U08_2(int, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_UU_2(uint, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_S08U08_2(int64, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(float, uint64)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(double, uint64)
//float
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(char, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(byte, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(short, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(ushort, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(int, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(uint, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(int64, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(uint64, float)
_DECLARE_FUNC_NUMERIC_SUBTRACT_FX_DX_2(double, float)
//double
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(char, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(byte, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(short, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(ushort, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(int, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(uint, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(int64, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(uint64, double)
_DECLARE_FUNC_NUMERIC_SUBTRACT_XF_XD_2(float, double)

// addition

template <typename T, typename U>
inline call_result numeric_add(T t, U u, T& r) throw();

template <typename T, typename U>
inline call_result _numeric_add_u2u2_s2u2(T t, U u, T& r) throw()
{
	int tmp = t + u;
	if( tmp <= (int)(limits_base<T>::Max) ) {
		r = (T)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_add_u04u4(T t, U u, T& r) throw()
{
	uint tmp = (uint)t + (uint)u;
	if( tmp >= t ) {
		r = (T)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_add_u2u04(T t, U u, T& r) throw()
{
	uint tmp = (uint)t + (uint)u;
	if( tmp >= t && tmp <= limits_base<T>::Max ) {
		r = (T)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_add_u08u8(T t, U u, T& r) throw()
{
	uint64 tmp = (uint64)t + (uint64)u;
	if( tmp >= t ) {
		r = (T)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_add_u4u08(T t, U u, T& r) throw()
{
	uint64 tmp = (uint64)t + (uint64)u;
	if( tmp >= t && tmp <= limits_base<T>::Max ) {
		r = (T)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_add_u2s2_s2s2(T t, U u, T& r) throw()
{
	int tmp = t + u;
	if( tmp <= (int)(limits_base<T>::Max) && tmp >= (int)(limits_base<T>::Lowest) ) {
		r = (T)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_add_u04s4_u2s04_s04s4_s2s04(T t, U u, T& r) throw()
{
	int64 tmp = (int64)t + (int64)u;
	if( tmp <= (int64)(limits_base<T>::Max) && tmp >= (int64)(limits_base<T>::Lowest) ) {
		r = (T)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_add_s04u2_s4u04(T t, U u, T& r) throw()
{
	int64 tmp = (int64)t + (int64)u;
	if( tmp <= limits_base<T>::Max ) {
		r = (T)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_add_u08s8(T t, U u, T& r) throw()
{
	uint64 tmp;
	if( u < 0 ) {
		// So we're effectively subtracting
		tmp = numeric_n2p(u);
		if( tmp <= t ) {
			r = t - tmp;
			return call_result();
		}
	}
	else {
		tmp = (uint64)t + (uint64)u;
		// We added and it did not become smaller
		if( tmp >= t ) {
			r = (T)tmp;
			return call_result();
		}
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_add_u4s08(T t, U u, T& r) throw()
{
	if( u < 0 ) {
		if( t >= (uint64)(-u) ) {
			r = (T)(t + u);
			return call_result();
		}
	}
	else {
		uint64 tmp = (uint64)t + (uint64)u;
		if( tmp <= limits_base<T>::Max ) {
			r = (T)tmp;
			return call_result();
		}
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_add_s08s8(T t, U u, T& r) throw()
{
	int64 tmp = (int64)t + (int64)u;
	if( t >= 0 ) {
		if( u >= 0 && tmp < t )
			return call_result(system_call_results::Overflow);
	}
	else {
		if( u < 0 && tmp > t )
			return call_result(system_call_results::Overflow);
	}
	r = (T)tmp;
	return call_result();
}
template <typename T, typename U>
inline call_result _numeric_add_s4s08(T t, U u, T& r) throw()
{
	int64 tmp;
	if( _numeric_add_s08s8((int64)t, (int64)u, tmp).IsSucceeded()
		&& tmp <= limits_base<T>::Max && tmp >= limits_base<T>::Lowest ) {
		r = (T)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_add_s08u4(T t, U u, T& r) throw()
{
	int64 tmp = t + (int64)u;
	if( tmp >= t ) {
		r = (T)tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_add_s08u08(T t, U u, T& r) throw()
{
	int64 tmp = t + (int64)u;
	if( tmp >= t ) {
		r = tmp;
		return call_result();
	}
	return call_result(system_call_results::Overflow);
}
template <typename T, typename U>
inline call_result _numeric_add_s4u08(T t, U u, T& r) throw()
{
	if( (uint)(u >> 32) == 0 ) {
		int tmp = (int)((uint)u + (uint)(int)t);
		if( tmp >= t && numeric_cast<int, T>(tmp, r).IsSucceeded() )
			return call_result();
	}
	return call_result(system_call_results::Overflow);
}

template <typename T, typename U>
inline call_result _numeric_add_xf_xd(T t, U u, T& r) throw()
{
	U tmp = (U)t + u;
	if( !float_helper::IsFinite(tmp) )
		return call_result(system_call_results::Overflow);
	return numeric_cast<U, T>(tmp, r);
}
template <typename T, typename U>
inline call_result _numeric_add_fx_dx(T t, U u, T& r) throw()
{
	r = (T)(t + (T)u);
	if( float_helper::IsFinite(r) )
		return call_result();
	return call_result(system_call_results::Overflow);
}

#define _DECLARE_FUNC_NUMERIC_ADD_U2U2_S2U2(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_u2u2_s2u2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_U04U4(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_u04u4<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_U2U04(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_u2u04<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_U08U8(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_u08u8<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_U4U08(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_u4u08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_U2S2_S2S2(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_u2s2_s2s2<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_U04S4_U2S04_S04S4_S2S04(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_u04s4_u2s04_s04s4_s2s04<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_S04U2_S4U04(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_s04u2_s4u04<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_U08S8(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_u08s8<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_U4S08(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_u4s08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_S08S8(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_s08s8<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_S4S08(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_s4s08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_S08U4(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_s08u4<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_S08U08(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_s08u08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_S4U08(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_s4u08<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_XF_XD(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_xf_xd<type1, type2>(t, u, r); }
#define _DECLARE_FUNC_NUMERIC_ADD_FX_DX(type1, type2)  \
	template <> inline call_result numeric_add<type1, type2>(type1 t, type2 u, type1& r) throw()  \
	{ return _numeric_add_fx_dx<type1, type2>(t, u, r); }

//char
_DECLARE_FUNC_NUMERIC_ADD_U2S2_S2S2(char, char)
_DECLARE_FUNC_NUMERIC_ADD_U2S2_S2S2(byte, char)
_DECLARE_FUNC_NUMERIC_ADD_U2S2_S2S2(short, char)
_DECLARE_FUNC_NUMERIC_ADD_U2S2_S2S2(ushort, char)
_DECLARE_FUNC_NUMERIC_ADD_U04S4_U2S04_S04S4_S2S04(int, char)
_DECLARE_FUNC_NUMERIC_ADD_U04S4_U2S04_S04S4_S2S04(uint, char)
_DECLARE_FUNC_NUMERIC_ADD_S08S8(int64, char)
_DECLARE_FUNC_NUMERIC_ADD_U08S8(uint64, char)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(float, char)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(double, char)
//byte
_DECLARE_FUNC_NUMERIC_ADD_U2U2_S2U2(char, byte)
_DECLARE_FUNC_NUMERIC_ADD_U2U2_S2U2(byte, byte)
_DECLARE_FUNC_NUMERIC_ADD_U2U2_S2U2(short, byte)
_DECLARE_FUNC_NUMERIC_ADD_U2U2_S2U2(ushort, byte)
_DECLARE_FUNC_NUMERIC_ADD_S04U2_S4U04(int, byte)
_DECLARE_FUNC_NUMERIC_ADD_U04U4(uint, byte)
_DECLARE_FUNC_NUMERIC_ADD_S08U4(int64, byte)
_DECLARE_FUNC_NUMERIC_ADD_U08U8(uint64, byte)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(float, byte)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(double, byte)
//short
_DECLARE_FUNC_NUMERIC_ADD_U2S2_S2S2(char, short)
_DECLARE_FUNC_NUMERIC_ADD_U2S2_S2S2(byte, short)
_DECLARE_FUNC_NUMERIC_ADD_U2S2_S2S2(short, short)
_DECLARE_FUNC_NUMERIC_ADD_U2S2_S2S2(ushort, short)
_DECLARE_FUNC_NUMERIC_ADD_U04S4_U2S04_S04S4_S2S04(int, short)
_DECLARE_FUNC_NUMERIC_ADD_U04S4_U2S04_S04S4_S2S04(uint, short)
_DECLARE_FUNC_NUMERIC_ADD_S08S8(int64, short)
_DECLARE_FUNC_NUMERIC_ADD_U08S8(uint64, short)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(float, short)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(double, short)
//ushort
_DECLARE_FUNC_NUMERIC_ADD_U2U2_S2U2(char, ushort)
_DECLARE_FUNC_NUMERIC_ADD_U2U2_S2U2(byte, ushort)
_DECLARE_FUNC_NUMERIC_ADD_U2U2_S2U2(short, ushort)
_DECLARE_FUNC_NUMERIC_ADD_U2U2_S2U2(ushort, ushort)
_DECLARE_FUNC_NUMERIC_ADD_S04U2_S4U04(int, ushort)
_DECLARE_FUNC_NUMERIC_ADD_U04U4(uint, ushort)
_DECLARE_FUNC_NUMERIC_ADD_S08U4(int64, ushort)
_DECLARE_FUNC_NUMERIC_ADD_U08U8(uint64, ushort)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(float, ushort)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(double, ushort)
//int
_DECLARE_FUNC_NUMERIC_ADD_U04S4_U2S04_S04S4_S2S04(char, int)
_DECLARE_FUNC_NUMERIC_ADD_U04S4_U2S04_S04S4_S2S04(byte, int)
_DECLARE_FUNC_NUMERIC_ADD_U04S4_U2S04_S04S4_S2S04(short, int)
_DECLARE_FUNC_NUMERIC_ADD_U04S4_U2S04_S04S4_S2S04(ushort, int)
_DECLARE_FUNC_NUMERIC_ADD_U04S4_U2S04_S04S4_S2S04(int, int)
_DECLARE_FUNC_NUMERIC_ADD_U04S4_U2S04_S04S4_S2S04(uint, int)
_DECLARE_FUNC_NUMERIC_ADD_S08S8(int64, int)
_DECLARE_FUNC_NUMERIC_ADD_U08S8(uint64, int)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(float, int)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(double, int)
//uint
_DECLARE_FUNC_NUMERIC_ADD_S04U2_S4U04(char, uint)
_DECLARE_FUNC_NUMERIC_ADD_U2U04(byte, uint)
_DECLARE_FUNC_NUMERIC_ADD_S04U2_S4U04(short, uint)
_DECLARE_FUNC_NUMERIC_ADD_U2U04(ushort, uint)
_DECLARE_FUNC_NUMERIC_ADD_S04U2_S4U04(int, uint)
_DECLARE_FUNC_NUMERIC_ADD_U04U4(uint, uint)
_DECLARE_FUNC_NUMERIC_ADD_S08U4(int64, uint)
_DECLARE_FUNC_NUMERIC_ADD_U08U8(uint64, uint)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(float, uint)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(double, uint)
//int64
_DECLARE_FUNC_NUMERIC_ADD_S4S08(char, int64)
_DECLARE_FUNC_NUMERIC_ADD_U4S08(byte, int64)
_DECLARE_FUNC_NUMERIC_ADD_S4S08(short, int64)
_DECLARE_FUNC_NUMERIC_ADD_U4S08(ushort, int64)
_DECLARE_FUNC_NUMERIC_ADD_S4S08(int, int64)
_DECLARE_FUNC_NUMERIC_ADD_U4S08(uint, int64)
_DECLARE_FUNC_NUMERIC_ADD_S08S8(int64, int64)
_DECLARE_FUNC_NUMERIC_ADD_U08S8(uint64, int64)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(float, int64)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(double, int64)
//uint64
_DECLARE_FUNC_NUMERIC_ADD_S4U08(char, uint64)
_DECLARE_FUNC_NUMERIC_ADD_U4U08(byte, uint64)
_DECLARE_FUNC_NUMERIC_ADD_S4U08(short, uint64)
_DECLARE_FUNC_NUMERIC_ADD_U4U08(ushort, uint64)
_DECLARE_FUNC_NUMERIC_ADD_S4U08(int, uint64)
_DECLARE_FUNC_NUMERIC_ADD_U4U08(uint, uint64)
_DECLARE_FUNC_NUMERIC_ADD_S08U08(int64, uint64)
_DECLARE_FUNC_NUMERIC_ADD_U08U8(uint64, uint64)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(float, uint64)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(double, uint64)
//float
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(char, float)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(byte, float)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(short, float)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(ushort, float)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(int, float)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(uint, float)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(int64, float)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(uint64, float)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(float, float)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(double, float)
//double
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(char, double)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(byte, double)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(short, double)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(ushort, double)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(int, double)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(uint, double)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(int64, double)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(uint64, double)
_DECLARE_FUNC_NUMERIC_ADD_XF_XD(float, double)
_DECLARE_FUNC_NUMERIC_ADD_FX_DX(double, double)

//------------------------------------------------------------------------------
//OS

#if defined(GKC_OS_WINDOWS)
	#include "Windows/numeric_types.h"
#elif defined(GKC_OS_LINUX)
	#include "Linux/numeric_types.h"
#else
	#error Error OS type!
#endif

//------------------------------------------------------------------------------
// safe_operators

class safe_operators
{
public:
	//cast
	template <typename T, typename U>
	static call_result Cast(const T& t, U& u) throw()
	{
		return numeric_cast<T, U>(t, u);
	}
	template <typename T, typename U>
	static U CastThrow(const T& t)
	{
		U u;
		call_result cr(Cast<T, U>(t, u));
		if( cr.IsFailed() )
			throw exception_base(cr);
		return u;
	}
	//negative
	template <typename T>
	static call_result Negative(const T& t, T& r) throw()
	{
		return numeric_negative<T>(t, r);
	}
	template <typename T>
	static T NegativeThrow(const T& t)
	{
		T r;
		call_result cr(Negative<T>(t, r));
		if( cr.IsFailed() )
			throw exception_base(cr);
		return r;
	}
	//modulus
	template <typename T, typename U>
	static call_result Modulus(const T& t, const U& u, T& r) throw()
	{
		return numeric_modulus<T, U>(t, u, r);
	}
	template <typename T, typename U>
	static T ModulusThrow(const T& t, const U& u)
	{
		T r;
		call_result cr(Modulus<T, U>(t, u, r));
		if( cr.IsFailed() )
			throw exception_base(cr);
		return r;
	}
	//divide
	template <typename T, typename U>
	static call_result Divide(const T& t, const U& u, T& r) throw()
	{
		return numeric_divide<T, U>(t, u, r);
	}
	template <typename T, typename U>
	static T DivideThrow(const T& t, const U& u)
	{
		T r;
		call_result cr(Divide<T, U>(t, u, r));
		if( cr.IsFailed() )
			throw exception_base(cr);
		return r;
	}
	//multiply
	template <typename T, typename U>
	static call_result Multiply(const T& t, const U& u, T& r) throw()
	{
		return numeric_multiply<T, U>(t, u, r);
	}
	template <typename T, typename U>
	static T MultiplyThrow(const T& t, const U& u)
	{
		T r;
		call_result cr(Multiply<T, U>(t, u, r));
		if( cr.IsFailed() )
			throw exception_base(cr);
		return r;
	}
	//subtract
	template <typename T, typename U>
	static call_result Subtract(const T& t, const U& u, T& r) throw()
	{
		return numeric_subtract<T, U>(t, u, r);
	}
	template <typename T, typename U>
	static T SubtractThrow(const T& t, const U& u)
	{
		T r;
		call_result cr(Subtract<T, U>(t, u, r));
		if( cr.IsFailed() )
			throw exception_base(cr);
		return r;
	}
	//version 2
	template <typename T, typename U>
	static call_result Subtract(const T& t, const U& u, U& r) throw()
	{
		return numeric_subtract<T, U>(t, u, r);
	}
	template <typename T, typename U>
	static U SubtractThrow(const T& t, const U& u)
	{
		U r;
		call_result cr(Subtract<T, U>(t, u, r));
		if( cr.IsFailed() )
			throw exception_base(cr);
		return r;
	}
	//add
	template <typename T, typename U>
	static call_result Add(const T& t, const U& u, T& r) throw()
	{
		return numeric_add<T, U>(t, u, r);
	}
	template <typename T, typename U>
	static T AddThrow(const T& t, const U& u)
	{
		T r;
		call_result cr(Add<T, U>(t, u, r));
		if( cr.IsFailed() )
			throw exception_base(cr);
		return r;
	}
};

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
