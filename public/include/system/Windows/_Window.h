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
This file contains classes for Window handle.

Author: Lijuan Mei
*/

////////////////////////////////////////////////////////////////////////////////
// internal header
////////////////////////////////////////////////////////////////////////////////

#ifndef __WINDOW_H__
#define __WINDOW_H__

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//internal

class _U_RECT
{
public:
	_U_RECT(IN LPRECT lpRect) throw() : m_lpRect(lpRect)
	{
	}
	_U_RECT(IN RECT& rc) throw() : m_lpRect(&rc)
	{
	}
	LPRECT m_lpRect;
};

class _U_MENUorID
{
public:
	_U_MENUorID(IN HMENU hMenu) throw() : m_hMenu(hMenu)
	{
	}
	_U_MENUorID(IN UINT nID) throw() : m_hMenu((HMENU)(UINT_PTR)nID)
	{
	}
	HMENU m_hMenu;
};

// _Window

class _Window
{
public:
	HWND m_hWnd;

	static RECT m_rcDefault;

	explicit _Window(IN HWND hWnd = NULL) throw() : m_hWnd(hWnd)
	{
	}

	_Window& operator=(IN HWND hWnd) throw()
	{
		m_hWnd = hWnd;
		return *this;
	}

	static LPCWSTR GetWndClassName() throw()
	{
		return NULL;
	}

	void Attach(IN HWND hWndNew) throw()
	{
		assert( m_hWnd == NULL );
		assert( (hWndNew == NULL) || ::IsWindow(hWndNew) );
		m_hWnd = hWndNew;
	}

	HWND Detach() throw()
	{
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		return hWnd;
	}

	HWND Create(
		IN LPCWSTR lpstrWndClass,
		IN HWND hWndParent,
		IN _U_RECT rect = NULL,
		IN LPCWSTR szWindowName = NULL,
		IN DWORD dwStyle = 0,
		IN DWORD dwExStyle = 0,
		IN _U_MENUorID MenuOrID = 0U,
		IN LPVOID lpCreateParam = NULL) throw()
	{
		assert( m_hWnd == NULL );
		if( rect.m_lpRect == NULL )
			rect.m_lpRect = &m_rcDefault;
		m_hWnd = ::CreateWindowExW(dwExStyle, lpstrWndClass, szWindowName,
			dwStyle, rect.m_lpRect->left, rect.m_lpRect->top, rect.m_lpRect->right - rect.m_lpRect->left,
			rect.m_lpRect->bottom - rect.m_lpRect->top, hWndParent, MenuOrID.m_hMenu,
			g_module.GetModuleInstance(), lpCreateParam);
		return m_hWnd;
	}

	BOOL DestroyWindow() throw()
	{
		assert( ::IsWindow(m_hWnd) );

		if( !::DestroyWindow(m_hWnd) )
			return FALSE;

		m_hWnd = NULL;
		return TRUE;
	}

// Attributes

	operator HWND() const throw()
	{
		return m_hWnd;
	}

	DWORD GetStyle() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return (DWORD)::GetWindowLongPtrW(m_hWnd, GWL_STYLE);
	}

	DWORD GetExStyle() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return (DWORD)::GetWindowLongPtrW(m_hWnd, GWL_EXSTYLE);
	}

	LONG_PTR GetWindowLongPtr(IN int nIndex) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetWindowLongPtrW(m_hWnd, nIndex);
	}

	LONG_PTR SetWindowLongPtr(
		IN int nIndex,
		IN LONG_PTR dwNewLong) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetWindowLongPtrW(m_hWnd, nIndex, dwNewLong);
	}

	WORD GetWindowWord(IN int nIndex) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetWindowWord(m_hWnd, nIndex);
	}

	WORD SetWindowWord(
		IN int nIndex,
		IN WORD wNewWord) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetWindowWord(m_hWnd, nIndex, wNewWord);
	}

// Message Functions

	LRESULT SendMessage(
		IN UINT message,
		IN WPARAM wParam = 0,
		IN LPARAM lParam = 0) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SendMessageW(m_hWnd, message, wParam, lParam);
	}

	BOOL PostMessage(
		IN UINT message,
		IN WPARAM wParam = 0,
		IN LPARAM lParam = 0) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::PostMessageW(m_hWnd, message, wParam, lParam);
	}

	BOOL SendNotifyMessage(
		IN UINT message,
		IN WPARAM wParam = 0,
		IN LPARAM lParam = 0) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SendNotifyMessageW(m_hWnd, message, wParam, lParam);
	}

// Window Text Functions

	BOOL SetWindowText(IN LPCWSTR lpszString) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetWindowTextW(m_hWnd, lpszString);
	}

	int GetWindowText(
		OUT LPWSTR lpszStringBuf,
		IN int nMaxCount) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetWindowTextW(m_hWnd, lpszStringBuf, nMaxCount);
	}

	int GetWindowTextLength() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetWindowTextLengthW(m_hWnd);
	}

// Font Functions

	void SetFont(
		IN HFONT hFont,
		IN BOOL bRedraw = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		::SendMessageW(m_hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(bRedraw, 0));
	}

	HFONT GetFont() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return (HFONT)::SendMessageW(m_hWnd, WM_GETFONT, 0, 0);
	}

// Menu Functions (non-child windows only)

	HMENU GetMenu() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return (HMENU)::GetMenu(m_hWnd);
	}

	BOOL SetMenu(IN HMENU hMenu) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetMenu(m_hWnd, hMenu);
	}

	BOOL DrawMenuBar() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::DrawMenuBar(m_hWnd);
	}

	HMENU GetSystemMenu(IN BOOL bRevert) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return (HMENU)::GetSystemMenu(m_hWnd, bRevert);
	}

	BOOL HiliteMenuItem(
		IN HMENU hMenu,
		IN UINT uItemHilite,
		IN UINT uHilite) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::HiliteMenuItem(m_hWnd, hMenu, uItemHilite, uHilite);
	}

// Window Size and Position Functions

	BOOL IsIconic() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::IsIconic(m_hWnd);
	}

	BOOL IsZoomed() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::IsZoomed(m_hWnd);
	}

	BOOL MoveWindow(
		IN int x,
		IN int y,
		IN int nWidth,
		IN int nHeight,
		IN BOOL bRepaint = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::MoveWindow(m_hWnd, x, y, nWidth, nHeight, bRepaint);
	}

	BOOL MoveWindow(
		IN LPCRECT lpRect,
		IN BOOL bRepaint = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::MoveWindow(m_hWnd, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bRepaint);
	}

	BOOL SetWindowPos(
		IN HWND hWndInsertAfter,
		IN int x,
		IN int y,
		IN int cx,
		IN int cy,
		IN UINT nFlags) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, x, y, cx, cy, nFlags);
	}

	BOOL SetWindowPos(
		IN HWND hWndInsertAfter,
		IN LPCRECT lpRect,
		IN UINT nFlags) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, nFlags);
	}

	UINT ArrangeIconicWindows() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ArrangeIconicWindows(m_hWnd);
	}

	BOOL BringWindowToTop() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::BringWindowToTop(m_hWnd);
	}

	BOOL GetWindowRect(OUT LPRECT lpRect) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetWindowRect(m_hWnd, lpRect);
	}

	BOOL GetClientRect(OUT LPRECT lpRect) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetClientRect(m_hWnd, lpRect);
	}

	BOOL GetWindowPlacement(OUT WINDOWPLACEMENT FAR* lpwndpl) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetWindowPlacement(m_hWnd, lpwndpl);
	}

	BOOL SetWindowPlacement(IN const WINDOWPLACEMENT FAR* lpwndpl) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetWindowPlacement(m_hWnd, lpwndpl);
	}

// Coordinate Mapping Functions

	BOOL ClientToScreen(INOUT LPPOINT lpPoint) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ClientToScreen(m_hWnd, lpPoint);
	}

	BOOL ClientToScreen(INOUT LPRECT lpRect) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		if( !::ClientToScreen(m_hWnd, (LPPOINT)lpRect) )
			return FALSE;
		return ::ClientToScreen(m_hWnd, ((LPPOINT)lpRect) + 1);
	}

	BOOL ScreenToClient(INOUT LPPOINT lpPoint) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ScreenToClient(m_hWnd, lpPoint);
	}

	BOOL ScreenToClient(INOUT LPRECT lpRect) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		if( !::ScreenToClient(m_hWnd, (LPPOINT)lpRect) )
			return FALSE;
		return ::ScreenToClient(m_hWnd, ((LPPOINT)lpRect) + 1);
	}

	int MapWindowPoints(
		IN HWND hWndTo,
		INOUT LPPOINT lpPoint,
		IN UINT nCount) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::MapWindowPoints(m_hWnd, hWndTo, lpPoint, nCount);
	}

	int MapWindowPoints(
		IN HWND hWndTo,
		INOUT LPRECT lpRect) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::MapWindowPoints(m_hWnd, hWndTo, (LPPOINT)lpRect, 2);
	}

// Update and Painting Functions

	HDC BeginPaint(OUT LPPAINTSTRUCT lpPaint) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::BeginPaint(m_hWnd, lpPaint);
	}

	void EndPaint(IN LPPAINTSTRUCT lpPaint) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		::EndPaint(m_hWnd, lpPaint);
	}

	HDC GetDC() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetDC(m_hWnd);
	}

	HDC GetWindowDC() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetWindowDC(m_hWnd);
	}

	int ReleaseDC(IN HDC hDC) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ReleaseDC(m_hWnd, hDC);
	}

	void Print(
		IN HDC hDC,
		IN DWORD dwFlags) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		::SendMessageW(m_hWnd, WM_PRINT, (WPARAM)hDC, dwFlags);
	}

	void PrintClient(
		IN HDC hDC,
		IN DWORD dwFlags) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		::SendMessageW(m_hWnd, WM_PRINTCLIENT, (WPARAM)hDC, dwFlags);
	}

	BOOL UpdateWindow() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::UpdateWindow(m_hWnd);
	}

	void SetRedraw(IN BOOL bRedraw = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		::SendMessageW(m_hWnd, WM_SETREDRAW, (WPARAM)bRedraw, 0);
	}

	BOOL GetUpdateRect(
		IN LPRECT lpRect,
		IN BOOL bErase = FALSE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetUpdateRect(m_hWnd, lpRect, bErase);
	}

	int GetUpdateRgn(
		IN HRGN hRgn,
		IN BOOL bErase = FALSE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetUpdateRgn(m_hWnd, hRgn, bErase);
	}

	BOOL Invalidate(IN BOOL bErase = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::InvalidateRect(m_hWnd, NULL, bErase);
	}

	BOOL InvalidateRect(
		IN LPCRECT lpRect,
		IN BOOL bErase = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd)  );
		return ::InvalidateRect(m_hWnd, lpRect, bErase);
	}

	BOOL ValidateRect(IN LPCRECT lpRect) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ValidateRect(m_hWnd, lpRect);
	}

	void InvalidateRgn(
		IN HRGN hRgn,
		IN BOOL bErase = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		::InvalidateRgn(m_hWnd, hRgn, bErase);
	}

	BOOL ValidateRgn(IN HRGN hRgn) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ValidateRgn(m_hWnd, hRgn);
	}

	BOOL ShowWindow(IN int nCmdShow) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ShowWindow(m_hWnd, nCmdShow);
	}

	BOOL IsWindowVisible() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::IsWindowVisible(m_hWnd);
	}

	BOOL ShowOwnedPopups(IN BOOL bShow = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ShowOwnedPopups(m_hWnd, bShow);
	}

	HDC GetDCEx(
		IN HRGN hRgnClip,
		IN DWORD flags) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetDCEx(m_hWnd, hRgnClip, flags);
	}

	BOOL LockWindowUpdate(IN BOOL bLock = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::LockWindowUpdate(bLock ? m_hWnd : NULL);
	}

	BOOL RedrawWindow(
		IN LPCRECT lpRectUpdate = NULL,
		IN HRGN hRgnUpdate = NULL,
		IN UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::RedrawWindow(m_hWnd, lpRectUpdate, hRgnUpdate, flags);
	}

// Timer Functions

	UINT_PTR SetTimer(
		IN UINT_PTR nIDEvent,
		IN UINT nElapse,
		IN void (CALLBACK* lpfnTimer)(HWND, UINT, UINT_PTR, DWORD) = NULL) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetTimer(m_hWnd, nIDEvent, nElapse, (TIMERPROC)lpfnTimer);
	}

	BOOL KillTimer(IN UINT_PTR nIDEvent) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::KillTimer(m_hWnd, nIDEvent);
	}

// Window State Functions

	BOOL IsWindowEnabled() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::IsWindowEnabled(m_hWnd);
	}

	BOOL EnableWindow(IN BOOL bEnable = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::EnableWindow(m_hWnd, bEnable);
	}

	HWND SetActiveWindow() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetActiveWindow(m_hWnd);
	}

	HWND SetCapture() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetCapture(m_hWnd);
	}

	HWND SetFocus() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetFocus(m_hWnd);
	}

// Dialog-Box Item Functions

	BOOL CheckDlgButton(
		IN int nIDButton,
		IN UINT nCheck) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::CheckDlgButton(m_hWnd, nIDButton, nCheck);
	}

	BOOL CheckRadioButton(
		IN int nIDFirstButton,
		IN int nIDLastButton,
		IN int nIDCheckButton) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::CheckRadioButton(m_hWnd, nIDFirstButton, nIDLastButton, nIDCheckButton);
	}

	int DlgDirList(
		INOUT LPWSTR lpPathSpec,
		IN int nIDListBox,
		IN int nIDStaticPath,
		IN UINT nFileType) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::DlgDirListW(m_hWnd, lpPathSpec, nIDListBox, nIDStaticPath, nFileType);
	}

	int DlgDirListComboBox(
		INOUT LPWSTR lpPathSpec,
		IN int nIDComboBox,
		IN int nIDStaticPath,
		IN UINT nFileType) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::DlgDirListComboBoxW(m_hWnd, lpPathSpec, nIDComboBox, nIDStaticPath, nFileType);
	}

	BOOL DlgDirSelect(
		OUT LPWSTR lpString,
		IN int nCount,
		IN int nIDListBox) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::DlgDirSelectExW(m_hWnd, lpString, nCount, nIDListBox);
	}

	BOOL DlgDirSelectComboBox(
		OUT LPWSTR lpString,
		IN int nCount,
		IN int nIDComboBox) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::DlgDirSelectComboBoxExW(m_hWnd, lpString, nCount, nIDComboBox);
	}

	UINT GetDlgItemInt(
		IN int nID,
		OUT BOOL* lpTrans = NULL,
		IN BOOL bSigned = TRUE) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetDlgItemInt(m_hWnd, nID, lpTrans, bSigned);
	}

	UINT GetDlgItemText(
		IN int nID,
		OUT LPWSTR lpStr,
		IN int nMaxCount) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetDlgItemTextW(m_hWnd, nID, lpStr, nMaxCount);
	}

	_Window GetNextDlgGroupItem(
		IN HWND hWndCtl,
		IN BOOL bPrevious = FALSE) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return _Window(::GetNextDlgGroupItem(m_hWnd, hWndCtl, bPrevious));
	}

	_Window GetNextDlgTabItem(
		IN HWND hWndCtl,
		IN BOOL bPrevious = FALSE) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return _Window(::GetNextDlgTabItem(m_hWnd, hWndCtl, bPrevious));
	}

	UINT IsDlgButtonChecked(IN int nIDButton) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::IsDlgButtonChecked(m_hWnd, nIDButton);
	}

	LRESULT SendDlgItemMessage(
		IN int nID,
		IN UINT message,
		IN WPARAM wParam = 0,
		IN LPARAM lParam = 0) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SendDlgItemMessageW(m_hWnd, nID, message, wParam, lParam);
	}

	BOOL SetDlgItemInt(
		IN int nID,
		IN UINT nValue,
		IN BOOL bSigned = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetDlgItemInt(m_hWnd, nID, nValue, bSigned);
	}

	BOOL SetDlgItemText(
		IN int nID,
		IN LPCWSTR lpszString) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetDlgItemTextW(m_hWnd, nID, lpszString);
	}

// Scrolling Functions

	int GetScrollPos(IN int nBar) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetScrollPos(m_hWnd, nBar);
	}

	BOOL GetScrollRange(
		IN int nBar,
		OUT LPINT lpMinPos,
		OUT LPINT lpMaxPos) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetScrollRange(m_hWnd, nBar, lpMinPos, lpMaxPos);
	}

	BOOL ScrollWindow(
		IN int xAmount,
		IN int yAmount,
		IN LPCRECT lpRect = NULL,
		IN LPCRECT lpClipRect = NULL) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ScrollWindow(m_hWnd, xAmount, yAmount, lpRect, lpClipRect);
	}

	int ScrollWindowEx(
		IN int dx,
		IN int dy,
		IN LPCRECT lpRectScroll,
		IN LPCRECT lpRectClip,
		IN HRGN hRgnUpdate,
		IN LPRECT lpRectUpdate,
		IN UINT uFlags) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ScrollWindowEx(m_hWnd, dx, dy, lpRectScroll, lpRectClip, hRgnUpdate, lpRectUpdate, uFlags);
	}

	int ScrollWindowEx(
		IN int dx,
		IN int dy,
		IN UINT uFlags,
		IN LPCRECT lpRectScroll = NULL,
		IN LPCRECT lpRectClip = NULL,
		IN HRGN hRgnUpdate = NULL,
		IN LPRECT lpRectUpdate = NULL) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ScrollWindowEx(m_hWnd, dx, dy, lpRectScroll, lpRectClip, hRgnUpdate, lpRectUpdate, uFlags);
	}

	int SetScrollPos(
		IN int nBar,
		IN int nPos,
		IN BOOL bRedraw = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetScrollPos(m_hWnd, nBar, nPos, bRedraw);
	}

	BOOL SetScrollRange(
		IN int nBar,
		IN int nMinPos,
		IN int nMaxPos,
		IN BOOL bRedraw = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetScrollRange(m_hWnd, nBar, nMinPos, nMaxPos, bRedraw);
	}

	BOOL ShowScrollBar(
		IN int nBar,
		IN BOOL bShow = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ShowScrollBar(m_hWnd, nBar, bShow);
	}

	BOOL EnableScrollBar(
		IN UINT uSBFlags,
		IN UINT uArrowFlags = ESB_ENABLE_BOTH) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::EnableScrollBar(m_hWnd, uSBFlags, uArrowFlags);
	}

// Window Access Functions

	_Window ChildWindowFromPoint(IN POINT point) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return _Window(::ChildWindowFromPoint(m_hWnd, point));
	}

	_Window ChildWindowFromPointEx(
		IN POINT point,
		IN UINT uFlags) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return _Window(::ChildWindowFromPointEx(m_hWnd, point, uFlags));
	}

	_Window GetTopWindow() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return _Window(::GetTopWindow(m_hWnd));
	}

	_Window GetWindow(IN UINT nCmd) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return _Window(::GetWindow(m_hWnd, nCmd));
	}

	_Window GetLastActivePopup() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return _Window(::GetLastActivePopup(m_hWnd));
	}

	BOOL IsChild(IN HWND hWnd) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::IsChild(m_hWnd, hWnd);
	}

	_Window GetParent() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return _Window(::GetParent(m_hWnd));
	}

	_Window SetParent(IN HWND hWndNewParent) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return _Window(::SetParent(m_hWnd, hWndNewParent));
 	}

// Window Tree Access

	int GetDlgCtrlID() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetDlgCtrlID(m_hWnd);
	}

	int SetDlgCtrlID(IN int nID) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return (int)::SetWindowLongPtrW(m_hWnd, GWL_ID, nID);
	}

	_Window GetDlgItem(IN int nID) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return _Window(::GetDlgItem(m_hWnd, nID));
	}

// Alert Functions

	BOOL FlashWindow(IN BOOL bInvert) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::FlashWindow(m_hWnd, bInvert);
	}

	int MessageBox(
		IN LPCWSTR lpszText,
		IN LPCWSTR lpszCaption = L"",
		IN UINT nType = MB_OK) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::MessageBoxW(m_hWnd, lpszText, lpszCaption, nType);
	}

// Clipboard Functions

	BOOL ChangeClipboardChain(IN HWND hWndNewNext) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ChangeClipboardChain(m_hWnd, hWndNewNext);
	}

	HWND SetClipboardViewer() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetClipboardViewer(m_hWnd);
	}

	BOOL OpenClipboard() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::OpenClipboard(m_hWnd);
	}

// Caret Functions

	BOOL CreateCaret(IN HBITMAP hBitmap) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::CreateCaret(m_hWnd, hBitmap, 0, 0);
	}

	BOOL CreateSolidCaret(IN int nWidth, IN int nHeight) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::CreateCaret(m_hWnd, (HBITMAP)0, nWidth, nHeight);
	}

	BOOL CreateGrayCaret(IN int nWidth, IN int nHeight) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::CreateCaret(m_hWnd, (HBITMAP)1, nWidth, nHeight);
	}

	BOOL HideCaret() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::HideCaret(m_hWnd);
	}

	BOOL ShowCaret() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ShowCaret(m_hWnd);
	}

// Drag-Drop Functions

	void DragAcceptFiles(IN BOOL bAccept = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		::DragAcceptFiles(m_hWnd, bAccept);
	}

// Icon Functions

	HICON SetIcon(
		IN HICON hIcon,
		IN BOOL bBigIcon = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return (HICON)::SendMessageW(m_hWnd, WM_SETICON, bBigIcon, (LPARAM)hIcon);
	}

	HICON GetIcon(IN BOOL bBigIcon = TRUE) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return (HICON)::SendMessageW(m_hWnd, WM_GETICON, bBigIcon, 0);
	}

// Help Functions

	BOOL WinHelp(
		IN LPCWSTR lpszHelp,
		IN UINT nCmd = HELP_CONTEXT,
		IN ULONG_PTR dwData = 0) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::WinHelpW(m_hWnd, lpszHelp, nCmd, dwData);
	}

	BOOL SetWindowContextHelpId(IN DWORD dwContextHelpId) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetWindowContextHelpId(m_hWnd, dwContextHelpId);
	}

	DWORD GetWindowContextHelpId() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetWindowContextHelpId(m_hWnd);
	}

// Hot Key Functions

	int SetHotKey(
		IN WORD wVirtualKeyCode,
		IN WORD wModifiers) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return (int)::SendMessageW(m_hWnd, WM_SETHOTKEY, MAKEWORD(wVirtualKeyCode, wModifiers), 0);
	}

	DWORD GetHotKey() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return (DWORD)::SendMessageW(m_hWnd, WM_GETHOTKEY, 0, 0);
	}

// Misc. Operations

//N new

	BOOL GetScrollInfo(
		IN int nBar,
		INOUT LPSCROLLINFO lpScrollInfo) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetScrollInfo(m_hWnd, nBar, lpScrollInfo);
	}

	int SetScrollInfo(
		IN int nBar,
		IN LPSCROLLINFO lpScrollInfo,
		IN BOOL bRedraw = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetScrollInfo(m_hWnd, nBar, lpScrollInfo, bRedraw);
	}

	BOOL IsDialogMessage(IN LPMSG lpMsg) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::IsDialogMessageW(m_hWnd, lpMsg);
	}

	void NextDlgCtrl() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		::SendMessageW(m_hWnd, WM_NEXTDLGCTL, 0, 0L);
	}

	void PrevDlgCtrl() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		::SendMessageW(m_hWnd, WM_NEXTDLGCTL, 1, 0L);
	}

	void GotoDlgCtrl(IN HWND hWndCtrl) const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		::SendMessageW(m_hWnd, WM_NEXTDLGCTL, (WPARAM)hWndCtrl, 1L);
	}

	BOOL ResizeClient(
		IN int nWidth,
		IN int nHeight,
		IN BOOL bRedraw = TRUE) throw()
	{
		assert( ::IsWindow(m_hWnd) );

		RECT rcWnd;
		if( !GetClientRect(&rcWnd) )
			return FALSE;

		if( nWidth != -1 )
			rcWnd.right = nWidth;
		if( nHeight != -1 )
			rcWnd.bottom = nHeight;

		if( !::AdjustWindowRectEx(&rcWnd, GetStyle(), (!(GetStyle() & WS_CHILD) && (GetMenu() != NULL)), GetExStyle()) )
			return FALSE;

		UINT uFlags = SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE;
		if( !bRedraw )
			uFlags |= SWP_NOREDRAW;

		return SetWindowPos(NULL, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top, uFlags);
	}

	int GetWindowRgn(OUT HRGN hRgn) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetWindowRgn(m_hWnd, hRgn);
	}

	int SetWindowRgn(
		IN HRGN hRgn,
		IN BOOL bRedraw = FALSE) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::SetWindowRgn(m_hWnd, hRgn, bRedraw);
	}

	HDWP DeferWindowPos(
		IN HDWP hWinPosInfo,
		IN HWND hWndInsertAfter,
		IN int x,
		IN int y,
		IN int cx,
		IN int cy,
		IN UINT uFlags) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::DeferWindowPos(hWinPosInfo, m_hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
	}

	DWORD GetWindowThreadID() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::GetWindowThreadProcessId(m_hWnd, NULL);
	}

	DWORD GetWindowProcessID() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		DWORD dwProcessID;
		::GetWindowThreadProcessId(m_hWnd, &dwProcessID);
		return dwProcessID;
	}

	BOOL IsWindow() const throw()
	{
		return ::IsWindow(m_hWnd);
	}

	BOOL IsWindowUnicode() const throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::IsWindowUnicode(m_hWnd);
	}

	BOOL IsParentDialog() throw()
	{
		assert( ::IsWindow(m_hWnd) );
		WCHAR szBuf[8]; // "#32770" + NUL character
		if( ::GetClassNameW(GetParent(), szBuf, sizeof(szBuf) / sizeof(szBuf[0])) == 0 )
			return FALSE;
		return ::lstrcmpW(szBuf, L"#32770") == 0;
	}

	BOOL ShowWindowAsync(IN int nCmdShow) throw()
	{
		assert( ::IsWindow(m_hWnd) );
		return ::ShowWindowAsync(m_hWnd, nCmdShow);
	}

	_Window GetDescendantWindow(IN int nID) const throw()
	{
		assert( ::IsWindow(m_hWnd) );

		// GetDlgItem recursive (return first found)
		// breadth-first for 1 level, then depth-first for next level

		// use GetDlgItem since it is a fast USER function
		HWND hWndChild, hWndTmp;
		if( (hWndChild = ::GetDlgItem(m_hWnd, nID)) != NULL ) {
			if( ::GetTopWindow(hWndChild) != NULL ) {
				// children with the same ID as their parent have priority
				_Window wnd(hWndChild);
				hWndTmp = wnd.GetDescendantWindow(nID);
				if( hWndTmp != NULL )
					return _Window(hWndTmp);
			}
			return _Window(hWndChild);
		}

		// walk each child
		for( hWndChild = ::GetTopWindow(m_hWnd); hWndChild != NULL;
			hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT) ) {
			_Window wnd(hWndChild);
			hWndTmp = wnd.GetDescendantWindow(nID);
			if( hWndTmp != NULL )
				return _Window(hWndTmp);
		}

		return _Window(NULL);    // not found
	}

	void SendMessageToDescendants(
		IN UINT message,
		IN WPARAM wParam = 0,
		IN LPARAM lParam = 0,
		IN BOOL bDeep = TRUE) throw()
	{
		for( HWND hWndChild = ::GetTopWindow(m_hWnd); hWndChild != NULL;
			hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT) ) {
			::SendMessageW(hWndChild, message, wParam, lParam);

			if( bDeep && ::GetTopWindow(hWndChild) != NULL ) {
				// send to child windows after parent
				_Window wnd(hWndChild);
				wnd.SendMessageToDescendants(message, wParam, lParam, bDeep);
			}
		} //end for
	}

	BOOL CenterWindow(IN HWND hWndCenter = NULL) throw()
	{
		assert( ::IsWindow(m_hWnd) );

		// determine owner window to center against
		DWORD dwStyle = GetStyle();
		if( hWndCenter == NULL ) {
			if( dwStyle & WS_CHILD )
				hWndCenter = ::GetParent(m_hWnd);
			else
				hWndCenter = ::GetWindow(m_hWnd, GW_OWNER);
		}

		// get coordinates of the window relative to its parent
		RECT rcDlg;
		::GetWindowRect(m_hWnd, &rcDlg);  //no check
		RECT rcArea;
		RECT rcCenter;
		HWND hWndParent;
		if( !(dwStyle & WS_CHILD) ) {
			// don't center against invisible or minimized windows
			if( hWndCenter != NULL ) {
				DWORD dwStyleCenter = (DWORD)::GetWindowLongPtrW(hWndCenter, GWL_STYLE);
				if( !(dwStyleCenter & WS_VISIBLE) || (dwStyleCenter & WS_MINIMIZE) )
					hWndCenter = NULL;
			}

			// center within screen coordinates
#if WINVER < 0x0500
			BOOL bResult = ::SystemParametersInfoW(SPI_GETWORKAREA, NULL, &rcArea, NULL);
			if( !bResult )
				return FALSE;
#else
			HMONITOR hMonitor = NULL;
			if( hWndCenter != NULL ) {
				hMonitor = ::MonitorFromWindow(hWndCenter, MONITOR_DEFAULTTONEAREST);
			}
			else {
				hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
			}
			if( hMonitor == NULL )
				return FALSE;

			MONITORINFO minfo;
			minfo.cbSize = sizeof(MONITORINFO);
			BOOL bResult = ::GetMonitorInfoW(hMonitor, &minfo);
			if( !bResult )
				return FALSE;

			rcArea = minfo.rcWork;
#endif
			if( hWndCenter == NULL )
				rcCenter = rcArea;
			else
				::GetWindowRect(hWndCenter, &rcCenter);  //no check
		}
		else {
			// center within parent client coordinates
			hWndParent = ::GetParent(m_hWnd);
			assert( ::IsWindow(hWndParent) );

			::GetClientRect(hWndParent, &rcArea);  //no check
			assert( ::IsWindow(hWndCenter) );
			::GetClientRect(hWndCenter, &rcCenter);  //no check
			::MapWindowPoints(hWndCenter, hWndParent, (POINT*)&rcCenter, 2);
		}

		int DlgWidth = rcDlg.right - rcDlg.left;
		int DlgHeight = rcDlg.bottom - rcDlg.top;

		// find dialog's upper left based on rcCenter
		int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
		int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

		// if the dialog is outside the screen, move it inside
		if( xLeft + DlgWidth > rcArea.right )
			xLeft = rcArea.right - DlgWidth;
		if( xLeft < rcArea.left )
			xLeft = rcArea.left;

		if( yTop + DlgHeight > rcArea.bottom )
			yTop = rcArea.bottom - DlgHeight;
		if( yTop < rcArea.top )
			yTop = rcArea.top;

		// map screen coordinates to child coordinates
		return ::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	BOOL ModifyStyle(
		IN DWORD dwRemove,
		IN DWORD dwAdd,
		IN UINT nFlags = 0) throw()
	{
		assert( ::IsWindow(m_hWnd) );

		DWORD dwStyle = (DWORD)::GetWindowLongPtrW(m_hWnd, GWL_STYLE);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if( dwStyle == dwNewStyle )
			return FALSE;

		::SetWindowLongPtrW(m_hWnd, GWL_STYLE, dwNewStyle);
		if( nFlags != 0 ) {
			::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}

		return TRUE;
	}

	BOOL ModifyStyleEx(
		IN DWORD dwRemove,
		IN DWORD dwAdd,
		IN UINT nFlags = 0) throw()
	{
		assert( ::IsWindow(m_hWnd) );

		DWORD dwStyle = (DWORD)::GetWindowLongPtrW(m_hWnd, GWL_EXSTYLE);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if( dwStyle == dwNewStyle )
			return FALSE;

		::SetWindowLongPtrW(m_hWnd, GWL_EXSTYLE, dwNewStyle);
		if( nFlags != 0 ) {
			::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}

		return TRUE;
	}

	_Window GetTopLevelParent() const throw()
	{
		assert( ::IsWindow(m_hWnd) );

		HWND hWndParent = m_hWnd;
		HWND hWndTmp;
		while( (hWndTmp = ::GetParent(hWndParent)) != NULL )
			hWndParent = hWndTmp;

		return _Window(hWndParent);
	}

	_Window GetTopLevelWindow() const throw()
	{
		assert( ::IsWindow(m_hWnd) );

		HWND hWndParent;
		HWND hWndTmp = m_hWnd;

		do {
			hWndParent = hWndTmp;
			hWndTmp = ((DWORD)::GetWindowLongPtrW(hWndParent, GWL_STYLE) & WS_CHILD) ? ::GetParent(hWndParent) : ::GetWindow(hWndParent, GW_OWNER);
		} while( hWndTmp != NULL );

		return _Window(hWndParent);
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__WINDOW_H__
////////////////////////////////////////////////////////////////////////////////
