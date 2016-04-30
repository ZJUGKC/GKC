/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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
This file contains classes for Graphics Device Interface.

Author: Lijuan Mei

Reference: WTL
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

// protect template members from windowsx.h macros
#ifdef _INC_WINDOWSX
  #undef CopyRgn
  #undef CreateBrush
  #undef CreatePen
  #undef SelectBrush
  #undef SelectPen
  #undef SelectFont
  #undef SelectBitmap
#endif // _INC_WINDOWSX

// required libraries
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "opengl32.lib")

////////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// _os_gdi_object
//
// _os_pen
// _os_brush
// _os_log_font
// _os_font
// _os_bitmap
// _os_palette
// _os_rgn
//
// _os_dc
// _os_paint_dc
// _os_client_dc
// _os_window_dc
// _os_memory_dc
//
// _os_enh_meta_file_info
// _os_enh_meta_file
// _os_enh_meta_file_dc
//
////////////////////////////////////////////////////////////////////////////////

// _os_gdi_object

class _os_gdi_object
{
public:
	explicit _os_gdi_object(HGDIOBJ h = NULL) throw() : m_hObj(h)
	{
	}
	_os_gdi_object(_os_gdi_object&& src) throw()
	{
		m_hObj = src.m_hObj;
		src.m_hObj = NULL;
	}
	~_os_gdi_object() throw()
	{
		Destroy();
	}

	void Destroy() throw()
	{
		if( m_hObj != NULL ) {
			BOOL bRet = ::DeleteObjectW(m_hObj);
			assert( bRet );
			m_hObj = NULL;
		}
	}

	//operators
	_os_gdi_object& operator=(_os_gdi_object&& src) throw()
	{
		if( &src != this ) {
			if( m_hObj != src.m_hObj ) {
				Destroy();
				m_hObj = src.m_hObj;
				src.m_hObj = NULL;
			}
		}
		return *this;
	}

	//methods
	HGDIOBJ GetHandle() const throw()
	{
		return m_hObj;
	}
	bool IsNull() const throw()
	{
		return m_hObj == NULL;
	}

	void Attach(HGDIOBJ h) throw()
	{
		if( h != m_hObj )
			Destroy();
		m_hObj = h;
	}
	HGDIOBJ Detach() throw()
	{
		HGDIOBJ h = m_hObj;
		m_hObj = NULL;
		return h;
	}

protected:
	HGDIOBJ  m_hObj;

private:
	//noncopyable
	_os_gdi_object(const _os_gdi_object&) throw();
	_os_gdi_object& operator=(const _os_gdi_object&) throw();
};

// weak_object_ref<T>
//   T: _os_pen, _os_brush, _os_font, _os_bitmap, _os_palette, _os_rgn

////////////////////////////////////////////////////////////////////////////////
// _os_pen

class _os_pen : public _os_gdi_object
{
private:
	typedef _os_gdi_object  baseClass;

public:
	explicit _os_pen(HPEN hPen = NULL) throw() : baseClass(hPen)
	{
	}
	_os_pen(_os_pen&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_os_pen() throw()
	{
	}

	_os_pen& operator=(_os_pen&& src) throw()
	{
		return static_cast<_os_pen&>(baseClass::operator=(rv_forward(static_cast<baseClass&>(src))));
	}

//methods
	void Attach(HPEN hPen) throw()
	{
		baseClass::Attach(hPen);
	}
	HPEN Detach() throw()
	{
		return (HPEN)baseClass::Detach();
	}

	HPEN GetHandle() const throw()
	{
		return (HPEN)baseClass::GetHandle();
	}

// Create methods
	bool Create(int iPenStyle, int iWidth, COLORREF crColor) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreatePen(iPenStyle, iWidth, crColor);
		return m_hObj != NULL;
	}
	bool Create(int iPenStyle, int iWidth, const LOGBRUSH* pLogBrush, int iStyleCount = 0, const DWORD* lpStyle = NULL) throw()
	{
		assert( IsNull() );
		m_hObj = ::ExtCreatePen(iPenStyle, iWidth, pLogBrush, iStyleCount, lpStyle);
		return m_hObj != NULL;
	}
	bool Create(const LOGPEN* lpLogPen) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreatePenIndirect(lpLogPen);
		return m_hObj != NULL;
	}

// Attributes
	bool GetLogPen(LOGPEN& LogPen) const throw()
	{
		assert( !IsNull() );
		return (::GetObjectW(m_hObj, sizeof(LOGPEN), &LogPen) == sizeof(LOGPEN));
	}
	bool GetExtLogPen(EXTLOGPEN& ExtLogPen) const throw()
	{
		assert( !IsNull() );
		int iSize = sizeof(EXTLOGPEN);
		int ret = ::GetObjectW(m_hObj, iSize, &ExtLogPen);
		return ((ret > 0) && (ret <= iSize));
	}

private:
	//noncopyable
	_os_pen(const _os_pen& src) throw();
	_os_pen& operator=(const _os_pen& src) throw();
};

////////////////////////////////////////////////////////////////////////////////
// _os_brush

class _os_brush : public _os_gdi_object
{
private:
	typedef _os_gdi_object  baseClass;

public:
	explicit _os_brush(HBRUSH hBrush = NULL) throw() : baseClass(hBrush)
	{
	}
	_os_brush(_os_brush&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_os_brush() throw()
	{
	}

	_os_brush& operator=(_os_brush&& src) throw()
	{
		return static_cast<_os_brush&>(baseClass::operator=(rv_forward(static_cast<baseClass&>(src))));
	}

	void Attach(HBRUSH hBrush) throw()
	{
		baseClass::Attach(hBrush);
	}
	HBRUSH Detach() throw()
	{
		return (HBRUSH)baseClass::Detach();
	}

	HBRUSH GetHandle() const throw()
	{
		return (HBRUSH)baseClass::GetHandle();
	}

// Create methods
	bool Create(COLORREF crColor) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateSolidBrush(crColor);
		return m_hObj != NULL;
	}
	bool Create(const LOGBRUSH* lpLogBrush) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateBrushIndirect(lpLogBrush);
		return m_hObj != NULL;
	}
	bool Create(HBITMAP hBitmap) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreatePatternBrush(hBitmap);
		return m_hObj != NULL;
	}
	//DIB
	bool Create(HGLOBAL hPackedDIB, UINT uUsage) throw()
	{
		assert( IsNull() );
		assert( hPackedDIB != NULL );
		const void* lpPackedDIB = ::GlobalLock(hPackedDIB);
		assert( lpPackedDIB != NULL );
		m_hObj = ::CreateDIBPatternBrushPt(lpPackedDIB, uUsage);
		::GlobalUnlock(hPackedDIB);
		return m_hObj != NULL;
	}
	bool Create(const void* lpPackedDIB, UINT uUsage) throw()
	{
		assert( IsNull() );
		assert( lpPackedDIB != NULL );
		m_hObj = ::CreateDIBPatternBrushPt(lpPackedDIB, uUsage);
		return m_hObj != NULL;
	}
	//SysColor Brush
	bool CreateSysColorBrush(int iIndex) throw()
	{
		assert( IsNull() );
		m_hObj = ::GetSysColorBrush(iIndex);
		return m_hObj != NULL;
	}

// Attributes
	bool GetLogBrush(LOGBRUSH& LogBrush) const throw()
	{
		assert( !IsNull() );
		return (::GetObjectW(m_hObj, sizeof(LOGBRUSH), &LogBrush) == sizeof(LOGBRUSH));
	}

private:
	//noncopyable
	_os_brush(const _os_brush& src) throw();
	_os_brush& operator=(const _os_brush& src) throw();
};

////////////////////////////////////////////////////////////////////////////////
// _os_font

class _os_log_font : public LOGFONTW
{
public:
	_os_log_font() throw()
	{
		mem_zero(this, sizeof(LOGFONTW));
	}
	explicit _os_log_font(const LOGFONTW& lf) throw()
	{
		Copy(&lf);
	}
	explicit _os_log_font(HFONT hFont) throw()
	{
		assert( ::GetObjectType(hFont) == OBJ_FONT );
		::GetObjectW(hFont, sizeof(LOGFONTW), (LOGFONTW*)this);  //no check
	}

//methods
	HFONT Create() throw()
	{
		return ::CreateFontIndirectW(this);
	}

	void SetBold() throw()
	{
		lfWeight = FW_BOLD;
	}
	bool IsBold() const throw()
	{
		return (lfWeight >= FW_BOLD);
	}
	void MakeBolder(int iScale = 1) throw()
	{
		lfWeight += (FW_BOLD * iScale);
	}
	void MakeLarger(int iScale) throw()
	{
		if( lfHeight > 0 )
			lfHeight += iScale;
		else
			lfHeight -= iScale;
	}

	void SetHeight(LONG lPointSize, HDC hDC = NULL) throw()
	{
		HDC hDC1 = (hDC != NULL) ? hDC : ::GetDC(NULL);
		// For MM_TEXT mapping mode
		lfHeight = -::MulDiv(lPointSize, ::GetDeviceCaps(hDC1, LOGPIXELSY), 72);
		if( hDC == NULL )
			::ReleaseDC(NULL, hDC1);
	}
	LONG GetHeight(HDC hDC = NULL) const throw()
	{
		HDC hDC1 = (hDC != NULL) ? hDC : ::GetDC(NULL);
		// For MM_TEXT mapping mode
		LONG lPointSize = ::MulDiv(-lfHeight, 72, ::GetDeviceCaps(hDC1, LOGPIXELSY));
		if( hDC == NULL )
			::ReleaseDC(NULL, hDC1);
		return lPointSize;
	}

	LONG GetDeciPointHeight(HDC hDC = NULL) const throw()
	{
		HDC hDC1 = (hDC != NULL) ? hDC : ::GetDC(NULL);
		POINT ptOrg = { 0, 0 };
		::DPtoLP(hDC1, &ptOrg, 1);
		POINT pt = { 0, 0 };
		pt.y = ::abs(lfHeight) + ptOrg.y;
		::LPtoDP(hDC1, &pt, 1);
		LONG lDeciPoint = ::MulDiv(pt.y, 720, ::GetDeviceCaps(hDC1, LOGPIXELSY));  // 72 points/inch, 10 decipoints/point
		if( hDC == NULL )
			::ReleaseDC(NULL, hDC1);
		return lDeciPoint;
	}
	void SetHeightFromDeciPoint(LONG lDeciPtHeight, HDC hDC = NULL) throw()
	{
		HDC hDC1 = (hDC != NULL) ? hDC : ::GetDC(NULL);
		POINT pt = { 0, 0 };
		pt.y = ::MulDiv(::GetDeviceCaps(hDC1, LOGPIXELSY), lDeciPtHeight, 720);  // 72 points/inch, 10 decipoints/point
		::DPtoLP(hDC1, &pt, 1);
		POINT ptOrg = { 0, 0 };
		::DPtoLP(hDC1, &ptOrg, 1);
		lfHeight = -::abs(pt.y - ptOrg.y);
		if( hDC == NULL )
			::ReleaseDC(NULL, hDC1);
	}

	void SetCaptionFont() throw()
	{
		NONCLIENTMETRICSW ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICSW);
		BOOL bRet = ::SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICSW), &ncm, 0);
		assert( bRet );
		Copy(&ncm.lfCaptionFont);
	}
	void SetMenuFont() throw()
	{
		NONCLIENTMETRICSW ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICSW);
		BOOL bRet = ::SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICSW), &ncm, 0);
		assert( bRet );
		Copy(&ncm.lfMenuFont);
	}
	void SetStatusFont() throw()
	{
		NONCLIENTMETRICSW ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICSW);
		BOOL bRet = ::SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICSW), &ncm, 0);
		assert( bRet );
		Copy(&ncm.lfStatusFont);
	}
	void SetMessageBoxFont() throw()
	{
		NONCLIENTMETRICSW ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICSW);
		BOOL bRet = ::SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICSW), &ncm, 0);
		assert( bRet );
		Copy(&ncm.lfMessageFont);
	}

	void Copy(const LOGFONTW* pLogFont) throw()
	{
		assert( pLogFont != NULL );
		*(LOGFONTW*)this = *pLogFont;
	}

	_os_log_font& operator=(const _os_log_font& src) throw()
	{
		if( this != &src ) {
			Copy(&src);
		}
		return *this;
	}
	_os_log_font& operator=(const LOGFONTW& src) throw()
	{
		Copy(&src);
		return *this;
	}
	_os_log_font& operator=(HFONT hFont) throw()
	{
		assert( ::GetObjectType(hFont) == OBJ_FONT );
		::GetObjectW(hFont, sizeof(LOGFONTW), (LOGFONTW*)this);  //no check
		return *this;
	}

	bool operator==(const LOGFONTW& logfont) const throw()
	{
		return(logfont.lfHeight == lfHeight
			&& logfont.lfWidth == lfWidth
			&& logfont.lfEscapement == lfEscapement
			&& logfont.lfOrientation == lfOrientation
			&& logfont.lfWeight == lfWeight
			&& logfont.lfItalic == lfItalic
			&& logfont.lfUnderline == lfUnderline
			&& logfont.lfStrikeOut == lfStrikeOut
			&& logfont.lfCharSet == lfCharSet
			&& logfont.lfOutPrecision == lfOutPrecision
			&& logfont.lfClipPrecision == lfClipPrecision
			&& logfont.lfQuality == lfQuality
			&& logfont.lfPitchAndFamily == lfPitchAndFamily
			&& compare_string(logfont.lfFaceName, lfFaceName) == 0);
	}
};

class _os_font : public _os_gdi_object
{
private:
	typedef _os_gdi_object  baseClass;

public:
	explicit _os_font(HFONT hFont = NULL) throw() : baseClass(hFont)
	{
	}
	_os_font(_os_font&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_os_font() throw()
	{
	}

	_os_font& operator=(_os_font&& src) throw()
	{
		return static_cast<_os_font&>(baseClass::operator=(rv_forward(static_cast<baseClass&>(src))));
	}

	void Attach(HFONT hFont) throw()
	{
		baseClass::Attach(hFont);
	}
	HFONT Detach() throw()
	{
		return (HFONT)baseClass::Detach();
	}
	HFONT GetHandle() const throw()
	{
		return (HFONT)baseClass::GetHandle();
	}

// Create methods
	bool Create(const LOGFONTW* lpLogFont) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateFontIndirectW(lpLogFont);
		return m_hObj != NULL;
	}
	bool Create(const ENUMLOGFONTEXDVW* penumlfex) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateFontIndirectExW(penumlfex);
		return m_hObj != NULL;
	}
	bool Create(int iHeight, int iWidth, int iEscapement,
			int iOrientation, int iWeight, BYTE bItalic, BYTE bUnderline,
			BYTE btStrikeOut, BYTE btCharSet, BYTE btOutPrecision,
			BYTE btClipPrecision, BYTE btQuality, BYTE btPitchAndFamily,
			LPCWSTR lpszFacename) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateFontW(iHeight, iWidth, iEscapement,
							iOrientation, iWeight, bItalic, bUnderline, btStrikeOut,
							btCharSet, btOutPrecision, btClipPrecision, btQuality,
							btPitchAndFamily, lpszFacename);
		return m_hObj != NULL;
	}
	bool CreatePointFont(int iPointSize, LPCWSTR lpszFaceName, HDC hDC = NULL, bool bBold = false, bool bItalic = false) throw()
	{
		LOGFONTW logFont = { 0 };
		logFont.lfCharSet = DEFAULT_CHARSET;
		logFont.lfHeight = iPointSize;
		copy_string(lpszFacename, _countof(logFont.lfFaceName), logFont.lfFaceName, _countof(logFont.lfFaceName));
		if( bBold )
			logFont.lfWeight = FW_BOLD;
		if( bItalic )
			logFont.lfItalic = (BYTE)TRUE;
		return CreatePointFont(&logFont, hDC);
	}
	bool CreatePointFont(const LOGFONTW* lpLogFont, HDC hDC = NULL) throw()
	{
		HDC hDC1 = (hDC != NULL) ? hDC : ::GetDC(NULL);
		// convert iPointSize to logical units based on hDC
		LOGFONTW logFont = *lpLogFont;
		POINT pt = { 0, 0 };
		pt.y = ::MulDiv(::GetDeviceCaps(hDC1, LOGPIXELSY), logFont.lfHeight, 720);  // 72 points/inch, 10 decipoints/point
		::DPtoLP(hDC1, &pt, 1);
		POINT ptOrg = { 0, 0 };
		::DPtoLP(hDC1, &ptOrg, 1);
		logFont.lfHeight = -::abs(pt.y - ptOrg.y);
		if( hDC == NULL )
			::ReleaseDC(NULL, hDC1);
		return Create(&logFont);
	}

// Attributes
	bool GetLogFont(LOGFONTW& LogFont) const throw()
	{
		assert( !IsNull() );
		return (::GetObjectW(m_hObj, sizeof(LOGFONTW), &LogFont) == sizeof(LOGFONTW));
	}

private:
	//noncopyable
	_os_font(const _os_font& src) throw();
	_os_font& operator=(const _os_font& src) throw();
};

////////////////////////////////////////////////////////////////////////////////
// _os_bitmap

class _os_bitmap : public _os_gdi_object
{
private:
	typedef _os_gdi_object  baseClass;

public:
	explicit _os_bitmap(HBITMAP hBitmap = NULL) throw() : baseClass(hBitmap)
	{
	}
	_os_bitmap(_os_bitmap&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_os_bitmap() throw()
	{
	}

	_os_bitmap& operator=(_os_bitmap&& src) throw()
	{
		return static_cast<_os_bitmap&>(baseClass::operator=(rv_forward(static_cast<baseClass&>(src))));
	}

	void Attach(HBITMAP hBitmap) throw()
	{
		baseClass::Attach(hBitmap);
	}
	HBITMAP Detach() throw()
	{
		return (HBITMAP)baseClass::Detach();
	}
	HBITMAP GetHandle() const throw()
	{
		return (HBITMAP)baseClass::GetHandle();
	}

// Create and load methods
	bool LoadBitmap(_os_u_string_or_id bitmap) throw()
	{
		assert( IsNull() );
		m_hObj = ::LoadBitmapW(_os_g_module.GetResourceInstance(), bitmap.m_lpstr);
		return m_hObj != NULL;
	}
	bool LoadOEMBitmap(UINT uIDBitmap) throw()  // for OBM_/OCR_/OIC_
	{
		assert( IsNull() );
		m_hObj = ::LoadBitmapW(NULL, MAKEINTRESOURCE(uIDBitmap));
		return m_hObj != NULL;
	}
	bool LoadMappedBitmap(UINT uIDBitmap, UINT uFlags = 0, LPCOLORMAP lpColorMap = NULL, int iMapSize = 0) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateMappedBitmap(_os_g_module.GetResourceInstance(), uIDBitmap, (WORD)uFlags, lpColorMap, iMapSize);
		return m_hObj != NULL;
	}
	bool Create(int iWidth, int iHeight, UINT uPlanes, UINT uBitsPerPixel, const void* lpBits) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateBitmap(iWidth, iHeight, uPlanes, uBitsPerPixel, lpBits);
		return m_hObj != NULL;
	}
	bool Create(const BITMAP* lpBitmap) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateBitmapIndirect(lpBitmap);
		return m_hObj != NULL;
	}
	bool CreateCompatibleBitmap(HDC hDC, int iWidth, int iHeight) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateCompatibleBitmap(hDC, iWidth, iHeight);
		return m_hObj != NULL;
	}
	bool CreateDiscardableBitmap(HDC hDC, int iWidth, int iHeight) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateDiscardableBitmap(hDC, iWidth, iHeight);
		return m_hObj != NULL;
	}

// Attributes
	bool GetBitmap(BITMAP& bm) const throw()
	{
		assert( !IsNull() );
		return (::GetObjectW(m_hObj, sizeof(BITMAP), &bm) == sizeof(BITMAP));
	}
	bool GetSize(SIZE& size) const throw()
	{
		assert( !IsNull() );
		BITMAP bm = { 0 };
		if( !GetBitmap(&bm) )
			return false;
		size.cx = bm.bmWidth;
		size.cy = bm.bmHeight;
		return true;
	}
	//do not use
	DWORD GetBitmapBits(DWORD dwCount, LPVOID lpBits) const throw()
	{
		assert( !IsNull() );
		return ::GetBitmapBits(m_hObj, dwCount, lpBits);
	}
	DWORD SetBitmapBits(DWORD dwCount, const void* lpBits) throw()
	{
		assert( !IsNull() );
		return ::SetBitmapBits(m_hObj, dwCount, lpBits);
	}

	BOOL GetBitmapDimension(LPSIZE lpSize) const throw()
	{
		assert( !IsNull() );
		return ::GetBitmapDimensionEx(m_hObj, lpSize);
	}
	BOOL SetBitmapDimension(int iWidth, int iHeight, LPSIZE lpSize = NULL) throw()
	{
		assert( !IsNull() );
		return ::SetBitmapDimensionEx(m_hObj, iWidth, iHeight, lpSize);
	}

// DIB support
	bool CreateDIBitmap(HDC hDC, const BITMAPINFOHEADER* lpbmih, DWORD dwInit, const VOID* lpbInit, const BITMAPINFO* lpbmi, UINT uColorUse) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateDIBitmap(hDC, lpbmih, dwInit, lpbInit, lpbmi, uColorUse);
		return m_hObj != NULL;
	}
	bool CreateDIBSection(HDC hDC, const BITMAPINFO* lpbmi, UINT uColorUse, VOID** ppvBits, HANDLE hSection, DWORD dwOffset) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateDIBSection(hDC, lpbmi, uColorUse, ppvBits, hSection, dwOffset);
		return m_hObj != NULL;
	}
	int GetDIBits(HDC hDC, UINT uStartScan, UINT cScanLines,  LPVOID lpvBits, LPBITMAPINFO lpbmi, UINT uColorUse) const throw()
	{
		assert( !IsNull() );
		return ::GetDIBits(hDC, m_hObj, uStartScan, cScanLines,  lpvBits, lpbmi, uColorUse);
	}
	int SetDIBits(HDC hDC, UINT uStartScan, UINT cScanLines, const VOID* lpvBits, const BITMAPINFO* lpbmi, UINT uColorUse) throw()
	{
		assert( !IsNull() );
		return ::SetDIBits(hDC, m_hObj, uStartScan, cScanLines, lpvBits, lpbmi, uColorUse);
	}

private:
	//noncopyable
	_os_bitmap(const _os_bitmap& src) throw();
	_os_bitmap& operator=(const _os_bitmap& src) throw();
};

////////////////////////////////////////////////////////////////////////////////
// _os_palette

class _os_palette : public _os_gdi_object
{
private:
	typedef _os_gdi_object  baseClass;

public:
	explicit _os_palette(HPALETTE hPalette = NULL) throw() : baseClass(hPalette)
	{
	}
	_os_palette(_os_palette&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_os_palette() throw()
	{
	}

	_os_palette& operator=(_os_palette&& src) throw()
	{
		return static_cast<_os_palette&>(baseClass::operator=(rv_forward(static_cast<baseClass&>(src))));
	}

	void Attach(HPALETTE hPalette) throw()
	{
		baseClass::Attach(hPalette);
	}
	HPALETTE Detach() throw()
	{
		return (HPALETTE)baseClass::Detach();
	}
	HPALETTE GetHandle() const throw()
	{
		return (HPALETTE)baseClass::GetHandle();
	}

// Create methods
	bool Create(const LOGPALETTE* lpLogPalette) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreatePalette(lpLogPalette);
		return m_hObj != NULL;
	}
	bool CreateHalftonePalette(HDC hDC) throw()
	{
		assert( IsNull() );
		assert( hDC != NULL );
		m_hObj = ::CreateHalftonePalette(hDC);
		return m_hObj != NULL;
	}

// Attributes
	int GetEntryCount() const throw()
	{
		assert( !IsNull() );
		WORD wEntries = 0;
		::GetObjectW(m_hObj, sizeof(WORD), &wEntries);  //no check
		return (int)wEntries;
	}
	UINT GetPaletteEntries(UINT uStartIndex, UINT uNumEntries, LPPALETTEENTRY lpPaletteColors) const throw()
	{
		assert( !IsNull() );
		return ::GetPaletteEntries(m_hObj, uStartIndex, uNumEntries, lpPaletteColors);
	}
	UINT SetPaletteEntries(UINT uStartIndex, UINT uNumEntries, const PALETTEENTRY* lpPaletteColors) throw()
	{
		assert( !IsNull() );
		return ::SetPaletteEntries(m_hObj, uStartIndex, uNumEntries, lpPaletteColors);
	}

// Operations
	BOOL AnimatePalette(UINT uStartIndex, UINT uNumEntries, const PALETTEENTRY* lpPaletteColors) throw()
	{
		assert( !IsNull() );
		return ::AnimatePalette(m_hObj, uStartIndex, uNumEntries, lpPaletteColors);
	}
	BOOL ResizePalette(UINT uNumEntries) throw()
	{
		assert( !IsNull() );
		return ::ResizePalette(m_hObj, uNumEntries);
	}
	UINT GetNearestPaletteIndex(COLORREF crColor) const throw()
	{
		assert( !IsNull() );
		return ::GetNearestPaletteIndex(m_hObj, crColor);
	}

private:
	//noncopyable
	_os_palette(const _os_palette& src) throw();
	_os_palette& operator=(const _os_palette& src) throw();
};

////////////////////////////////////////////////////////////////////////////////
// _os_rgn

class _os_rgn : public _os_gdi_object
{
private:
	typedef _os_gdi_object  baseClass;

public:
	explicit _os_rgn(HRGN hRgn = NULL) throw() : baseClass(hRgn)
	{
	}
	_os_rgn(_os_rgn&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_os_rgn() throw()
	{
	}

	_os_rgn& operator=(_os_rgn&& src) throw()
	{
		return static_cast<_os_rgn&>(baseClass::operator=(rv_forward(static_cast<baseClass&>(src))));
	}

	void Attach(HRGN hRgn) throw()
	{
		baseClass::Attach(hRgn);
	}
	HRGN Detach() throw()
	{
		return (HRGN)baseClass::Detach();
	}
	HRGN GetHandle() const throw()
	{
		return (HRGN)baseClass::GetHandle();
	}

// Create methods
	bool CreateRectRgn(int x1, int y1, int x2, int y2) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateRectRgn(x1, y1, x2, y2);
		return m_hObj != NULL;
	}
	bool CreateRectRgn(const RECT* lpRect) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateRectRgnIndirect(lpRect);
		return m_hObj != NULL;
	}
	bool CreateEllipticRgn(int x1, int y1, int x2, int y2) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateEllipticRgn(x1, y1, x2, y2);
		return m_hObj != NULL;
	}
	bool CreateEllipticRgn(const RECT* lpRect) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateEllipticRgnIndirect(lpRect);
		return m_hObj != NULL;
	}
	bool CreatePolygonRgn(const POINT* lpPoints, int iCount, int iMode) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreatePolygonRgn(lpPoints, iCount, iMode);
		return m_hObj != NULL;
	}
	bool CreatePolyPolygonRgn(const POINT* lpPoints, const INT* lpPolyCounts, int iCount, int iPolyFillMode) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreatePolyPolygonRgn(lpPoints, lpPolyCounts, iCount, iPolyFillMode);
		return m_hObj != NULL;
	}
	bool CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3) throw()
	{
		assert( IsNull() );
		m_hObj = ::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3);
		return m_hObj != NULL;
	}
	bool CreateFromPath(HDC hDC) throw()
	{
		assert( IsNull() );
		assert( hDC != NULL );
		m_hObj = ::PathToRegion(hDC);
		return m_hObj != NULL;
	}
	bool CreateFromData(const XFORM* lpXForm, DWORD dwCount, const RGNDATA* pRgnData) throw()
	{
		assert( IsNull() );
		m_hObj = ::ExtCreateRegion(lpXForm, dwCount, pRgnData);
		return m_hObj != NULL;
	}

// Operations
	BOOL SetRectRgn(int x1, int y1, int x2, int y2) throw()
	{
		assert( !IsNull() );
		return ::SetRectRgn(m_hObj, x1, y1, x2, y2);
	}
	BOOL SetRectRgn(const RECT* lpRect) throw()
	{
		assert( !IsNull() );
		return ::SetRectRgn(m_hObj, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
	}
	int CombineRgn(HRGN hRgnSrc1, HRGN hRgnSrc2, int iCombineMode) throw()
	{
		assert( !IsNull() );
		return ::CombineRgn(m_hObj, hRgnSrc1, hRgnSrc2, iCombineMode);
	}
	int CombineRgn(HRGN hRgnSrc, int iCombineMode) throw()
	{
		assert( !IsNull() );
		return ::CombineRgn(m_hObj, m_hObj, hRgnSrc, iCombineMode);
	}
	int CopyRgn(HRGN hRgnSrc) throw()
	{
		assert( !IsNull() );
		return ::CombineRgn(m_hObj, hRgnSrc, NULL, RGN_COPY);
	}

	BOOL EqualRgn(HRGN hRgn) const throw()
	{
		assert( !IsNull() );
		return ::EqualRgn(m_hObj, hRgn);
	}
	int OffsetRgn(int x, int y) throw()
	{
		assert( !IsNull() );
		return ::OffsetRgn(m_hObj, x, y);
	}
	int OffsetRgn(const POINT& point) throw()
	{
		assert( !IsNull() );
		return ::OffsetRgn(m_hObj, point.x, point.y);
	}

	int GetRgnBox(LPRECT lpRect) const throw()
	{
		assert( !IsNull() );
		return ::GetRgnBox(m_hObj, lpRect);
	}
	BOOL PtInRegion(int x, int y) const throw()
	{
		assert( !IsNull() );
		return ::PtInRegion(m_hObj, x, y);
	}
	BOOL PtInRegion(const POINT& point) const throw()
	{
		assert( !IsNull() );
		return ::PtInRegion(m_hObj, point.x, point.y);
	}
	BOOL RectInRegion(const RECT* lpRect) const throw()
	{
		assert( !IsNull() );
		return ::RectInRegion(m_hObj, lpRect);
	}
	DWORD GetRegionData(LPRGNDATA lpRgnData, DWORD dwDataSize) const throw()
	{
		assert( !IsNull() );
		return ::GetRegionData(m_hObj, dwDataSize, lpRgnData);
	}

private:
	//noncopyable
	_os_rgn(const _os_rgn& src) throw();
	_os_rgn& operator=(const _os_rgn& src) throw();
};

////////////////////////////////////////////////////////////////////////////////
// _os_dc - The device context class

class _os_dc
{
public:
	explicit _os_dc(HDC hDC = NULL) throw() : m_hDC(hDC)
	{
	}
	_os_dc(_os_dc&& src) throw()
	{
		m_hDC = src.m_hDC;
		src.m_hDC = NULL;
	}
	~_os_dc() throw()
	{
		Destroy();
	}

	void Destroy() throw()
	{
		if( m_hDC != NULL ) {
			BOOL bRet = ::DeleteDC(m_hDC);
			assert( bRet );
			m_hDC = NULL;
		}
	}

	_os_dc& operator=(_os_dc&& src) throw()
	{
		if( this != &src ) {
			if( m_hDC != src.m_hDC ) {
				Destroy();
				m_hDC = src.m_hDC;
				src.m_hDC = NULL;
			}
		}
		return *this;
	}

	void Attach(HDC hDC) throw()
	{
		if( m_hDC != hDC )
			Destroy();
		m_hDC = hDC;
	}
	HDC Detach() throw()
	{
		HDC hDC = m_hDC;
		m_hDC = NULL;
		return hDC;
	}

	HDC GetHandle() const throw()
	{
		return m_hDC;
	}
	bool IsNull() const throw()
	{
		return (m_hDC == NULL);
	}

// Operations
	HWND WindowFromDC() const throw()
	{
		assert( !IsNull() );
		return ::WindowFromDC(m_hDC);
	}

	weak_object_ref<_os_pen> GetCurrentPen() const throw()
	{
		assert( !IsNull() );
		return weak_object_ref<_os_pen>(_os_pen((HPEN)::GetCurrentObject(m_hDC, OBJ_PEN)));
	}
	weak_object_ref<_os_brush> GetCurrentBrush() const throw()
	{
		assert( !IsNull() );
		return weak_object_ref<_os_brush>(_os_brush((HBRUSH)::GetCurrentObject(m_hDC, OBJ_BRUSH)));
	}
	weak_object_ref<_os_palette> GetCurrentPalette() const throw()
	{
		assert( !IsNull() );
		return weak_object_ref<_os_palette>(_os_palette((HPALETTE)::GetCurrentObject(m_hDC, OBJ_PAL)));
	}
	weak_object_ref<_os_font> GetCurrentFont() const throw()
	{
		assert( !IsNull() );
		return weak_object_ref<_os_font>(_os_font((HFONT)::GetCurrentObject(m_hDC, OBJ_FONT)));
	}
	weak_object_ref<_os_bitmap> GetCurrentBitmap() const throw()
	{
		assert( !IsNull() );
		return weak_object_ref<_os_bitmap>(_os_bitmap((HBITMAP)::GetCurrentObject(m_hDC, OBJ_BITMAP)));
	}

	bool CreateDC(LPCWSTR lpszDriverName, LPCWSTR lpszDeviceName, LPCWSTR lpszOutput, const DEVMODEW* lpInitData) throw()
	{
		assert( IsNull() );
		m_hDC = ::CreateDCW(lpszDriverName, lpszDeviceName, lpszOutput, lpInitData);
		return m_hDC != NULL;
	}
	bool CreateCompatibleDC(HDC hDC = NULL) throw()
	{
		assert( IsNull() );
		m_hDC = ::CreateCompatibleDC(hDC);
		return m_hDC != NULL;
	}

// Device-Context Functions
	int SaveDC() throw()
	{
		assert( !IsNull() );
		return ::SaveDC(m_hDC);
	}
	BOOL RestoreDC(int nSavedDC) throw()
	{
		assert( !IsNull() );
		return ::RestoreDC(m_hDC, nSavedDC);
	}
	int GetDeviceCaps(int nIndex) const throw()
	{
		assert( !IsNull() );
		return ::GetDeviceCaps(m_hDC, nIndex);
	}
	UINT SetBoundsRect(const RECT* lpRectBounds, UINT flags) throw()
	{
		assert( !IsNull() );
		return ::SetBoundsRect(m_hDC, lpRectBounds, flags);
	}
	UINT GetBoundsRect(LPRECT lpRectBounds, UINT flags) const throw()
	{
		assert( !IsNull() );
		return ::GetBoundsRect(m_hDC, lpRectBounds, flags);
	}
	BOOL ResetDC(const DEVMODEW* lpDevMode) throw()
	{
		assert( !IsNull() );
		return ::ResetDCW(m_hDC, lpDevMode) != NULL;
	}

// Drawing-Tool Functions
	BOOL GetBrushOrg(LPPOINT lpPoint) const throw()
	{
		assert( !IsNull() );
		return ::GetBrushOrgEx(m_hDC, lpPoint);
	}
	BOOL SetBrushOrg(int x, int y, LPPOINT lpPoint = NULL) throw()
	{
		assert( !IsNull() );
		return ::SetBrushOrgEx(m_hDC, x, y, lpPoint);
	}
	BOOL SetBrushOrg(const POINT& point, LPPOINT lpPointRet = NULL) throw()
	{
		assert( !IsNull() );
		return ::SetBrushOrgEx(m_hDC, point.x, point.y, lpPointRet);
	}
	int EnumObjects(int nObjectType, int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData) throw()
	{
		assert( !IsNull() );
		return ::EnumObjects(m_hDC, nObjectType, (GOBJENUMPROC)lpfn, (LPVOID)lpData);
	}

// Type-safe selection helpers
	HPEN SelectPen(HPEN hPen) throw()
	{
		assert( !IsNull() );
		assert( hPen == NULL || ::GetObjectType(hPen) == OBJ_PEN || ::GetObjectType(hPen) == OBJ_EXTPEN );
		return (HPEN)::SelectObject(m_hDC, hPen);
	}
	HBRUSH SelectBrush(HBRUSH hBrush) throw()
	{
		assert( !IsNull() );
		assert( hBrush == NULL || ::GetObjectType(hBrush) == OBJ_BRUSH );
		return (HBRUSH)::SelectObject(m_hDC, hBrush);
	}
	HFONT SelectFont(HFONT hFont) throw()
	{
		assert( !IsNull() );
		assert( hFont == NULL || ::GetObjectType(hFont) == OBJ_FONT);
		return (HFONT)::SelectObject(m_hDC, hFont);
	}
	HBITMAP SelectBitmap(HBITMAP hBitmap) throw()
	{
		assert( !IsNull() );
		assert( hBitmap == NULL || ::GetObjectType(hBitmap) == OBJ_BITMAP );
		return (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}
	// special return for regions
	int SelectRgn(HRGN hRgn) throw()
	{
		assert( !IsNull() );
		assert( hRgn == NULL || ::GetObjectType(hRgn) == OBJ_REGION );
		return PtrToInt(::SelectObject(m_hDC, hRgn));
	}

// Type-safe selection helpers for stock objects
	HPEN SelectStockPen(int iPen) throw()
	{
		assert( iPen == WHITE_PEN || iPen == BLACK_PEN || iPen == NULL_PEN || iPen == DC_PEN );
		return SelectPen((HPEN)::GetStockObject(iPen));
	}
	HBRUSH SelectStockBrush(int iBrush) throw()
	{
		assert( (iBrush >= WHITE_BRUSH && iBrush <= HOLLOW_BRUSH) || iBrush == DC_BRUSH );
		return SelectBrush((HBRUSH)::GetStockObject(iBrush));
	}
	HFONT SelectStockFont(int iFont) throw()
	{
		assert( (iFont >= OEM_FIXED_FONT && iFont <= SYSTEM_FIXED_FONT) || iFont == DEFAULT_GUI_FONT );
		return SelectFont((HFONT)::GetStockObject(iFont));
	}
	HPALETTE SelectStockPalette(int iPalette, BOOL bForceBackground) throw()
	{
		assert( iPalette == DEFAULT_PALETTE ); // the only one supported
		return SelectPalette((HPALETTE)::GetStockObject(iPalette), bForceBackground);
	}

// Color and Color Palette Functions
	COLORREF GetNearestColor(COLORREF crColor) const throw()
	{
		assert( !IsNull() );
		return ::GetNearestColor(m_hDC, crColor);
	}
	HPALETTE SelectPalette(HPALETTE hPalette, BOOL bForceBackground) throw()
	{
		assert( !IsNull() );
		return ::SelectPalette(m_hDC, hPalette, bForceBackground);
	}
	UINT RealizePalette() throw()
	{
		assert( !IsNull() );
		return ::RealizePalette(m_hDC);
	}
	BOOL UpdateColors() throw()
	{
		assert( !IsNull() );
		return ::UpdateColors(m_hDC);
	}

// Drawing-Attribute Functions
	COLORREF GetBkColor() const throw()
	{
		assert( !IsNull() );
		return ::GetBkColor(m_hDC);
	}
	int GetBkMode() const throw()
	{
		assert( !IsNull() );
		return ::GetBkMode(m_hDC);
	}
	int GetPolyFillMode() const throw()
	{
		assert( !IsNull() );
		return ::GetPolyFillMode(m_hDC);
	}
	int GetROP2() const throw()
	{
		assert( !IsNull() );
		return ::GetROP2(m_hDC);
	}
	int GetStretchBltMode() const throw()
	{
		assert( !IsNull() );
		return ::GetStretchBltMode(m_hDC);
	}
	COLORREF GetTextColor() const throw()
	{
		assert( !IsNull() );
		return ::GetTextColor(m_hDC);
	}

	COLORREF SetBkColor(COLORREF crColor) throw()
	{
		assert( !IsNull() );
		return ::SetBkColor(m_hDC, crColor);
	}
	int SetBkMode(int iBkMode) throw()
	{
		assert( !IsNull() );
		return ::SetBkMode(m_hDC, iBkMode);
	}
	int SetPolyFillMode(int iPolyFillMode) throw()
	{
		assert( !IsNull() );
		return ::SetPolyFillMode(m_hDC, iPolyFillMode);
	}
	int SetROP2(int iDrawMode) throw()
	{
		assert( !IsNull() );
		return ::SetROP2(m_hDC, iDrawMode);
	}
	int SetStretchBltMode(int iStretchMode) throw()
	{
		assert( !IsNull() );
		return ::SetStretchBltMode(m_hDC, iStretchMode);
	}
	COLORREF SetTextColor(COLORREF crColor) throw()
	{
		assert( !IsNull() );
		return ::SetTextColor(m_hDC, crColor);
	}

	BOOL GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const throw()
	{
		assert( !IsNull() );
		return ::GetColorAdjustment(m_hDC, lpColorAdjust);
	}
	BOOL SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust) throw()
	{
		assert( !IsNull() );
		return ::SetColorAdjustment(m_hDC, lpColorAdjust);
	}

// Mapping Functions
	int GetMapMode() const throw()
	{
		assert( !IsNull() );
		return ::GetMapMode(m_hDC);
	}
	BOOL GetViewportOrg(LPPOINT lpPoint) const throw()
	{
		assert( !IsNull() );
		return ::GetViewportOrgEx(m_hDC, lpPoint);
	}
	int SetMapMode(int nMapMode) throw()
	{
		assert( !IsNull() );
		return ::SetMapMode(m_hDC, nMapMode);
	}
	// Viewport Origin
	BOOL SetViewportOrg(int x, int y, LPPOINT lpPoint = NULL) throw()
	{
		assert( !IsNull() );
		return ::SetViewportOrgEx(m_hDC, x, y, lpPoint);
	}
	BOOL SetViewportOrg(const POINT& point, LPPOINT lpPointRet = NULL) throw()
	{
		return SetViewportOrg(point.x, point.y, lpPointRet);
	}
	BOOL OffsetViewportOrg(int iWidth, int iHeight, LPPOINT lpPoint = NULL) throw()
	{
		assert( !IsNull() );
		return ::OffsetViewportOrgEx(m_hDC, iWidth, iHeight, lpPoint);
	}
	// Viewport Extent
	BOOL GetViewportExt(LPSIZE lpSize) const throw()
	{
		assert( !IsNull() );
		return ::GetViewportExtEx(m_hDC, lpSize);
	}
	BOOL SetViewportExt(int x, int y, LPSIZE lpSize = NULL) throw()
	{
		assert( !IsNull() );
		return ::SetViewportExtEx(m_hDC, x, y, lpSize);
	}
	BOOL SetViewportExt(const SIZE& size, LPSIZE lpSizeRet = NULL) throw()
	{
		return SetViewportExt(size.cx, size.cy, lpSizeRet);
	}
	BOOL ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize = NULL) throw()
	{
		assert( !IsNull() );
		return ::ScaleViewportExtEx(m_hDC, xNum, xDenom, yNum, yDenom, lpSize);
	}
	// Window Origin
	BOOL GetWindowOrg(LPPOINT lpPoint) const throw()
	{
		assert( !IsNull() );
		return ::GetWindowOrgEx(m_hDC, lpPoint);
	}
	BOOL SetWindowOrg(int x, int y, LPPOINT lpPoint = NULL) throw()
	{
		assert( !IsNull() );
		return ::SetWindowOrgEx(m_hDC, x, y, lpPoint);
	}
	BOOL SetWindowOrg(const POINT& point, LPPOINT lpPointRet = NULL) throw()
	{
		return SetWindowOrg(point.x, point.y, lpPointRet);
	}
	BOOL OffsetWindowOrg(int iWidth, int iHeight, LPPOINT lpPoint = NULL) throw()
	{
		assert( !IsNull() );
		return ::OffsetWindowOrgEx(m_hDC, iWidth, iHeight, lpPoint);
	}
	// Window extent
	BOOL GetWindowExt(LPSIZE lpSize) const throw()
	{
		assert( !IsNull() );
		return ::GetWindowExtEx(m_hDC, lpSize);
	}
	BOOL SetWindowExt(int x, int y, LPSIZE lpSize = NULL) throw()
	{
		assert( !IsNull() );
		return ::SetWindowExtEx(m_hDC, x, y, lpSize);
	}
	BOOL SetWindowExt(const SIZE& size, LPSIZE lpSizeRet = NULL) throw()
	{
		return SetWindowExt(size.cx, size.cy, lpSizeRet);
	}
	BOOL ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize = NULL) throw()
	{
		assert( !IsNull() );
		return ::ScaleWindowExtEx(m_hDC, xNum, xDenom, yNum, yDenom, lpSize);
	}

// Coordinate Functions
	BOOL DPtoLP(LPPOINT lpPoints, int nCount = 1) const throw()
	{
		assert( !IsNull() );
		return ::DPtoLP(m_hDC, lpPoints, nCount);
	}
	BOOL DPtoLP(LPRECT lpRect) const throw()
	{
		assert( !IsNull() );
		return ::DPtoLP(m_hDC, (LPPOINT)lpRect, 2);
	}
	BOOL DPtoLP(LPSIZE lpSize) const throw()
	{
		SIZE sizeWinExt = { 0, 0 };
		if( !GetWindowExt(&sizeWinExt) )
			return FALSE;
		SIZE sizeVpExt = { 0, 0 };
		if( !GetViewportExt(&sizeVpExt) )
			return FALSE;
		lpSize->cx = ::MulDiv(lpSize->cx, ::abs(sizeWinExt.cx), ::abs(sizeVpExt.cx));
		lpSize->cy = ::MulDiv(lpSize->cy, ::abs(sizeWinExt.cy), ::abs(sizeVpExt.cy));
		return TRUE;
	}
	BOOL LPtoDP(LPPOINT lpPoints, int nCount = 1) const throw()
	{
		assert( !IsNull() );
		return ::LPtoDP(m_hDC, lpPoints, nCount);
	}
	BOOL LPtoDP(LPRECT lpRect) const throw()
	{
		assert( !IsNull() );
		return ::LPtoDP(m_hDC, (LPPOINT)lpRect, 2);
	}
	BOOL LPtoDP(LPSIZE lpSize) const throw()
	{
		SIZE sizeWinExt = { 0, 0 };
		if( !GetWindowExt(&sizeWinExt) )
			return FALSE;
		SIZE sizeVpExt = { 0, 0 };
		if( !GetViewportExt(&sizeVpExt) )
			return FALSE;
		lpSize->cx = ::MulDiv(lpSize->cx, ::abs(sizeVpExt.cx), ::abs(sizeWinExt.cx));
		lpSize->cy = ::MulDiv(lpSize->cy, ::abs(sizeVpExt.cy), ::abs(sizeWinExt.cy));
		return TRUE;
	}

// Special Coordinate Functions (useful for dealing with metafiles and OLE)
	enum { HIMETRIC_INCH = 2540 };  // HIMETRIC units per inch

	void DPtoHIMETRIC(LPSIZE lpSize) const throw()
	{
		assert( !IsNull() );
		int iMapMode;
		if( (iMapMode = GetMapMode()) < MM_ISOTROPIC && iMapMode != MM_TEXT ) {
			// when using a constrained map mode, map against physical inch
			this->SetMapMode(MM_HIMETRIC);
			DPtoLP(lpSize);
			this->SetMapMode(iMapMode);
		}
		else {
			// map against logical inch for non-constrained mapping modes
			int cxPerInch = GetDeviceCaps(LOGPIXELSX);
			int cyPerInch = GetDeviceCaps(LOGPIXELSY);
			assert( cxPerInch != 0 && cyPerInch != 0 );
			lpSize->cx = ::MulDiv(lpSize->cx, HIMETRIC_INCH, cxPerInch);
			lpSize->cy = ::MulDiv(lpSize->cy, HIMETRIC_INCH, cyPerInch);
		} //end if
	}
	void HIMETRICtoDP(LPSIZE lpSize) const throw()
	{
		assert( !IsNull() );
		int iMapMode;
		if( (iMapMode = GetMapMode()) < MM_ISOTROPIC && iMapMode != MM_TEXT ) {
			// when using a constrained map mode, map against physical inch
			this->SetMapMode(MM_HIMETRIC);
			LPtoDP(lpSize);
			this->SetMapMode(iMapMode);
		}
		else {
			// map against logical inch for non-constrained mapping modes
			int cxPerInch = GetDeviceCaps(LOGPIXELSX);
			int cyPerInch = GetDeviceCaps(LOGPIXELSY);
			assert( cxPerInch != 0 && cyPerInch != 0 );
			lpSize->cx = ::MulDiv(lpSize->cx, cxPerInch, HIMETRIC_INCH);
			lpSize->cy = ::MulDiv(lpSize->cy, cyPerInch, HIMETRIC_INCH);
		} //end if
	}
	void LPtoHIMETRIC(LPSIZE lpSize) const throw()
	{
		LPtoDP(lpSize);
		DPtoHIMETRIC(lpSize);
	}
	void HIMETRICtoLP(LPSIZE lpSize) const throw()
	{
		HIMETRICtoDP(lpSize);
		DPtoLP(lpSize);
	}

// Region Functions
	BOOL FillRgn(HRGN hRgn, HBRUSH hBrush) throw()
	{
		assert( !IsNull() );
		return ::FillRgn(m_hDC, hRgn, hBrush);
	}
	BOOL FrameRgn(HRGN hRgn, HBRUSH hBrush, int iWidth, int iHeight) throw()
	{
		assert( !IsNull() );
		return ::FrameRgn(m_hDC, hRgn, hBrush, iWidth, iHeight);
	}
	BOOL InvertRgn(HRGN hRgn) throw()
	{
		assert( !IsNull() );
		return ::InvertRgn(m_hDC, hRgn);
	}
	BOOL PaintRgn(HRGN hRgn) throw()
	{
		assert( !IsNull() );
		return ::PaintRgn(m_hDC, hRgn);
	}

// Clipping Functions
	int GetClipBox(LPRECT lpRect) const throw()
	{
		assert( !IsNull() );
		return ::GetClipBox(m_hDC, lpRect);
	}
	int GetClipRgn(_os_rgn& region) const throw()
	{
		assert( !IsNull() );
		if( region.IsNull() )
			region.CreateRectRgn(0, 0, 0, 0);
		int iRet = ::GetClipRgn(m_hDC, region.GetHandle());
		if( iRet != 1 )
			region.Destroy();
		return iRet;
	}
	BOOL PtVisible(int x, int y) const throw()
	{
		assert( !IsNull() );
		return ::PtVisible(m_hDC, x, y);
	}
	BOOL PtVisible(const POINT& point) const throw()
	{
		assert( !IsNull() );
		return ::PtVisible(m_hDC, point.x, point.y);
	}
	BOOL RectVisible(const RECT* lpRect) const throw()
	{
		assert( !IsNull() );
		return ::RectVisible(m_hDC, lpRect);
	}
	int SelectClipRgn(HRGN hRgn) throw()
	{
		assert( !IsNull() );
		return ::SelectClipRgn(m_hDC, hRgn);
	}
	int ExcludeClipRect(int x1, int y1, int x2, int y2) throw()
	{
		assert( !IsNull() );
		return ::ExcludeClipRect(m_hDC, x1, y1, x2, y2);
	}
	int ExcludeClipRect(const RECT* lpRect) throw()
	{
		assert( !IsNull() );
		return ::ExcludeClipRect(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
	}
	int ExcludeUpdateRgn(HWND hWnd) throw()
	{
		assert( !IsNull() );
		return ::ExcludeUpdateRgn(m_hDC, hWnd);
	}
	int IntersectClipRect(int x1, int y1, int x2, int y2) throw()
	{
		assert( !IsNull() );
		return ::IntersectClipRect(m_hDC, x1, y1, x2, y2);
	}
	int IntersectClipRect(const RECT* lpRect) throw()
	{
		assert( !IsNull() );
		return ::IntersectClipRect(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
	}
	int OffsetClipRgn(int x, int y) throw()
	{
		assert( !IsNull() );
		return ::OffsetClipRgn(m_hDC, x, y);
	}
	int OffsetClipRgn(const SIZE& size) throw()
	{
		assert( !IsNull() );
		return ::OffsetClipRgn(m_hDC, size.cx, size.cy);
	}
	int SelectClipRgn(HRGN hRgn, int iMode) throw()
	{
		assert( !IsNull() );
		return ::ExtSelectClipRgn(m_hDC, hRgn, iMode);
	}

// Line-Output Functions
	BOOL GetCurrentPosition(LPPOINT lpPoint) const throw()
	{
		assert( !IsNull() );
		return ::GetCurrentPositionEx(m_hDC, lpPoint);
	}
	BOOL MoveTo(int x, int y, LPPOINT lpPoint = NULL) throw()
	{
		assert( !IsNull() );
		return ::MoveToEx(m_hDC, x, y, lpPoint);
	}
	BOOL MoveTo(const POINT& point, LPPOINT lpPointRet = NULL) throw()
	{
		return MoveTo(point.x, point.y, lpPointRet);
	}
	BOOL LineTo(int x, int y) throw()
	{
		assert( !IsNull() );
		return ::LineTo(m_hDC, x, y);
	}
	BOOL LineTo(const POINT& point) throw()
	{
		return LineTo(point.x, point.y);
	}
	BOOL Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) throw()
	{
		assert( !IsNull() );
		return ::Arc(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}
	BOOL Arc(const RECT* lpRect, const POINT& ptStart, const POINT& ptEnd) throw()
	{
		assert( !IsNull() );
		return ::Arc(m_hDC, lpRect->left, lpRect->top,
			lpRect->right, lpRect->bottom, ptStart.x, ptStart.y,
			ptEnd.x, ptEnd.y);
	}
	BOOL Polyline(const POINT* lpPoints, int iCount) throw()
	{
		assert( !IsNull() );
		return ::Polyline(m_hDC, lpPoints, iCount);
	}
	BOOL AngleArc(int x, int y, DWORD dwRadius, float fStartAngle, float fSweepAngle) throw()
	{
		assert( !IsNull() );
		return ::AngleArc(m_hDC, x, y, dwRadius, fStartAngle, fSweepAngle);
	}
	BOOL ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) throw()
	{
		assert( !IsNull() );
		return ::ArcTo(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}
	BOOL ArcTo(const RECT* lpRect, const POINT& ptStart, const POINT& ptEnd) throw()
	{
		return ArcTo(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom,
					ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}
	int GetArcDirection() const throw()
	{
		assert( !IsNull() );
		return ::GetArcDirection(m_hDC);
	}
	int SetArcDirection(int iArcDirection) throw()
	{
		assert( !IsNull() );
		return ::SetArcDirection(m_hDC, iArcDirection);
	}
	BOOL PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, int iCount) throw()
	{
		assert( !IsNull() );
		return ::PolyDraw(m_hDC, lpPoints, lpTypes, iCount);
	}
	BOOL PolylineTo(const POINT* lpPoints, DWORD dwCount) throw()
	{
		assert( !IsNull() );
		return ::PolylineTo(m_hDC, lpPoints, dwCount);
	}
	BOOL PolyPolyline(const POINT* lpPoints,
		const DWORD* lpPolyPoints, DWORD dwCount) throw()
	{
		assert( !IsNull() );
		return ::PolyPolyline(m_hDC, lpPoints, lpPolyPoints, dwCount);
	}
	BOOL PolyBezier(const POINT* lpPoints, DWORD dwCount) throw()
	{
		assert( !IsNull() );
		return ::PolyBezier(m_hDC, lpPoints, dwCount);
	}
	BOOL PolyBezierTo(const POINT* lpPoints, DWORD dwCount) throw()
	{
		assert( !IsNull() );
		return ::PolyBezierTo(m_hDC, lpPoints, dwCount);
	}

// Simple Drawing Functions
	BOOL FillRect(const RECT* lpRect, HBRUSH hBrush) throw()
	{
		assert( !IsNull() );
		return ::FillRect(m_hDC, lpRect, hBrush);
	}
	BOOL FillRect(const RECT* lpRect, int iColorIndex) throw()
	{
		assert( !IsNull() );
		return ::FillRect(m_hDC, lpRect, (HBRUSH)LongToPtr(iColorIndex + 1));
	}
	BOOL FrameRect(const RECT* lpRect, HBRUSH hBrush) throw()
	{
		assert( !IsNull() );
		return ::FrameRect(m_hDC, lpRect, hBrush);
	}
	BOOL InvertRect(const RECT* lpRect) throw()
	{
		assert( !IsNull() );
		return ::InvertRect(m_hDC, lpRect);
	}
	BOOL DrawIcon(int x, int y, HICON hIcon) throw()
	{
		assert( !IsNull() );
		return ::DrawIcon(m_hDC, x, y, hIcon);
	}
	BOOL DrawIcon(const POINT& point, HICON hIcon) throw()
	{
		assert( !IsNull() );
		return ::DrawIcon(m_hDC, point.x, point.y, hIcon);
	}
	BOOL DrawIconEx(int x, int y, HICON hIcon, int cxWidth, int cyWidth, UINT uStepIfAniCur = 0, HBRUSH hbrFlickerFreeDraw = NULL, UINT uFlags = DI_NORMAL) throw()
	{
		assert( !IsNull() );
		return ::DrawIconEx(m_hDC, x, y, hIcon, cxWidth, cyWidth, uStepIfAniCur, hbrFlickerFreeDraw, uFlags);
	}
	BOOL DrawIconEx(const POINT& point, HICON hIcon, const SIZE& size, UINT uStepIfAniCur = 0, HBRUSH hbrFlickerFreeDraw = NULL, UINT uFlags = DI_NORMAL) throw()
	{
		assert( !IsNull() );
		return ::DrawIconEx(m_hDC, point.x, point.y, hIcon, size.cx, size.cy, uStepIfAniCur, hbrFlickerFreeDraw, uFlags);
	}
	BOOL DrawState(const POINT& pt, const SIZE& size, HBITMAP hBitmap, UINT nFlags, HBRUSH hBrush = NULL) throw()
	{
		assert( !IsNull() );
		return ::DrawStateW(m_hDC, hBrush, NULL, (LPARAM)hBitmap, 0, pt.x, pt.y, size.cx, size.cy, nFlags | DST_BITMAP);
	}
	BOOL DrawState(const POINT& pt, const SIZE& size, HICON hIcon, UINT nFlags, HBRUSH hBrush = NULL) throw()
	{
		assert( !IsNull() );
		return ::DrawStateW(m_hDC, hBrush, NULL, (LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags | DST_ICON);
	}
	BOOL DrawState(const POINT& pt, const SIZE& size, LPCWSTR lpszText, UINT nFlags, BOOL bPrefixText = TRUE, int nTextLen = 0, HBRUSH hBrush = NULL) throw()
	{
		assert( !IsNull() );
		return ::DrawStateW(m_hDC, hBrush, NULL, (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags | (bPrefixText ? DST_PREFIXTEXT : DST_TEXT));
	}
	BOOL DrawState(const POINT& pt, const SIZE& size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, HBRUSH hBrush = NULL) throw()
	{
		assert( !IsNull() );
		return ::DrawStateW(m_hDC, hBrush, lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags | DST_COMPLEX);
	}

// Ellipse and Polygon Functions
	BOOL Chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) throw()
	{
		assert( !IsNull() );
		return ::Chord(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}
	BOOL Chord(const RECT* lpRect, const POINT& ptStart, const POINT& ptEnd) throw()
	{
		assert( !IsNull() );
		return ::Chord(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}
	BOOL DrawFocusRect(const RECT* lpRect) throw()
	{
		assert( !IsNull() );
		return ::DrawFocusRect(m_hDC, lpRect);
	}
	BOOL Ellipse(int x1, int y1, int x2, int y2) throw()
	{
		assert( !IsNull() );
		return ::Ellipse(m_hDC, x1, y1, x2, y2);
	}
	BOOL Ellipse(const RECT* lpRect) throw()
	{
		assert( !IsNull() );
		return ::Ellipse(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
	}
	BOOL Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) throw()
	{
		assert( !IsNull() );
		return ::Pie(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}
	BOOL Pie(const RECT* lpRect, const POINT& ptStart, const POINT& ptEnd) throw()
	{
		assert( !IsNull() );
		return ::Pie(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}
	BOOL Polygon(const POINT* lpPoints, int iCount) throw()
	{
		assert( !IsNull() );
		return ::Polygon(m_hDC, lpPoints, iCount);
	}
	BOOL PolyPolygon(const POINT* lpPoints, const INT* lpPolyCounts, int iCount) throw()
	{
		assert( !IsNull() );
		return ::PolyPolygon(m_hDC, lpPoints, lpPolyCounts, iCount);
	}
	BOOL Rectangle(int x1, int y1, int x2, int y2) throw()
	{
		assert( !IsNull() );
		return ::Rectangle(m_hDC, x1, y1, x2, y2);
	}
	BOOL Rectangle(const RECT* lpRect) throw()
	{
		assert( !IsNull() );
		return ::Rectangle(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
	}
	BOOL RoundRect(int x1, int y1, int x2, int y2, int x3, int y3) throw()
	{
		assert( !IsNull() );
		return ::RoundRect(m_hDC, x1, y1, x2, y2, x3, y3);
	}
	BOOL RoundRect(const RECT* lpRect, const POINT& point) throw()
	{
		assert( !IsNull() );
		return ::RoundRect(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, point.x, point.y);
	}

// Bitmap Functions
	BOOL PatBlt(int x, int y, int iWidth, int iHeight, DWORD dwRop) throw()
	{
		assert( !IsNull() );
		return ::PatBlt(m_hDC, x, y, iWidth, iHeight, dwRop);
	}
	BOOL BitBlt(int x, int y, int iWidth, int iHeight, HDC hSrcDC,
		int xSrc, int ySrc, DWORD dwRop) throw()
	{
		assert( !IsNull() );
		return ::BitBlt(m_hDC, x, y, iWidth, iHeight, hSrcDC, xSrc, ySrc, dwRop);
	}
	BOOL StretchBlt(int x, int y, int iWidth, int iHeight, HDC hSrcDC, int xSrc, int ySrc, int iSrcWidth, int iSrcHeight, DWORD dwRop) throw()
	{
		assert( !IsNull() );
		return ::StretchBlt(m_hDC, x, y, iWidth, iHeight, hSrcDC, xSrc, ySrc, iSrcWidth, iSrcHeight, dwRop);
	}
	COLORREF GetPixel(int x, int y) const throw()
	{
		assert( !IsNull() );
		return ::GetPixel(m_hDC, x, y);
	}
	COLORREF GetPixel(const POINT& point) const throw()
	{
		assert( !IsNull() );
		return ::GetPixel(m_hDC, point.x, point.y);
	}
	COLORREF SetPixel(int x, int y, COLORREF crColor) throw()
	{
		assert( !IsNull() );
		return ::SetPixel(m_hDC, x, y, crColor);
	}
	COLORREF SetPixel(const POINT& point, COLORREF crColor) throw()
	{
		assert( !IsNull() );
		return ::SetPixel(m_hDC, point.x, point.y, crColor);
	}
	BOOL FloodFill(int x, int y, COLORREF crColor) throw()
	{
		assert( !IsNull() );
		return ::FloodFill(m_hDC, x, y, crColor);
	}
	BOOL ExtFloodFill(int x, int y, COLORREF crColor, UINT uFillType) throw()
	{
		assert( !IsNull() );
		return ::ExtFloodFill(m_hDC, x, y, crColor, uFillType);
	}
	BOOL MaskBlt(int x, int y, int iWidth, int iHeight, HDC hSrcDC, int xSrc, int ySrc, HBITMAP hMaskBitmap, int xMask, int yMask, DWORD dwRop) throw()
	{
		assert( !IsNull() );
		return ::MaskBlt(m_hDC, x, y, iWidth, iHeight, hSrcDC, xSrc, ySrc, hMaskBitmap, xMask, yMask, dwRop);
	}
	BOOL PlgBlt(const POINT* lpPoint, HDC hSrcDC, int xSrc, int ySrc, int iWidth, int iHeight, HBITMAP hMaskBitmap, int xMask, int yMask) throw()
	{
		assert( !IsNull() );
		return ::PlgBlt(m_hDC, lpPoint, hSrcDC, xSrc, ySrc, iWidth, iHeight, hMaskBitmap, xMask, yMask);
	}
	BOOL SetPixelV(int x, int y, COLORREF crColor) throw()
	{
		assert( !IsNull() );
		return ::SetPixelV(m_hDC, x, y, crColor);
	}
	BOOL SetPixelV(const POINT& point, COLORREF crColor) throw()
	{
		assert( !IsNull() );
		return ::SetPixelV(m_hDC, point.x, point.y, crColor);
	}
	BOOL TransparentBlt(int x, int y, int iWidth, int iHeight, HDC hSrcDC, int xSrc, int ySrc, int iSrcWidth, int iSrcHeight, UINT crTransparent) throw()
	{
		assert( !IsNull() );
		return ::TransparentBlt(m_hDC, x, y, iWidth, iHeight, hSrcDC, xSrc, ySrc, iSrcWidth, iSrcHeight, crTransparent);
	}
	BOOL GradientFill(const PTRIVERTEX pVertices, ULONG ulVertices, void* pMeshElements, ULONG ulMeshElements, ULONG ulMode) throw()
	{
		assert( !IsNull() );
		return ::GradientFill(m_hDC, pVertices, ulVertices, pMeshElements, ulMeshElements, ulMode);
	}
	BOOL GradientFillRect(const RECT& rect, COLORREF clr1, COLORREF clr2, bool bHorizontal) throw()
	{
		assert( !IsNull() );

		TRIVERTEX arrTvx[2] = { { 0 }, { 0 } };

		arrTvx[0].x = rect.left;
		arrTvx[0].y = rect.top;
		arrTvx[0].Red = MAKEWORD(0, GetRValue(clr1));
		arrTvx[0].Green = MAKEWORD(0, GetGValue(clr1));
		arrTvx[0].Blue = MAKEWORD(0, GetBValue(clr1));
		arrTvx[0].Alpha = 0;

		arrTvx[1].x = rect.right;
		arrTvx[1].y = rect.bottom;
		arrTvx[1].Red = MAKEWORD(0, GetRValue(clr2));
		arrTvx[1].Green = MAKEWORD(0, GetGValue(clr2));
		arrTvx[1].Blue = MAKEWORD(0, GetBValue(clr2));
		arrTvx[1].Alpha = 0;

		GRADIENT_RECT gr = { 0, 1 };

		return ::GradientFill(m_hDC, arrTvx, 2, &gr, 1, bHorizontal ? GRADIENT_FILL_RECT_H : GRADIENT_FILL_RECT_V);
	}
	BOOL AlphaBlend(int x, int y, int iWidth, int iHeight, HDC hSrcDC, int xSrc, int ySrc, int iSrcWidth, int iSrcHeight, BLENDFUNCTION bf) throw()
	{
		assert( !IsNull() );
		return ::AlphaBlend(m_hDC, x, y, iWidth, iHeight, hSrcDC, xSrc, ySrc, iSrcWidth, iSrcHeight, bf);
	}

// Extra bitmap functions
	// Helper function for painting a disabled toolbar or menu bitmap
	// This function can take either an HBITMAP (for SS) or a DC with
	//   the bitmap already painted (for cmdbar)
	BOOL DitherBlt(int x, int y, int iWidth, int iHeight, HDC hSrcDC, HBITMAP hBitmap, int xSrc, int ySrc,
			HBRUSH hBrushBackground = ::GetSysColorBrush(COLOR_3DFACE),
			HBRUSH hBrush3DEffect = ::GetSysColorBrush(COLOR_3DHILIGHT),
			HBRUSH hBrushDisabledImage = ::GetSysColorBrush(COLOR_3DSHADOW)) throw()
	{
		assert( !IsNull() );
		assert( hBitmap != NULL );
		assert( iWidth > 0 && iHeight > 0 );

		// Create a generic DC for all BitBlts
		weak_object_ref<_os_dc> dc( _os_dc((hSrcDC != NULL) ? hSrcDC : ::CreateCompatibleDC(m_hDC)) );
		if( dc.IsNull() )
			return FALSE;

		// Create a DC for the monochrome DIB section
		_os_dc dcBW(::CreateCompatibleDC(m_hDC));
		if( dcBW.IsNull() ) {
			if( hSrcDC == NULL )
				dc.GetObject().Destroy();
			return FALSE;
		}

		// Create the monochrome DIB section with a black and white palette
		struct RGBBWBITMAPINFO
		{
			BITMAPINFOHEADER bmiHeader; 
			RGBQUAD bmiColors[2]; 
		};

		RGBBWBITMAPINFO rgbBWBitmapInfo = {
			{ sizeof(BITMAPINFOHEADER), iWidth, iHeight, 1, 1, BI_RGB, 0, 0, 0, 0, 0 },
			{ { 0x00, 0x00, 0x00, 0x00 }, { 0xFF, 0xFF, 0xFF, 0x00 } }
		};

		VOID* pbitsBW;
		_os_bitmap bmpBW( ::CreateDIBSection(dcBW, (LPBITMAPINFO)&rgbBWBitmapInfo, DIB_RGB_COLORS, &pbitsBW, NULL, 0) );
		if( bmpBW.IsNull() ) {
			if( hSrcDC == NULL )
				dc.GetObject().Destroy();
			return FALSE;
		}

		// Attach the monochrome DIB section and the bitmap to the DCs
		HBITMAP hbmOldBW = dcBW.SelectBitmap(bmpBW);
		HBITMAP hbmOldDC = NULL;
		if( hBitmap != NULL )
			hbmOldDC = dc.SelectBitmap(hBitmap);

		// Block: Dark gray removal: we want (128, 128, 128) pixels to become black and not white
		{
			_os_dc dcTemp1(::CreateCompatibleDC(m_hDC));
			_os_dc dcTemp2(::CreateCompatibleDC(m_hDC));
			_os_bitmap bmpTemp1;
			bmpTemp1.CreateCompatibleBitmap(dc, iWidth, iHeight);
			_os_bitmap bmpTemp2;
			bmpTemp2.CreateBitmap(iWidth, iHeight, 1, 1, NULL);
			HBITMAP hOldBmp1 = dcTemp1.SelectBitmap(bmpTemp1);
			HBITMAP hOldBmp2 = dcTemp2.SelectBitmap(bmpTemp2);
			// Let's copy our image, it will be altered
			dcTemp1.BitBlt(0, 0, iWidth, iHeight, dc, xSrc, ySrc, SRCCOPY);

			// All dark gray pixels will become white, the others black
			dcTemp1.SetBkColor(RGB(128, 128, 128));
			dcTemp2.BitBlt(0, 0, iWidth, iHeight, dcTemp1, 0, 0, SRCCOPY);
			// Do an XOR to set to black these white pixels
			dcTemp1.BitBlt(0, 0, iWidth, iHeight, dcTemp2, 0, 0, SRCINVERT);

			// BitBlt the bitmap into the monochrome DIB section
			// The DIB section will do a true monochrome conversion
			// The magenta background being closer to white will become white
			dcBW.BitBlt(0, 0, iWidth, iHeight, dcTemp1, 0, 0, SRCCOPY);

			// Cleanup
			dcTemp1.SelectBitmap(hOldBmp1);
			dcTemp2.SelectBitmap(hOldBmp2);
		}

		// Paint the destination rectangle using hBrushBackground
		if( hBrushBackground != NULL ) {
			RECT rc = { x, y, x + iWidth, y + iHeight };
			FillRect(&rc, hBrushBackground);
		}

		// BitBlt the black bits in the monochrome bitmap into hBrush3DEffect color in the destination DC
		// The magic ROP comes from the Charles Petzold's book
		HBRUSH hOldBrush = SelectBrush(hBrush3DEffect);
		BitBlt(x + 1, y + 1, iWidth, iHeight, dcBW, 0, 0, 0xB8074A);

		// BitBlt the black bits in the monochrome bitmap into hBrushDisabledImage color in the destination DC
		SelectBrush(hBrushDisabledImage);
		BitBlt(x, y, iWidth, iHeight, dcBW, 0, 0, 0xB8074A);

		SelectBrush(hOldBrush);
		dcBW.SelectBitmap(hbmOldBW);
		dc.SelectBitmap(hbmOldDC);

		if( hSrcDC == NULL )
			dc.GetObject().Destoy();

		return TRUE;
	}

// Text Functions
	BOOL TextOut(int x, int y, LPCWSTR lpszString, int iCount = -1) throw()
	{
		assert( !IsNull() );
		if( iCount == -1 )
			iCount = ::lstrlenW(lpszString);
		return ::TextOutW(m_hDC, x, y, lpszString, iCount);
	}
	BOOL ExtTextOut(int x, int y, UINT uOptions, const RECT* lpRect, LPCWSTR lpszString, UINT uCount = (UINT)-1, const INT* lpDxWidths = NULL) throw()
	{
		assert( !IsNull() );
		if( uCount == (UINT)-1 )
			uCount = ::lstrlenW(lpszString);
		return ::ExtTextOutW(m_hDC, x, y, uOptions, lpRect, lpszString, uCount, lpDxWidths);
	}
	SIZE TabbedTextOut(int x, int y, LPCWSTR lpszString, int iCount = -1, int iTabPositions = 0, const LPINT lpnTabStopPositions = NULL, int iTabOrigin = 0) throw()
	{
		assert( !IsNull() );
		if( iCount == -1 )
			iCount = ::lstrlenW(lpszString);
		LONG lRes = ::TabbedTextOutW(m_hDC, x, y, lpszString, iCount, iTabPositions, lpnTabStopPositions, iTabOrigin);
		SIZE size = { GET_X_LPARAM(lRes), GET_Y_LPARAM(lRes) };
		return size;
	}
	int DrawText(LPCWSTR lpstrText, int cchText, LPRECT lpRect, UINT uFormat) throw()
	{
		assert( !IsNull() );
		assert( (uFormat & DT_MODIFYSTRING) == 0 );
		return ::DrawTextW(m_hDC, lpstrText, cchText, lpRect, uFormat);
	}
	int DrawText(LPWSTR lpstrText, int cchText, LPRECT lpRect, UINT uFormat) throw()
	{
		assert( !IsNull() );
		return ::DrawTextW(m_hDC, lpstrText, cchText, lpRect, uFormat);
	}
	int DrawTextEx(LPWSTR lpstrText, int cchText, LPRECT lpRect, UINT uFormat, LPDRAWTEXTPARAMS lpDTParams = NULL) throw()
	{
		assert( !IsNull() );
		return ::DrawTextExW(m_hDC, lpstrText, cchText, lpRect, uFormat, lpDTParams);
	}
	int DrawShadowText(LPCWSTR lpstrText, int cchText, LPRECT lpRect, DWORD dwFlags, COLORREF clrText, COLORREF clrShadow, int xOffset, int yOffset) throw()
	{
		assert( !IsNull() );
		// This function is present only if comctl32.dll version 6 is loaded;
		// we use LoadLibrary/GetProcAddress to allow apps compiled with
		// _WIN32_WINNT >= 0x0501 to run on older Windows/CommCtrl
		int iRet = 0;
		HMODULE hCommCtrlDLL = ::LoadLibraryW(L"comctl32.dll");
		assert( hCommCtrlDLL != NULL );
		if( hCommCtrlDLL != NULL ) {
			typedef int (WINAPI *PFN_DrawShadowText)(HDC hDC, LPCWSTR lpstrText, UINT cchText, LPRECT lpRect, DWORD dwFlags, COLORREF clrText, COLORREF clrShadow, int xOffset, int yOffset);
			PFN_DrawShadowText pfnDrawShadowText = (PFN_DrawShadowText)::GetProcAddress(hCommCtrlDLL, "DrawShadowText");
			assert( pfnDrawShadowText != NULL );  // this function requires CommCtrl6
			if( pfnDrawShadowText != NULL )
				iRet = pfnDrawShadowText(m_hDC, lpstrText, cchText, lpRect, dwFlags, clrText, clrShadow, xOffset, yOffset);
			::FreeLibrary(hCommCtrlDLL);
		}
		return iRet;
	}
	BOOL GetTextExtent(LPCWSTR lpszString, int iCount, LPSIZE lpSize) const throw()
	{
		assert( !IsNull() );
		if( iCount == -1 )
			iCount = ::lstrlenW(lpszString);
		return ::GetTextExtentPoint32W(m_hDC, lpszString, iCount, lpSize);
	}
	BOOL GetTextExtentExPoint(LPCWSTR lpszString, int cchString, LPSIZE lpSize, int iMaxExtent, LPINT lpnFit = NULL, LPINT alpDx = NULL) const throw()
	{
		assert( !IsNull() );
		return ::GetTextExtentExPointW(m_hDC, lpszString, cchString, iMaxExtent, lpnFit, alpDx, lpSize);
	}
	DWORD GetTabbedTextExtent(LPCWSTR lpszString, int iCount = -1, int iTabPositions = 0, const LPINT lpnTabStopPositions = NULL) const throw()
	{
		assert( !IsNull() );
		if( iCount == -1 )
			iCount = ::lstrlenW(lpszString);
		return ::GetTabbedTextExtentW(m_hDC, lpszString, iCount, iTabPositions, lpnTabStopPositions);
	}
	BOOL GrayString(HBRUSH hBrush, BOOL (CALLBACK* lpfnOutput)(HDC, LPARAM, int), LPARAM lpData, int iCount, int x, int y, int iWidth, int iHeight) throw()
	{
		assert( !IsNull() );
		return ::GrayStringW(m_hDC, hBrush, (GRAYSTRINGPROC)lpfnOutput, lpData, iCount, x, y, iWidth, iHeight);
	}
	UINT GetTextAlign() const throw()
	{
		assert( !IsNull() );
		return ::GetTextAlign(m_hDC);
	}
	UINT SetTextAlign(UINT uFlags) throw()
	{
		assert( !IsNull() );
		return ::SetTextAlign(m_hDC, uFlags);
	}
	int GetTextFace(LPWSTR lpszFacename, int iCount) const throw()
	{
		assert( !IsNull() );
		return ::GetTextFaceW(m_hDC, iCount, lpszFacename);
	}
	int GetTextFaceLen() const throw()
	{
		assert( !IsNull() );
		return ::GetTextFaceW(m_hDC, 0, NULL);
	}
	BOOL GetTextMetrics(LPTEXTMETRICW lpMetrics) const throw()
	{
		assert( !IsNull() );
		return ::GetTextMetricsW(m_hDC, lpMetrics);
	}
	BOOL SetTextJustification(int iBreakExtra, int iBreakCount) throw()
	{
		assert( !IsNull() );
		return ::SetTextJustification(m_hDC, iBreakExtra, iBreakCount);
	}
	int GetTextCharacterExtra() const throw()
	{
		assert( !IsNull() );
		return ::GetTextCharacterExtra(m_hDC);
	}
	int SetTextCharacterExtra(int iCharExtra) throw()
	{
		assert( !IsNull() );
		return ::SetTextCharacterExtra(m_hDC, iCharExtra);
	}

// Advanced Drawing
	BOOL DrawEdge(LPRECT lpRect, UINT uEdge, UINT uFlags) throw()
	{
		assert( !IsNull() );
		return ::DrawEdge(m_hDC, lpRect, uEdge, uFlags);
	}
	BOOL DrawFrameControl(LPRECT lpRect, UINT uType, UINT uState) throw()
	{
		assert( !IsNull() );
		return ::DrawFrameControl(m_hDC, lpRect, uType, uState);
	}

// Scrolling Functions
	BOOL ScrollDC(int dx, int dy, const RECT* lpRectScroll, const RECT* lpRectClip, HRGN hRgnUpdate, LPRECT lpRectUpdate) throw()
	{
		assert( !IsNull() );
		return ::ScrollDC(m_hDC, dx, dy, lpRectScroll, lpRectClip, hRgnUpdate, lpRectUpdate);
	}

// Font Functions
	BOOL GetCharWidth(UINT uFirstChar, UINT uLastChar, LPINT lpBuffer) const throw()
	{
		assert( !IsNull() );
		return ::GetCharWidthW(m_hDC, uFirstChar, uLastChar, lpBuffer);
	}
	// GetCharWidth32 is not supported under Win9x
	BOOL GetCharWidth32(UINT uFirstChar, UINT uLastChar, LPINT lpBuffer) const throw()
	{
		assert( !IsNull() );
		return ::GetCharWidth32W(m_hDC, uFirstChar, uLastChar, lpBuffer);
	}
	DWORD SetMapperFlags(DWORD dwFlag) throw()
	{
		assert( !IsNull() );
		return ::SetMapperFlags(m_hDC, dwFlag);
	}
	BOOL GetAspectRatioFilter(LPSIZE lpSize) const throw()
	{
		assert( !IsNull() );
		return ::GetAspectRatioFilterEx(m_hDC, lpSize);
	}
	BOOL GetCharABCWidths(UINT uFirstChar, UINT uLastChar, LPABC lpabc) const throw()
	{
		assert( !IsNull() );
		return ::GetCharABCWidthsW(m_hDC, uFirstChar, uLastChar, lpabc);
	}
	DWORD GetFontData(DWORD dwTable, DWORD dwOffset, LPVOID lpData, DWORD cbData) const throw()
	{
		assert( !IsNull() );
		return ::GetFontData(m_hDC, dwTable, dwOffset, lpData, cbData);
	}
	DWORD GetKerningPairs(DWORD dwPairs, LPKERNINGPAIR lpkrnpair) const throw()
	{
		assert( !IsNull() );
		return ::GetKerningPairsW(m_hDC, nPairs, lpkrnpair);
	}
	UINT GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRICW lpotm) const throw()
	{
		assert( !IsNull() );
		return ::GetOutlineTextMetricsW(m_hDC, cbData, lpotm);
	}
	DWORD GetGlyphOutline(UINT uChar, UINT uFormat, LPGLYPHMETRICS lpgm, DWORD cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const throw()
	{
		assert( !IsNull() );
		return ::GetGlyphOutlineW(m_hDC, uChar, uFormat, lpgm, cbBuffer, lpBuffer, lpmat2);
	}
	BOOL GetCharABCWidths(UINT uFirstChar, UINT uLastChar, LPABCFLOAT lpABCF) const throw()
	{
		assert( !IsNull() );
		return ::GetCharABCWidthsFloatW(m_hDC, uFirstChar, uLastChar, lpABCF);
	}
	BOOL GetCharWidth(UINT uFirstChar, UINT uLastChar, float* lpFloatBuffer) const throw()
	{
		assert( !IsNull() );
		return ::GetCharWidthFloatW(m_hDC, uFirstChar, uLastChar, lpFloatBuffer);
	}

// Printer/Device Escape Functions
	int Escape(int iEscape, int iCount, LPCSTR lpszInData, LPVOID lpOutData) throw()
	{
		assert( !IsNull() );
		return ::Escape(m_hDC, iEscape, iCount, lpszInData, lpOutData);
	}
	int Escape(int iEscape, int iInputSize, LPCSTR lpszInputData,
		int iOutputSize, LPSTR lpszOutputData) throw()
	{
		assert( !IsNull() );
		return ::ExtEscape(m_hDC, iEscape, iInputSize, lpszInputData, iOutputSize, lpszOutputData);
	}
	int DrawEscape(int iEscape, int iInputSize, LPCSTR lpszInputData) throw()
	{
		assert( !IsNull() );
		return ::DrawEscape(m_hDC, iEscape, iInputSize, lpszInputData);
	}

	// Escape helpers
#if defined(StartDoc)
	int StartDoc(LPCWSTR lpszDocName) throw()  // old Win3.0 version
	{
		DOCINFOW di = { 0 };
		di.cbSize = sizeof(DOCINFOW);
		di.lpszDocName = lpszDocName;
		return StartDoc(&di);
	}
	int StartDoc(const DOCINFOW* lpDocInfo) throw()
	{
		assert( !IsNull() );
		return ::StartDocW(m_hDC, lpDocInfo);
	}
	int StartPage() throw()
	{
		assert( !IsNull() );
		return ::StartPage(m_hDC);
	}
	int EndPage() throw()
	{
		assert( !IsNull() );
		return ::EndPage(m_hDC);
	}
	int SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, int)) throw()
	{
		assert( !IsNull() );
		return ::SetAbortProc(m_hDC, (ABORTPROC)lpfn);
	}
	int AbortDoc() throw()
	{
		assert( !IsNull() );
		return ::AbortDoc(m_hDC);
	}
	int EndDoc() throw()
	{
		assert( !IsNull() );
		return ::EndDoc(m_hDC);
	}
#endif // defined(StartDoc)

// MetaFile Functions
	BOOL PlayMetaFile(HMETAFILE hMF) throw()
	{
		assert( !IsNull() );
		if( ::GetDeviceCaps(m_hDC, TECHNOLOGY) == DT_METAFILE ) {
			// playing metafile in metafile, just use core windows API
			return ::PlayMetaFile(m_hDC, hMF);
		}
		// for special playback, lParam == pDC
		return ::EnumMetaFile(m_hDC, hMF, EnumMetaFileProc, (LPARAM)this);
	}
	BOOL PlayMetaFile(HENHMETAFILE hEnhMetaFile, const RECT* lpBounds) throw()
	{
		assert( !IsNull() );
		return ::PlayEnhMetaFile(m_hDC, hEnhMetaFile, lpBounds);
	}
	BOOL AddMetaFileComment(UINT uDataSize, const BYTE* pCommentData) throw()  // can be used for enhanced metafiles only
	{
		assert( !IsNull() );
		return ::GdiComment(m_hDC, uDataSize, pCommentData);
	}
	// Special handling for metafile playback
	static int CALLBACK EnumMetaFileProc(HDC hDC, HANDLETABLE* pHandleTable, METARECORD* pMetaRec, int iHandles, LPARAM lParam) throw()
	{
		_os_dc* pDC = (_os_dc*)lParam;

		switch( pMetaRec->rdFunction ) {
		case META_SETMAPMODE:
			pDC->SetMapMode((int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SETWINDOWEXT:
			pDC->SetWindowExt((int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SETWINDOWORG:
			pDC->SetWindowOrg((int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SETVIEWPORTEXT:
			pDC->SetViewportExt((int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SETVIEWPORTORG:
			pDC->SetViewportOrg((int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SCALEWINDOWEXT:
			pDC->ScaleWindowExt((int)(short)pMetaRec->rdParm[3], (int)(short)pMetaRec->rdParm[2],
				(int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SCALEVIEWPORTEXT:
			pDC->ScaleViewportExt((int)(short)pMetaRec->rdParm[3], (int)(short)pMetaRec->rdParm[2],
				(int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
			break;
		case META_OFFSETVIEWPORTORG:
			pDC->OffsetViewportOrg((int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SAVEDC:
			pDC->SaveDC();
			break;
		case META_RESTOREDC:
			pDC->RestoreDC((int)(short)pMetaRec->rdParm[0]);
			break;
		case META_SETBKCOLOR:
			pDC->SetBkColor(*(UNALIGNED COLORREF*)&pMetaRec->rdParm[0]);
			break;
		case META_SETTEXTCOLOR:
			pDC->SetTextColor(*(UNALIGNED COLORREF*)&pMetaRec->rdParm[0]);
			break;

		// need to watch out for SelectObject(HFONT), for custom font mapping
		case META_SELECTOBJECT:
			{
				HGDIOBJ hObject = pHandleTable->objectHandle[pMetaRec->rdParm[0]];
				UINT uObjType = ::GetObjectTypeW(hObject);
				if( uObjType == 0 ) {
					// object type is unknown, determine if it is a font
					HFONT hStockFont = (HFONT)::GetStockObject(SYSTEM_FONT);
					HFONT hFontOld = (HFONT)::SelectObject(pDC->m_hDC, hStockFont);
					HGDIOBJ hObjOld = ::SelectObject(pDC->m_hDC, hObject);
					if( hObjOld == hStockFont ) {
						// got the stock object back, so must be selecting a font
						pDC->SelectFont((HFONT)hObject);
						break;  // don't play the default record
					}
					else {
						// didn't get the stock object back, so restore everything
						::SelectObject(pDC->m_hDC, hFontOld);
						::SelectObject(pDC->m_hDC, hObjOld);
					}
					// and fall through to PlayMetaFileRecord...
				}
				else if( uObjType == OBJ_FONT ) {
					// play back as _DCHandle::SelectFont(HFONT)
					pDC->SelectFont((HFONT)hObject);
					break;  // don't play the default record
				}
			}
			// fall through...

		default:
			::PlayMetaFileRecord(hDC, pHandleTable, pMetaRec, iHandles);  //no check
			break;
		}

		return 1;
	}

// Path Functions
	BOOL AbortPath() throw()
	{
		assert( !IsNull() );
		return ::AbortPath(m_hDC);
	}
	BOOL BeginPath() throw()
	{
		assert( !IsNull() );
		return ::BeginPath(m_hDC);
	}
	BOOL CloseFigure() throw()
	{
		assert( !IsNull() );
		return ::CloseFigure(m_hDC);
	}
	BOOL EndPath() throw()
	{
		assert( !IsNull() );
		return ::EndPath(m_hDC);
	}
	BOOL FillPath() throw()
	{
		assert( !IsNull() );
		return ::FillPath(m_hDC);
	}
	BOOL FlattenPath() throw()
	{
		assert( !IsNull() );
		return ::FlattenPath(m_hDC);
	}
	BOOL StrokeAndFillPath() throw()
	{
		assert( !IsNull() );
		return ::StrokeAndFillPath(m_hDC);
	}
	BOOL StrokePath() throw()
	{
		assert( !IsNull() );
		return ::StrokePath(m_hDC);
	}
	BOOL WidenPath() throw()
	{
		assert( !IsNull() );
		return ::WidenPath(m_hDC);
	}
	BOOL GetMiterLimit(PFLOAT pfMiterLimit) const throw()
	{
		assert( !IsNull() );
		return ::GetMiterLimit(m_hDC, pfMiterLimit);
	}
	BOOL SetMiterLimit(float fMiterLimit) throw()
	{
		assert( !IsNull() );
		return ::SetMiterLimit(m_hDC, fMiterLimit, NULL);
	}
	int GetPath(LPPOINT lpPoints, LPBYTE lpTypes, int iCount) const throw()
	{
		assert( !IsNull() );
		return ::GetPath(m_hDC, lpPoints, lpTypes, iCount);
	}
	BOOL SelectClipPath(int iMode) throw()
	{
		assert( !IsNull() );
		return ::SelectClipPath(m_hDC, iMode);
	}

// Misc Helper Functions
	static HBRUSH PASCAL GetHalftoneBrush() throw()
	{
		HBRUSH halftoneBrush = NULL;
		WORD grayPattern[8] = { 0 };
		for( int i = 0; i < 8; i++ )
			grayPattern[i] = (WORD)(0x5555 << (i & 1));
		HBITMAP grayBitmap = ::CreateBitmap(8, 8, 1, 1, &grayPattern);
		if( grayBitmap != NULL ) {
			halftoneBrush = ::CreatePatternBrush(grayBitmap);
			::DeleteObjectW(grayBitmap);
		}
		return halftoneBrush;
	}
	void DrawDragRect(const RECT* lpRect, const SIZE& size, const RECT* lpRectLast, const SIZE& sizeLast, HBRUSH hBrush = NULL, HBRUSH hBrushLast = NULL) throw()
	{
		// first, determine the update region and select it
		_os_rgn rgnOutside;
		rgnOutside.CreateRectRgn(lpRect);
		RECT rect = *lpRect;
		::InflateRect(&rect, -size.cx, -size.cy);
		::IntersectRect(&rect, &rect, lpRect);
		_os_rgn rgnInside;
		rgnInside.CreateRectRgn(&rect);
		_os_rgn rgnNew;
		rgnNew.CreateRectRgn(0, 0, 0, 0);
		rgnNew.CombineRgn(rgnOutside, rgnInside, RGN_XOR);

		HBRUSH hBrushOld = NULL;
		_os_brush brushHalftone;
		if( hBrush == NULL )
			brushHalftone(hBrush = _os_dc::GetHalftoneBrush());
		if( hBrushLast == NULL )
			hBrushLast = hBrush;

		_os_rgn rgnLast;
		_os_rgn rgnUpdate;
		if( lpRectLast != NULL ) {
			// find difference between new region and old region
			rgnLast.CreateRectRgn(0, 0, 0, 0);
			rgnOutside.SetRectRgn(lpRectLast->left, lpRectLast->top, lpRectLast->right, lpRectLast->bottom);
			rect = *lpRectLast;
			::InflateRect(&rect, -sizeLast.cx, -sizeLast.cy);
			::IntersectRect(&rect, &rect, lpRectLast);
			rgnInside.SetRectRgn(rect.left, rect.top, rect.right, rect.bottom);
			rgnLast.CombineRgn(rgnOutside, rgnInside, RGN_XOR);

			// only diff them if brushes are the same
			if( hBrush == hBrushLast ) {
				rgnUpdate.CreateRectRgn(0, 0, 0, 0);
				rgnUpdate.CombineRgn(rgnLast, rgnNew, RGN_XOR);
			}
		}
		if( hBrush != hBrushLast && lpRectLast != NULL ) {
			// brushes are different -- erase old region first
			SelectClipRgn(rgnLast);
			GetClipBox(&rect);
			hBrushOld = SelectBrush(hBrushLast);
			PatBlt(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATINVERT);
			SelectBrush(hBrushOld);
			hBrushOld = NULL;
		}

		// draw into the update/new region
		SelectClipRgn(rgnUpdate.IsNull() ? rgnNew : rgnUpdate);
		GetClipBox(&rect);
		hBrushOld = SelectBrush(hBrush);
		PatBlt(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATINVERT);

		// cleanup DC
		if( hBrushOld != NULL )
			SelectBrush(hBrushOld);
		SelectClipRgn(NULL);
	}
	void FillSolidRect(const RECT* lpRect, COLORREF clr) throw()
	{
		assert( !IsNull() );
		COLORREF clrOld = ::SetBkColor(m_hDC, clr);
		assert( clrOld != CLR_INVALID );
		if( clrOld != CLR_INVALID ) {
			::ExtTextOutW(m_hDC, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
			::SetBkColor(m_hDC, clrOld);
		}
	}
	void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr) throw()
	{
		RECT rect = { x, y, x + cx, y + cy };
		FillSolidRect(&rect, clr);
	}
	void Draw3dRect(const RECT* lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight) throw()
	{
		Draw3dRect(lpRect->left, lpRect->top, lpRect->right - lpRect->left,
			lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);
	}
	void Draw3dRect(int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight) throw()
	{
		FillSolidRect(x, y, cx - 1, 1, clrTopLeft);
		FillSolidRect(x, y, 1, cy - 1, clrTopLeft);
		FillSolidRect(x + cx, y, -1, cy, clrBottomRight);
		FillSolidRect(x, y + cy, cx, -1, clrBottomRight);
	}

// DIB support
	int SetDIBitsToDevice(int x, int y, DWORD dwWidth, DWORD dwHeight, int xSrc, int ySrc, UINT uStartScan, UINT cScanLines, CONST VOID* lpvBits, CONST BITMAPINFO* lpbmi, UINT uColorUse) throw()
	{
		assert( !IsNull() );
		return ::SetDIBitsToDevice(m_hDC, x, y, dwWidth, dwHeight, xSrc, ySrc, uStartScan, cScanLines, lpvBits, lpbmi, uColorUse);
	}
	int StretchDIBits(int x, int y, int iWidth, int iHeight, int xSrc, int ySrc, int iSrcWidth, int iSrcHeight, CONST VOID* lpvBits, CONST BITMAPINFO* lpbmi, UINT uColorUse, DWORD dwRop) throw()
	{
		assert( !IsNull() );
		return ::StretchDIBits(m_hDC, x, y, iWidth, iHeight, xSrc, ySrc, iSrcWidth, iSrcHeight, lpvBits, lpbmi, uColorUse, dwRop);
	}
	UINT GetDIBColorTable(UINT uStartIndex, UINT cEntries, RGBQUAD* pColors) const throw()
	{
		assert( !IsNull() );
		return ::GetDIBColorTable(m_hDC, uStartIndex, cEntries, pColors);
	}
	UINT SetDIBColorTable(UINT uStartIndex, UINT cEntries, CONST RGBQUAD* pColors) throw()
	{
		assert( !IsNull() );
		return ::SetDIBColorTable(m_hDC, uStartIndex, cEntries, pColors);
	}

// OpenGL support
	int ChoosePixelFormat(CONST PIXELFORMATDESCRIPTOR* ppfd) throw()
	{
		assert( !IsNull() );
		return ::ChoosePixelFormat(m_hDC, ppfd);
	}
	int DescribePixelFormat(int iPixelFormat, UINT uBytes, LPPIXELFORMATDESCRIPTOR ppfd) throw()
	{
		assert( !IsNull() );
		return ::DescribePixelFormat(m_hDC, iPixelFormat, uBytes, ppfd);
	}
	int GetPixelFormat() const throw()
	{
		assert( !IsNull() );
		return ::GetPixelFormat(m_hDC);
	}
	BOOL SetPixelFormat(int iPixelFormat, CONST PIXELFORMATDESCRIPTOR* ppfd) throw()
	{
		assert( !IsNull() );
		return ::SetPixelFormat(m_hDC, iPixelFormat, ppfd);
	}
	BOOL SwapBuffers() throw()
	{
		assert( !IsNull() );
		return ::SwapBuffers(m_hDC);
	}
	HGLRC wglCreateContext() throw()
	{
		assert( !IsNull() );
		return ::wglCreateContext(m_hDC);
	}
	HGLRC wglCreateLayerContext(int iLayerPlane) throw()
	{
		assert( !IsNull() );
		return ::wglCreateLayerContext(m_hDC, iLayerPlane);
	}
	BOOL wglMakeCurrent(HGLRC hglrc) throw()
	{
		assert( !IsNull() );
		return ::wglMakeCurrent(m_hDC, hglrc);
	}
	BOOL wglUseFontBitmaps(DWORD dwFirst, DWORD dwCount, DWORD listBase) throw()
	{
		assert( !IsNull() );
		return ::wglUseFontBitmapsW(m_hDC, dwFirst, dwCount, listBase);
	}
	BOOL wglUseFontOutlines(DWORD dwFirst, DWORD dwCount, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf) throw()
	{
		assert( !IsNull() );
		return ::wglUseFontOutlinesW(m_hDC, dwFirst, dwCount, listBase, deviation, extrusion, format, lpgmf);
	}
	BOOL wglDescribeLayerPlane(int iPixelFormat, int iLayerPlane, UINT uBytes, LPLAYERPLANEDESCRIPTOR plpd) throw()
	{
		assert( !IsNull() );
		return ::wglDescribeLayerPlane(m_hDC, iPixelFormat, iLayerPlane, uBytes, plpd);
	}
	int wglSetLayerPaletteEntries(int iLayerPlane, int iStart, int cEntries, CONST COLORREF* pclr) throw()
	{
		assert( !IsNull() );
		return ::wglSetLayerPaletteEntries(m_hDC, iLayerPlane, iStart, cEntries, pclr);
	}
	int wglGetLayerPaletteEntries(int iLayerPlane, int iStart, int cEntries, COLORREF* pclr) const throw()
	{
		assert( !IsNull() );
		return ::wglGetLayerPaletteEntries(m_hDC, iLayerPlane, iStart, cEntries, pclr);
	}
	BOOL wglRealizeLayerPalette(int iLayerPlane, BOOL bRealize) throw()
	{
		assert( !IsNull() );
		return ::wglRealizeLayerPalette(m_hDC, iLayerPlane, bRealize);
	}
	BOOL wglSwapLayerBuffers(UINT uPlanes) throw()
	{
		assert( !IsNull() );
		return ::wglSwapLayerBuffers(m_hDC, uPlanes);
	}

// New for Windows 2000 only
	COLORREF GetDCPenColor() const throw()
	{
		assert( !IsNull() );
		return ::GetDCPenColor(m_hDC);
	}
	COLORREF SetDCPenColor(COLORREF clr) throw()
	{
		assert( !IsNull() );
		return ::SetDCPenColor(m_hDC, clr);
	}
	COLORREF GetDCBrushColor() const throw()
	{
		assert( !IsNull() );
		return ::GetDCBrushColor(m_hDC);
	}
	COLORREF SetDCBrushColor(COLORREF clr) throw()
	{
		assert( !IsNull() );
		return ::SetDCBrushColor(m_hDC, clr);
	}
	DWORD GetFontUnicodeRanges(LPGLYPHSET lpgs) const throw()
	{
		assert( !IsNull() );
		return ::GetFontUnicodeRanges(m_hDC, lpgs);
	}
	DWORD GetGlyphIndices(LPCWSTR lpstr, int cch, LPWORD pgi, DWORD dwFlags) const throw()
	{
		assert( !IsNull() );
		return ::GetGlyphIndicesW(m_hDC, lpstr, cch, pgi, dwFlags);
	}
	BOOL GetTextExtentPointI(LPWORD pgiIn, int cgi, LPSIZE lpSize) const throw()
	{
		assert( !IsNull() );
		return ::GetTextExtentPointI(m_hDC, pgiIn, cgi, lpSize);
	}
	BOOL GetTextExtentExPointI(LPWORD pgiIn, int cgi, int iMaxExtent, LPINT lpnFit, LPINT alpDx, LPSIZE lpSize) const throw()
	{
		assert( !IsNull() );
		return ::GetTextExtentExPointI(m_hDC, pgiIn, cgi, iMaxExtent, lpnFit, alpDx, lpSize);
	}
	BOOL GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, LPINT lpBuffer) const throw()
	{
		assert( !IsNull() );
		return ::GetCharWidthI(m_hDC, giFirst, cgi, pgi, lpBuffer);
	}
	BOOL GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi, LPABC lpabc) const throw()
	{
		assert( !IsNull() );
		return ::GetCharABCWidthsI(m_hDC, giFirst, cgi, pgi, lpabc);
	}

// New for Windows 2000 and Windows 98
	BOOL ColorCorrectPalette(HPALETTE hPalette, DWORD dwFirstEntry, DWORD dwNumOfEntries) throw()
	{
		assert( !IsNull() );
		return ::ColorCorrectPalette(m_hDC, hPalette, dwFirstEntry, dwNumOfEntries);
	}

protected:
	HDC m_hDC;  //handle

private:
	//noncopyable
	_os_dc(const _os_dc& src) throw();
	_os_dc& operator=(const _os_dc& src) throw();
};

////////////////////////////////////////////////////////////////////////////////
// DC Helpers

// _os_paint_dc

class _os_paint_dc : public _os_dc
{
public:
	explicit _os_paint_dc(HWND hWnd) throw()
	{
		assert( ::IsWindow(hWnd) );
		m_hWnd = hWnd;
		m_hDC = ::BeginPaint(hWnd, &m_ps);
	}
	~_os_paint_dc() throw()
	{
		assert( !IsNull() );
		assert( ::IsWindow(m_hWnd) );
		::EndPaint(m_hWnd, &m_ps);  //no check
		Detach();
	}

protected:
	HWND m_hWnd;
	PAINTSTRUCT m_ps;

private:
	//noncopyable
	_os_paint_dc(const _os_paint_dc& src) throw();
	_os_paint_dc& operator=(const _os_paint_dc& src) throw();
};

// _os_client_dc

class _os_client_dc : public _os_dc
{
public:
	explicit _os_client_dc(HWND hWnd) throw()
	{
		assert( hWnd == NULL || ::IsWindow(hWnd) );
		m_hWnd = hWnd;
		m_hDC = ::GetDC(hWnd);
	}
	~_os_client_dc() throw()
	{
		assert( !IsNull() );
		::ReleaseDC(m_hWnd, Detach());  //no check
	}

protected:
	HWND m_hWnd;

private:
	//noncopyable
	_os_client_dc(const _os_client_dc& src) throw();
	_os_client_dc& operator=(const _os_client_dc& src) throw();
};

// _os_window_dc

class _os_window_dc : public _os_dc
{
public:
	explicit _os_window_dc(HWND hWnd) throw()
	{
		assert( hWnd == NULL || ::IsWindow(hWnd) );
		m_hWnd = hWnd;
		m_hDC = ::GetWindowDC(hWnd);
	}
	~_os_window_dc() throw()
	{
		assert( !IsNull() );
		::ReleaseDC(m_hWnd, Detach());  //no check
	}

protected:
	HWND m_hWnd;

private:
	//noncopyable
	_os_window_dc(const _os_window_dc& src) throw();
	_os_window_dc& operator=(const _os_window_dc& src) throw();
};

// _os_memory_dc

class _os_memory_dc : public _os_dc
{
public:
	_os_memory_dc(HDC hDC, const RECT& rcPaint) throw() : m_hDCOriginal(hDC), m_hBmpOld(NULL)
	{
		m_rcPaint = rcPaint;
		CreateCompatibleDC(m_hDCOriginal);
		assert( !IsNull() );
		m_bmp.CreateCompatibleBitmap(m_hDCOriginal, m_rcPaint.right - m_rcPaint.left, m_rcPaint.bottom - m_rcPaint.top);
		assert( !m_bmp.IsNull() );
		m_hBmpOld = SelectBitmap(m_bmp);
		SetViewportOrg(-m_rcPaint.left, -m_rcPaint.top);
	}
	~_os_memory_dc() throw()
	{
		::BitBlt(m_hDCOriginal, m_rcPaint.left, m_rcPaint.top, m_rcPaint.right - m_rcPaint.left, m_rcPaint.bottom - m_rcPaint.top, m_hDC, m_rcPaint.left, m_rcPaint.top, SRCCOPY);
		SelectBitmap(m_hBmpOld);
	}

protected:
	HDC m_hDCOriginal;
	RECT m_rcPaint;
	_os_bitmap m_bmp;
	HBITMAP m_hBmpOld;

private:
	//noncopyable
	_os_memory_dc(const _os_memory_dc& src) throw();
	_os_memory_dc& operator=(const _os_memory_dc& src) throw();
};

////////////////////////////////////////////////////////////////////////////////
// Enhanced metafile support

// _os_enh_meta_file_info

class _os_enh_meta_file_info
{
public:

// Constructor/destructor
	explicit _os_enh_meta_file_info(HENHMETAFILE hEMF) throw() : m_pBits(NULL), m_pDesc(NULL), m_hEMF(hEMF)
	{
	}
	~_os_enh_meta_file_info() throw()
	{
		if( m_pDesc != NULL )
			crt_free(m_pDesc);
		if( m_pBits != NULL )
			crt_free(m_pBits);
	}

// Operations
	BYTE* GetEnhMetaFileBits() throw()
	{
		assert( m_hEMF != NULL );
		UINT uBytes = ::GetEnhMetaFileBits(m_hEMF, 0, NULL);
		crt_free(m_pBits);
		m_pBits = NULL;
		m_pBits = (BYTE*)crt_alloc(uBytes * sizeof(BYTE));
		if( m_pBits != NULL )
			::GetEnhMetaFileBits(m_hEMF, uBytes, m_pBits);
		return m_pBits;
	}
	LPWSTR GetEnhMetaFileDescription() throw()
	{
		assert( m_hEMF != NULL );
		UINT uLen = ::GetEnhMetaFileDescriptionW(m_hEMF, 0, NULL);
		crt_free(m_pDesc);
		m_pDesc = NULL;
		m_pDesc = (WCHAR*)crt_alloc(uLen * sizeof(WCHAR));
		if( m_pDesc != NULL )
			uLen = ::GetEnhMetaFileDescriptionW(m_hEMF, uLen, m_pDesc);
		return m_pDesc;
	}
	ENHMETAHEADER* GetEnhMetaFileHeader() throw()
	{
		assert( m_hEMF != NULL );
		mem_zero(&m_header, sizeof(m_header));
		m_header.iType = EMR_HEADER;
		m_header.nSize = sizeof(ENHMETAHEADER);
		UINT u = ::GetEnhMetaFileHeader(m_hEMF, sizeof(ENHMETAHEADER), &m_header);
		return (u != 0) ? &m_header : NULL;
	}
	PIXELFORMATDESCRIPTOR* GetEnhMetaFilePixelFormat() throw()
	{
		assert( m_hEMF != NULL );
		mem_zero(&m_pfd, sizeof(m_pfd));
		UINT u = ::GetEnhMetaFilePixelFormat(m_hEMF, sizeof(m_pfd), &m_pfd);
		return (u != 0 && u != GDI_ERROR) ? &m_pfd : NULL;
	}

private:
	HENHMETAFILE m_hEMF;
	BYTE* m_pBits;
	WCHAR* m_pDesc;
	ENHMETAHEADER m_header;
	PIXELFORMATDESCRIPTOR m_pfd;

private:
	//noncopyable
	_os_enh_meta_file_info(const _os_enh_meta_file_info& src) throw();
	_os_enh_meta_file_info& operator=(const _os_enh_meta_file_info& src) throw();
};

// _os_enh_meta_file

class _os_enh_meta_file
{
public:
	explicit _os_enh_meta_file(HENHMETAFILE hEMF = NULL) throw() : m_hEMF(hEMF)
	{
	}
	_os_enh_meta_file(_os_enh_meta_file&& src) throw()
	{
		m_hEMF = src.m_hEMF;
		src.m_hEMF = NULL;
	}
	~_os_enh_meta_file() throw()
	{
		Destroy();
	}

	void Destroy() throw()
	{
		if( m_hEMF != NULL ) {
			BOOL bRet = ::DeleteEnhMetaFileW(m_hEMF);
			assert( bRet );
			m_hEMF = NULL;
		}
	}

// Operations
	_os_enh_meta_file& operator=(_os_enh_meta_file&& src) throw()
	{
		if( this != &src ) {
			if( m_hEMF != src.m_hEMF ) {
				Destroy();
				m_hEMF = src.m_hEMF;
				src.m_hEMF = NULL;
			}
		}
		return *this;
	}

	void Attach(HENHMETAFILE hEMF) throw()
	{
		if( m_hEMF != hEMF )
			Destroy();
		m_hEMF = hEMF;
	}
	HENHMETAFILE Detach() throw()
	{
		HENHMETAFILE hEMF = m_hEMF;
		m_hEMF = NULL;
		return hEMF;
	}
	HENHMETAFILE GetHandle() const throw()
	{
		return m_hEMF;
	}

	bool IsNull() const throw()
	{
		return (m_hEMF == NULL);
	}

	UINT GetEnhMetaFileBits(UINT cbBuffer, LPBYTE lpbBuffer) const throw()
	{
		assert( !IsNull() );
		return ::GetEnhMetaFileBits(m_hEMF, cbBuffer, lpbBuffer);
	}
	UINT GetEnhMetaFileDescription(UINT cchBuffer, LPWSTR lpszDescription) const throw()
	{
		assert( !IsNull() );
		return ::GetEnhMetaFileDescriptionW(m_hEMF, cchBuffer, lpszDescription);
	}
	UINT GetEnhMetaFileHeader(LPENHMETAHEADER lpemh) const throw()
	{
		assert( !IsNull() );
		lpemh->iType = EMR_HEADER;
		lpemh->nSize = sizeof(ENHMETAHEADER);
		return ::GetEnhMetaFileHeader(m_hEMF, sizeof(ENHMETAHEADER), lpemh);
	}
	UINT GetEnhMetaFilePaletteEntries(UINT cEntries, LPPALETTEENTRY lppe) const throw()
	{
		assert( !IsNull() );
		return ::GetEnhMetaFilePaletteEntries(m_hEMF, cEntries, lppe);
	}
	UINT GetEnhMetaFilePixelFormat(DWORD cbBuffer, PIXELFORMATDESCRIPTOR* ppfd) const throw()
	{
		assert( !IsNull() );
		return ::GetEnhMetaFilePixelFormat(m_hEMF, cbBuffer, ppfd);
	}

private:
	HENHMETAFILE m_hEMF;

private:
	_os_enh_meta_file(const _os_enh_meta_file& src) throw();
	_os_enh_meta_file& operator=(const _os_enh_meta_file& src) throw();
};

// _os_enh_meta_file_dc

class _os_enh_meta_file_dc : public _os_dc
{
public:
	_os_enh_meta_file_dc() throw()
	{
	}
	_os_enh_meta_file_dc(HDC hdc, const RECT* lpRect) throw()
	{
		Create(hdc, NULL, lpRect, NULL);
		assert( m_hDC != NULL );
	}
	_os_enh_meta_file_dc(HDC hdcRef, LPCWSTR lpFilename, const RECT* lpRect, LPCWSTR lpDescription) throw()
	{
		Create(hdcRef, lpFilename, lpRect, lpDescription);
		assert( m_hDC != NULL );
	}
	~_os_enh_meta_file_dc() throw()
	{
		HENHMETAFILE hEMF = Close();
		if( hEMF != NULL )
			::DeleteEnhMetaFileW(hEMF);  //no check
	}

// Operations
	void Create(HDC hdcRef, LPCWSTR lpFilename, const RECT* lpRect, LPCWSTR lpDescription) throw()
	{
		assert( IsNull() );
		m_hDC = ::CreateEnhMetaFileW(hdcRef, lpFilename, lpRect, lpDescription);
	}
	HENHMETAFILE Close() throw()
	{
		HENHMETAFILE hEMF = NULL;
		if( !IsNull() ) {
			hEMF = ::CloseEnhMetaFile(m_hDC);
			m_hDC = NULL;
		}
		return hEMF;
	}

private:
	_os_enh_meta_file_dc(const _os_enh_meta_file_dc& src) throw();
	_os_enh_meta_file_dc& operator=(const _os_enh_meta_file_dc& src) throw();
};

////////////////////////////////////////////////////////////////////////////////
