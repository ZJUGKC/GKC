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

//Linux

//------------------------------------------------------------------------------
// Currency

#pragma pack(push, 1)

// currency_value

class currency_value
{
public:
	currency_value() noexcept : m_value(0)
	{
	}
	currency_value(const currency_value& src) noexcept : m_value(src.m_value)
	{
	}
	currency_value(int64 iInteger, short sFraction)
	{
		m_value = 0;
		call_result cr = SetInteger(iInteger);
		if( cr.IsFailed() )
			throw exception_base(cr);
		cr = SetFraction(sFraction);
		if( cr.IsFailed() )
			throw exception_base(cr);
	}

	~currency_value() noexcept
	{
	}

//properties
	call_result SetInteger(int64 iInteger) noexcept
	{
		// check if within range
		if( iInteger < _CY_MIN_INTEGER || iInteger > _CY_MAX_INTEGER )
			return call_result(CR_INVALID);

		if( m_value != 0 ) {
			// signs must match
			if( (m_value < 0 && iInteger > 0)
				|| (m_value > 0 && iInteger < 0) )
				return call_result(CR_INVALID);

			// get fractional part
			int64 iTemp = m_value % _CY_SCALE;
			// check if within range again
			if( (iInteger == _CY_MAX_INTEGER && iTemp > _CY_MAX_TAIL)
				|| (iInteger == _CY_MIN_INTEGER && iTemp < _CY_MIN_TAIL) )
				return call_result(CR_OVERFLOW);
			// set to fractional part, wiping out integer part
			m_value = iTemp;
		}
		// add new integer part scaled by _CY_SCALE
		m_value += (iInteger * _CY_SCALE);
		return call_result();
	}
	call_result SetFraction(short sFraction) noexcept
	{
		// check if within range
		if( sFraction < _CY_MIN_FRACTION || sFraction > _CY_MAX_FRACTION )
			return call_result(CR_INVALID);

		if( m_value != 0 ) {
			// signs must match
			if( (m_value < 0 && sFraction > 0)
				|| (m_value > 0 && sFraction < 0) )
				return call_result(CR_INVALID);

			// get integer part, wiping out fractional part
			int64 iTemp = m_value / _CY_SCALE;
			// check if within range again
			if( (iTemp == _CY_MAX_INTEGER && sFraction > _CY_MAX_TAIL)
				|| (iTemp == _CY_MIN_INTEGER && sFraction < _CY_MIN_TAIL) )
				return call_result(CR_OVERFLOW);
			// scale to _CY_SCALE
			m_value = iTemp * _CY_SCALE;
		}
		m_value += sFraction;
		return call_result();
	}
	int64 GetInteger() const noexcept
	{
		return m_value == 0 ? 0 : m_value / _CY_SCALE;
	}
	short GetFraction() const noexcept
	{
		return m_value == 0 ? 0 : (short)(m_value % _CY_SCALE);
	}

// misc functions
	call_result Round(int iDecimals) noexcept
	{
		if( iDecimals < 0 || iDecimals > _CY_DECIMAL_DIGITS )
			return call_result(CR_INVALID);
		if( iDecimals == _CY_DECIMAL_DIGITS )
			return call_result();
		int scale1 = _CY_SCALE;
		for( int i = 0; i < iDecimals; i ++ )
			scale1 /= 10;
		int64 iTemp1 = m_value / scale1 * scale1;
		int64 iTemp2 = m_value - iTemp1;
		if( iTemp2 < 0 )
			iTemp2 = -iTemp2;
		iTemp2 /= (scale1 / 10);
		if( iTemp2 >= 5 ) {
			if( iTemp1 < 0 )
				scale1 = -scale1;
			call_result cr(safe_operators::Add(iTemp1, scale1, iTemp2));
			if( cr.IsSucceeded() )
				m_value = iTemp2;
			return cr;
		}
		m_value = iTemp1;
		return call_result();
	}

// set value
	void SetValue(byte btSrc) noexcept
	{
		m_value = (int64)btSrc * _CY_SCALE;
	}
	void SetValue(short sSrc) noexcept
	{
		m_value = (int64)sSrc * _CY_SCALE;
	}
	void SetValue(int iSrc) noexcept
	{
		m_value = (int64)iSrc * _CY_SCALE;
	}
	call_result SetValue(float fSrc) noexcept
	{
		return safe_operators::Multiply((int64)_CY_SCALE, fSrc, m_value);
	}
	call_result SetValue(double dSrc) noexcept
	{
		return safe_operators::Multiply((int64)_CY_SCALE, dSrc, m_value);
	}
	void SetValue(char cSrc) noexcept
	{
		m_value = (int64)cSrc * _CY_SCALE;
	}
	void SetValue(ushort usSrc) noexcept
	{
		m_value = (int64)usSrc * _CY_SCALE;
	}
	void SetValue(uint uiSrc) noexcept
	{
		m_value = (int64)uiSrc * _CY_SCALE;
	}

// arithmetic
	call_result Add(const currency_value& cur) noexcept
	{
		return safe_operators::Add(m_value, cur.m_value, m_value);
	}
	call_result Subtract(const currency_value& cur) noexcept
	{
		return safe_operators::Subtract(m_value, cur.m_value, m_value);
	}
	call_result Multiply(const currency_value& cur) noexcept
	{
		int64 a1 = GetInteger();
		int64 b1 = GetFraction();
		int64 a2 = cur.GetInteger();
		int64 b2 = cur.GetFraction();
		// a1 * a2 * 10000 + a1 * b2 + a2 * b1 + b1 * b2 / 10000
		int64 tmp;
		call_result cr;
		cr = safe_operators::Multiply(a1, a2, tmp);
		if( cr.IsFailed() )
			return cr;
		cr = safe_operators::Multiply(tmp, _CY_SCALE, tmp);
		if( cr.IsFailed() )
			return cr;
		cr = safe_operators::Add(tmp, a1 * b2, tmp);
		if( cr.IsFailed() )
			return cr;
		cr = safe_operators::Add(tmp, a2 * b1, tmp);
		if( cr.IsFailed() )
			return cr;
		cr = safe_operators::Add(tmp, b1 * b2 / _CY_SCALE, tmp);
		if( cr.IsFailed() )
			return cr;
		m_value = tmp;
		return cr;
	}
	call_result Multiply(int iValue) noexcept
	{
		return safe_operators::Multiply(m_value, iValue, m_value);
	}
	call_result Negative() noexcept
	{
		return safe_operators::Negative(m_value, m_value);
	}
	call_result Divide(int iValue) noexcept
	{
		return safe_operators::Divide(m_value, iValue, m_value);
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
		currency_value res(*this);
		return res += cur;  //may throw
	}
	currency_value operator-(const currency_value& cur) const
	{
		currency_value res(*this);
		return res -= cur;  //may throw
	}
	currency_value& operator+=(const currency_value& cur)
	{
		call_result cr(Add(cur));
		if( cr.IsFailed() )
			throw exception_base(cr);
		return *this;
	}
	currency_value& operator-=(const currency_value& cur)
	{
		call_result cr(Subtract(cur));
		if( cr.IsFailed() )
			throw exception_base(cr);
		return *this;
	}
	currency_value operator*(const currency_value& cur) const
	{
		currency_value res(*this);
		return res *= cur;  //may throw
	}
	currency_value& operator*=(const currency_value& cur)
	{
		call_result cr(Multiply(cur));
		if( cr.IsFailed() )
			throw exception_base(cr);
		return *this;
	}
	currency_value operator*(int iValue) const
	{
		currency_value res(*this);
		return res *= iValue;  //may throw
	}
	currency_value& operator*=(int iValue)
	{
		call_result cr(Multiply(iValue));
		if( cr.IsFailed() )
			throw exception_base(cr);
		return *this;
	}
	currency_value operator-() const
	{
		currency_value res(*this);
		call_result cr(res.Negative());
		if( cr.IsFailed() )
			throw exception_base(cr);
		return res;
	}
	currency_value operator/(int iValue) const
	{
		currency_value res(*this);
		return res /= iValue;  //may throw
	}
	currency_value& operator/=(int iValue)
	{
		call_result cr(Divide(iValue));
		if( cr.IsFailed() )
			throw exception_base(cr);
		return *this;
	}

private:
	int64  m_value;
};

#pragma pack(pop)

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
