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

//Windows

//------------------------------------------------------------------------------
// Currency

class _os_com_currency
{
public:
// constructors
	_os_com_currency() throw()
	{
		m_currency.int64 = 0;
	}
	explicit _os_com_currency(_In_ CURRENCY cySrc) throw()
	{
		m_currency.int64 = cySrc.int64;
	}
	_os_com_currency(_In_ const _os_com_currency& curSrc) throw()
	{
		*this = curSrc;
	}
	_os_com_currency(_In_ LONGLONG nInteger, _In_ SHORT nFraction)
	{
		m_currency.int64 = 0;
		HRESULT hRes = SetInteger(nInteger);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		hRes = SetFraction(nFraction);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
	}

	_os_com_currency(_In_ BYTE bSrc)
	{
		*this = bSrc;
	}
	_os_com_currency(_In_ SHORT sSrc)
	{
		*this = sSrc;
	}
	_os_com_currency(_In_ LONG lSrc)
	{
		*this = lSrc;
	}
	_os_com_currency(_In_ FLOAT fSrc)
	{
		*this = fSrc;
	}
	_os_com_currency(_In_ DOUBLE dSrc)
	{
		*this = dSrc;
	}
	_os_com_currency(_In_ CHAR cSrc)
	{
		*this = cSrc;
	}
	_os_com_currency(_In_ USHORT usSrc)
	{
		*this = usSrc;
	}
	_os_com_currency(_In_ ULONG ulSrc)
	{
		*this = ulSrc;
	}
	_os_com_currency(_In_ DECIMAL dSrc)
	{
		*this = dSrc;
	}

	explicit _os_com_currency(_In_z_ LPCWSTR szSrc)
	{
		assert( szSrc != NULL );
		HRESULT hRes = ::VarCyFromStr(const_cast<LPWSTR>(szSrc), ::GetThreadLocale(), LOCALE_NOUSEROVERRIDE, &m_currency);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
	}
	explicit _os_com_currency(_In_ const VARIANT& varSrc)
	{
		VARIANT var;
		::VariantInit(&var);
		HRESULT hRes = ::VariantChangeType(&var, const_cast<VARIANT*>(&varSrc), 0, VT_CY);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		m_currency.int64 = V_CY(&var).int64;
	}
	explicit _os_com_currency(_In_ LPDISPATCH pDispSrc)
	{
		assert( pDispSrc != NULL );
		HRESULT hRes = ::VarCyFromDisp(pDispSrc, ::GetThreadLocale(), &m_currency);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
	}

	~_os_com_currency() throw()
	{
	}

// assignment operators
	const _os_com_currency& operator=(_In_ CURRENCY cySrc) throw()
	{
		m_currency.int64 = cySrc.int64;
		return *this;
	}
	const _os_com_currency& operator=(_In_ const _os_com_currency& curSrc) throw()
	{
		m_currency.int64 = curSrc.m_currency.int64;
		return *this;
	}

	const _os_com_currency& operator=(_In_ BYTE bSrc)
	{
		HRESULT hRes = SetValue(bSrc);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return *this;
	}
	const _os_com_currency& operator=(_In_ SHORT sSrc)
	{
		HRESULT hRes = SetValue(sSrc);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return *this;
	}
	const _os_com_currency& operator=(_In_ LONG lSrc)
	{
		HRESULT hRes = SetValue(lSrc);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return *this;
	}
	const _os_com_currency& operator=(_In_ FLOAT fSrc)
	{
		HRESULT hRes = SetValue(fSrc);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return *this;
	}
	const _os_com_currency& operator=(_In_ DOUBLE dSrc)
	{
		HRESULT hRes = SetValue(dSrc);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return *this;
	}
	const _os_com_currency& operator=(_In_ CHAR cSrc)
	{
		HRESULT hRes = SetValue(cSrc);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return *this;
	}
	const _os_com_currency& operator=(_In_ USHORT usSrc)
	{
		HRESULT hRes = SetValue(usSrc);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return *this;
	}
	const _os_com_currency& operator=(_In_ ULONG ulSrc)
	{
		HRESULT hRes = SetValue(ulSrc);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return *this;
	}
	const _os_com_currency& operator=(_In_ DECIMAL dSrc)
	{
		HRESULT hRes = ::VarCyFromDec(&dSrc, &m_currency);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return *this;
	}

// comparison operators
	bool operator==(_In_ const _os_com_currency& cur) const throw()
	{
		return (static_cast<HRESULT>(VARCMP_EQ) == ::VarCyCmp(m_currency, cur.m_currency));
	}
	bool operator!=(_In_ const _os_com_currency& cur) const throw()
	{
		return (static_cast<HRESULT>(VARCMP_EQ) != ::VarCyCmp(m_currency, cur.m_currency));
	}
	bool operator<(_In_ const _os_com_currency& cur) const throw()
	{
		return (static_cast<HRESULT>(VARCMP_LT) == ::VarCyCmp(m_currency, cur.m_currency));
	}
	bool operator>(_In_ const _os_com_currency& cur) const throw()
	{
		return (static_cast<HRESULT>(VARCMP_GT) == ::VarCyCmp(m_currency, cur.m_currency));
	}
	bool operator<=(_In_ const _os_com_currency& cur) const throw()
	{
		HRESULT hRes = ::VarCyCmp(m_currency, cur.m_currency);
		return (static_cast<HRESULT>(VARCMP_LT) == hRes || (HRESULT)VARCMP_EQ == hRes);
	}
	bool operator>=(_In_ const _os_com_currency& cur) const throw()
	{
		HRESULT hRes = ::VarCyCmp(m_currency, cur.m_currency);
		return (static_cast<HRESULT>(VARCMP_GT) == hRes || static_cast<HRESULT>(VARCMP_EQ) == hRes);
	}

// math operators
	_os_com_currency operator+(_In_ const _os_com_currency& cur) const
	{
		CURRENCY cy;
		HRESULT hRes = Add(cur, cy);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return _os_com_currency(cy);
	}
	_os_com_currency operator-(_In_ const _os_com_currency& cur) const
	{
		CURRENCY cy;
		HRESULT hRes = Subtract(cur, cy);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return _os_com_currency(cy);
	}
	const _os_com_currency& operator+=(_In_ const _os_com_currency& cur)
	{
		HRESULT hRes = Add(cur);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return *this;
	}
	const _os_com_currency& operator-=(_In_ const _os_com_currency& cur)
	{
		HRESULT hRes = Subtract(cur);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return *this;
	}
	_os_com_currency operator*(_In_ const _os_com_currency& cur) const
	{
		CURRENCY cy;
		HRESULT hRes = Multiply(cur, cy);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return _os_com_currency(cy);
	}
	const _os_com_currency& operator*=(_In_ const _os_com_currency& cur)
	{
		HRESULT hRes = Multiply(cur);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return *this;
	}
	_os_com_currency operator*(_In_ long nOperand) const
	{
		CURRENCY cy;
		HRESULT hRes = Multiply(nOperand, cy);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return _os_com_currency(cy);
	}
	const _os_com_currency& operator*=(_In_ long nOperand)
	{
		HRESULT hRes = Multiply(nOperand);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return *this;
	}
	_os_com_currency operator-() const
	{
		CURRENCY cy;
		HRESULT hRes = Negative(cy);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return _os_com_currency(cy);
	}
	_os_com_currency operator/(_In_ long nOperand) const
	{
		CURRENCY cy;
		HRESULT hRes = Divide(nOperand, cy);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return _os_com_currency(cy);
	}
	const _os_com_currency& operator/=(_In_ long nOperand)
	{
		HRESULT hRes = Divide(nOperand);
		if( FAILED(hRes) )
			throw exception_base(call_result((int)hRes));
		return *this;
	}

//properties
	HRESULT SetInteger(_In_ LONGLONG nInteger) throw()
	{
		// check if within range
		if( nInteger < _CY_MIN_INTEGER || nInteger > _CY_MAX_INTEGER )
			return E_INVALIDARG;

		if( m_currency.int64 != 0 ) {
			// signs must match
			if( (m_currency.int64 < 0 && nInteger > 0)
				|| (m_currency.int64 > 0 && nInteger < 0) )
				return TYPE_E_TYPEMISMATCH;

			CURRENCY cyTemp;
			// get fractional part
			cyTemp.int64 = m_currency.int64 % _CY_SCALE;
			// check if within range again
			if( (nInteger == _CY_MAX_INTEGER && cyTemp.int64 > _CY_MAX_TAIL)
				|| (nInteger == _CY_MIN_INTEGER && cyTemp.int64 < _CY_MIN_TAIL) )
				return TYPE_E_OUTOFBOUNDS;
			// set to fractional part, wiping out integer part
			m_currency.int64 = cyTemp.int64;
		}
		// add new integer part scaled by _CY_SCALE
		m_currency.int64 += (nInteger * _CY_SCALE);
		return S_OK;
	}

	// Based on 4 digits.  To set .2, pass 2000, to set .0002, pass a 2
	HRESULT SetFraction(_In_ SHORT nFraction) throw()
	{
		// check if within range
		if( nFraction < _CY_MIN_FRACTION || nFraction > _CY_MAX_FRACTION )
			return E_INVALIDARG;

		if( m_currency.int64 != 0 ) {
			// signs must match
			if( (m_currency.int64 < 0 && nFraction > 0)
				|| (m_currency.int64 > 0 && nFraction < 0) )
				return TYPE_E_TYPEMISMATCH;

			CURRENCY cyTemp;
			// get integer part, wiping out fractional part
			cyTemp.int64 = m_currency.int64 / _CY_SCALE;
			// check if within range again
			if( (cyTemp.int64 == _CY_MAX_INTEGER && nFraction > _CY_MAX_TAIL)
				|| (cyTemp.int64 == _CY_MIN_INTEGER && nFraction < _CY_MIN_TAIL) )
				return TYPE_E_OUTOFBOUNDS;
			// scale to _CY_SCALE
			m_currency.int64 = cyTemp.int64 * _CY_SCALE;
		}
		m_currency.int64 += nFraction;
		return S_OK;
	}

	LONGLONG GetInteger() const throw()
	{
		if( m_currency.int64 != 0 )
			return (m_currency.int64 / _CY_SCALE);
		return 0;
	}
	SHORT GetFraction() const throw()
	{
		if( m_currency.int64 != 0 )
			// get fractional part
			return static_cast<SHORT>(m_currency.int64 % _CY_SCALE);
		return 0;
	}

// misc functions
	HRESULT Round(_In_ int nDecimals) throw()
	{
		if( nDecimals < 0 || nDecimals > _CY_DECIMAL_DIGITS )
			return E_INVALIDARG;

		return ::VarCyRound(m_currency, nDecimals, &m_currency);
	}

// cast operators
	operator CURRENCY&() throw()
	{
		return m_currency;
	}
	operator const CURRENCY&() const throw()
	{
		return m_currency;
	}
	CURRENCY* GetCurrencyPtr() throw()
	{
		return &m_currency;
	}

// set value
	HRESULT SetValue(BYTE bSrc) throw()
	{
		return ::VarCyFromUI1(bSrc, &m_currency);
	}
	HRESULT SetValue(SHORT sSrc) throw()
	{
		return ::VarCyFromI2(sSrc, &m_currency);
	}
	HRESULT SetValue(LONG lSrc) throw()
	{
		return ::VarCyFromI4(lSrc, &m_currency);
	}
	HRESULT SetValue(FLOAT fSrc) throw()
	{
		return ::VarCyFromR4(fSrc, &m_currency);
	}
	HRESULT SetValue(DOUBLE dSrc) throw()
	{
		return ::VarCyFromR8(dSrc, &m_currency);
	}
	HRESULT SetValue(CHAR cSrc) throw()
	{
		return ::VarCyFromI1(cSrc, &m_currency);
	}
	HRESULT SetValue(USHORT usSrc) throw()
	{
		return ::VarCyFromUI2(usSrc, &m_currency);
	}
	HRESULT SetValue(ULONG ulSrc) throw()
	{
		return ::VarCyFromUI4(ulSrc, &m_currency);
	}

// arithmetic
	HRESULT Add(const _os_com_currency& cur) throw()
	{
		return ::VarCyAdd(m_currency, cur.m_currency, &m_currency);
	}
	HRESULT Add(const _os_com_currency& cur, CURRENCY& cy) const throw()
	{
		return ::VarCyAdd(m_currency, cur.m_currency, &cy);
	}
	HRESULT Subtract(const _os_com_currency& cur) throw()
	{
		return ::VarCySub(m_currency, cur.m_currency, &m_currency);
	}
	HRESULT Subtract(const _os_com_currency& cur, CURRENCY& cy) const throw()
	{
		return ::VarCySub(m_currency, cur.m_currency, &cy);
	}
	HRESULT Multiply(const _os_com_currency& cur) throw()
	{
		return ::VarCyMul(m_currency, cur.m_currency, &m_currency);
	}
	HRESULT Multiply(const _os_com_currency& cur, CURRENCY& cy) const throw()
	{
		return ::VarCyMul(m_currency, cur.m_currency, &cy);
	}
	HRESULT Multiply(long nOperand) throw()
	{
		return ::VarCyMulI4(m_currency, nOperand, &m_currency);
	}
	HRESULT Multiply(long nOperand, CURRENCY& cy) const throw()
	{
		return ::VarCyMulI4(m_currency, nOperand, &cy);
	}
	HRESULT Negative() throw()
	{
		return ::VarCyNeg(m_currency, &m_currency);
	}
	HRESULT Negative(CURRENCY& cy) const throw()
	{
		return ::VarCyNeg(m_currency, &cy);
	}
	HRESULT Divide(long nOperand) throw()
	{
		return Divide(nOperand, m_currency);
	}
	HRESULT Divide(long nOperand, CURRENCY& cy) const throw()
	{
		if( nOperand == 0 )
			return E_INVALIDARG;
		if( m_currency.int64 == limits_base<int64>::Lowest && nOperand == -1 )
			return CR_OVERFLOW;
		cy.int64 = m_currency.int64 / nOperand;
		return S_OK;
	}

private:
	CURRENCY m_currency;
};

#pragma pack(push, 1)

// currency_value

class currency_value
{
public:
	currency_value() noexcept
	{
	}
	currency_value(const currency_value& src) noexcept : m_value(src.m_value)
	{
	}
	currency_value(int64 iInteger, short sFraction) : m_value((LONGLONG)iInteger, sFraction)
	{
	}

	~currency_value() noexcept
	{
	}

//properties
	call_result SetInteger(int64 iInteger) noexcept
	{
		return call_result((int)m_value.SetInteger((LONGLONG)iInteger));
	}
	call_result SetFraction(short sFraction) noexcept
	{
		return call_result((int)m_value.SetFraction(sFraction));
	}
	int64 GetInteger() const noexcept
	{
		return (int64)m_value.GetInteger();
	}
	short GetFraction() const noexcept
	{
		return m_value.GetFraction();
	}

// misc functions
	call_result Round(int iDecimals) noexcept
	{
		return call_result((int)m_value.Round(iDecimals));
	}

// set value
	void SetValue(byte btSrc) noexcept
	{
		m_value = _os_com_currency();
		m_value.SetValue(btSrc);
	}
	void SetValue(short sSrc) noexcept
	{
		m_value = _os_com_currency();
		m_value.SetValue(sSrc);
	}
	void SetValue(int iSrc) noexcept
	{
		m_value = _os_com_currency();
		m_value.SetValue((LONG)iSrc);
	}
	call_result SetValue(float fSrc) noexcept
	{
		return call_result((int)m_value.SetValue(fSrc));
	}
	call_result SetValue(double dSrc) noexcept
	{
		return call_result((int)m_value.SetValue(dSrc));
	}
	void SetValue(char cSrc) noexcept
	{
		m_value = _os_com_currency();
		m_value.SetValue(cSrc);
	}
	void SetValue(ushort usSrc) noexcept
	{
		m_value = _os_com_currency();
		m_value.SetValue(usSrc);
	}
	void SetValue(uint uiSrc) noexcept
	{
		m_value = _os_com_currency();
		m_value.SetValue((ULONG)uiSrc);
	}

// arithmetic
	call_result Add(const currency_value& cur) noexcept
	{
		return call_result((int)m_value.Add(cur.m_value));
	}
	call_result Subtract(const currency_value& cur) noexcept
	{
		return call_result((int)m_value.Subtract(cur.m_value));
	}
	call_result Multiply(const currency_value& cur) noexcept
	{
		return call_result((int)m_value.Multiply(cur.m_value));
	}
	call_result Multiply(int iValue) noexcept
	{
		return call_result((int)m_value.Multiply((long)iValue));
	}
	call_result Negative() noexcept
	{
		return call_result((int)m_value.Negative());
	}
	call_result Divide(int iValue) noexcept
	{
		return call_result((int)m_value.Divide((long)iValue));
	}

// assignment operators
	currency_value& operator=(const currency_value& curSrc) noexcept
	{
		m_value = curSrc.m_value;
		return *this;
	}

// comparison operators
	bool operator==(const currency_value& cur) const noexcept
	{
		return m_value == cur.m_value;
	}
	bool operator!=(const currency_value& cur) const noexcept
	{
		return m_value != cur.m_value;
	}
	bool operator<(const currency_value& cur) const noexcept
	{
		return m_value < cur.m_value;
	}
	bool operator>(const currency_value& cur) const noexcept
	{
		return m_value > cur.m_value;
	}
	bool operator<=(const currency_value& cur) const noexcept
	{
		return m_value <= cur.m_value;
	}
	bool operator>=(const currency_value& cur) const noexcept
	{
		return m_value >= cur.m_value;
	}

// math operators
	currency_value operator+(const currency_value& cur) const
	{
		return currency_value(m_value + cur.m_value);  //may throw
	}
	currency_value operator-(const currency_value& cur) const
	{
		return currency_value(m_value - cur.m_value);  //may throw
	}
	currency_value& operator+=(const currency_value& cur)
	{
		m_value += cur.m_value;  //may throw
		return *this;
	}
	currency_value& operator-=(const currency_value& cur)
	{
		m_value -= cur.m_value;  //may throw
		return *this;
	}
	currency_value operator*(const currency_value& cur) const
	{
		return currency_value(m_value * cur.m_value);  //may throw
	}
	currency_value& operator*=(const currency_value& cur)
	{
		m_value *= cur.m_value;  //may throw
		return *this;
	}
	currency_value operator*(int iValue) const
	{
		return currency_value(m_value * (long)iValue);  //may throw
	}
	currency_value& operator*=(int iValue)
	{
		m_value *= (long)iValue;  //may throw
		return *this;
	}
	currency_value operator-() const
	{
		return currency_value(-m_value);  //may throw
	}
	currency_value operator/(int iValue) const
	{
		return currency_value(m_value / (long)iValue);  //may throw
	}
	currency_value& operator/=(int iValue)
	{
		m_value /= (long)iValue;  //may throw
		return *this;
	}

private:
	explicit currency_value(const _os_com_currency& curSrc) noexcept : m_value(curSrc)
	{
	}

private:
	_os_com_currency  m_value;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
