/*
** Copyright (c) 2020, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

// Internal header

////////////////////////////////////////////////////////////////////////////////
#ifndef __INTERNAL_TXT_STREAM_H__
#define __INTERNAL_TXT_STREAM_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//array

// windows
DECLARE_STATIC_CONST_ARRAY(g_windows_crlf_a, CharA)
DECLARE_STATIC_CONST_ARRAY(g_windows_crlf_h, CharH)
DECLARE_STATIC_CONST_ARRAY(g_windows_crlf_l, CharL)
// unix
DECLARE_STATIC_CONST_ARRAY(g_unix_crlf_a, CharA)
DECLARE_STATIC_CONST_ARRAY(g_unix_crlf_h, CharH)
DECLARE_STATIC_CONST_ARRAY(g_unix_crlf_l, CharL)
// mac
DECLARE_STATIC_CONST_ARRAY(g_mac_crlf_a, CharA)
DECLARE_STATIC_CONST_ARRAY(g_mac_crlf_h, CharH)
DECLARE_STATIC_CONST_ARRAY(g_mac_crlf_l, CharL)

// separator set
DECLARE_STATIC_CONST_ARRAY(g_separator_set_a, CharA)
DECLARE_STATIC_CONST_ARRAY(g_separator_set_h, CharH)
DECLARE_STATIC_CONST_ARRAY(g_separator_set_l, CharL)

//functions

template <typename Tchar>
inline void initialize_separator_set(ConstArray<Tchar>& ca) noexcept;

#define DECLARE_INITIALIZE_SEPARATOR_SET_FUNCTION(char_type, g_type)  \
template <> inline void initialize_separator_set<char_type>(ConstArray<char_type>& ca) noexcept  \
{ if ( ca.IsNull() ) ca = ConstArray<char_type>(g_type::GetAddress(), g_type::GetCount()); }

DECLARE_INITIALIZE_SEPARATOR_SET_FUNCTION(CharA, g_separator_set_a)
DECLARE_INITIALIZE_SEPARATOR_SET_FUNCTION(CharH, g_separator_set_h)
DECLARE_INITIALIZE_SEPARATOR_SET_FUNCTION(CharL, g_separator_set_l)

template <typename Tchar>
inline bool check_separator_set(const Tchar& ch, const ConstArray<Tchar>& carr) throw()
{
	auto iter(carr.GetBegin());
	for( ; iter != carr.GetEnd(); iter.MoveNext() ) {
		if( iter.get_Value() == ch )
			return true;
	}
	return ch == '\r' || ch == '\n';
}

//classes

// _TextStreamBase

class _TextStreamBase
{
protected:
	enum { BUFFER_SIZE = 4 * 1024 };  // 4K

	//flags
	enum {
		MASK_TEST_READING = 0x00000001,
		MASK_CAN_READ = 0x00000002,
		MASK_TEST_WRITING = 0x00000004,
		MASK_CAN_WRITE = 0x00000008,
		MASK_VALID = 0x00000010,  //buffer valid
		MASK_DIRTY = 0x00000020
	};

protected:
	//string mode
	enum {
		MODE_ALL = 0,
		MODE_SEPARATOR,
		MODE_CRLF,
		MODE_MAX
	};

protected:
	_TextStreamBase() noexcept : m_iTotal(0), m_uFlags(0), m_iBomType(_BOMTypes::None),
								m_iCRLFStyle(_CRLFStyles::Windows), m_bSwap(false)
	{
		assert( sizeof(byte) == 1 );
	}
	~_TextStreamBase() noexcept
	{
	}

	void set_stream(const RefPtr<_IByteStream>& rs) noexcept
	{
		m_refStream = rs;
	}

	void set_separator_set_a(const ConstArray<CharA>& ca) noexcept
	{
		m_carrSeparatorSetA = ca;
	}
	void set_separator_set_h(const ConstArray<CharH>& ca) noexcept
	{
		m_carrSeparatorSetH = ca;
	}
	void set_separator_set_l(const ConstArray<CharL>& ca) noexcept
	{
		m_carrSeparatorSetL = ca;
	}

	//reset
	void reset_buffer() throw()
	{
		m_iTotal = 0;
		m_uFlags = 0;
		m_iBomType = _BOMTypes::None;
		m_bSwap = false;
	}

	//set BOM type
	void set_BOM_type(int iType) throw()
	{
		m_iBomType = iType;
		m_bSwap = false;
		switch(iType) {
		case _BOMTypes::UTF16LE:
		case _BOMTypes::UTF32LE:
			m_bSwap = ByteOrderHelper::IsBigEndianHost();
			break;
		case _BOMTypes::UTF16BE:
		case _BOMTypes::UTF32BE:
			m_bSwap = !ByteOrderHelper::IsBigEndianHost();
			break;
		default:
			break;
		}
	}

	//swap
	template <typename Tchar>
	static void process_swap(Tchar* p, uintptr uCount) throw()
	{
		for( uintptr i = 0; i < uCount; i ++ ) {
			*p = ByteOrderHelper::Swap(*p);
			p ++;
		}
	}

	//process dirty flag
	CallResult process_dirty_flag() throw()
	{
		CallResult cr;
		if( m_uFlags & MASK_DIRTY ) {
			assert( m_uFlags & MASK_VALID );
			//begin
			int64 iCurPos, iNewPos;
			cr = m_refStream.Deref().Seek(IO_SEEK_CURRENT, 0, iCurPos);
			if( cr.IsFailed() )
				return cr;
			//move
			int64 iMove;
			if( !(m_uFlags & MASK_CAN_READ) )
				iMove = (int64)m_uDirtyBegin;
			else
				iMove = -(int64)(m_uValidSize0 - m_uDirtyBegin);
			cr = m_refStream.Deref().Seek(IO_SEEK_CURRENT, iMove, iNewPos);
			if( cr.IsFailed() )
				return cr;
			//write
			uint uSize = m_uDirtyEnd - m_uDirtyBegin + 1;
			uint uWritten;
			cr = m_refStream.Deref().Write((uintptr)(FixedArrayHelper::GetInternalPointer(m_buffer) + m_uDirtyBegin), uSize, uWritten);
			if( cr.IsFailed() ) {
				//restore
				CallResult crSeek(m_refStream.Deref().Seek(IO_SEEK_BEGIN, iCurPos, iNewPos));
				assert( crSeek.IsSucceeded() );
				return cr;
			}
			if( uWritten != uSize ) {
				//restore
				CallResult crSeek(m_refStream.Deref().Seek(IO_SEEK_BEGIN, iCurPos, iNewPos));
				assert( crSeek.IsSucceeded() );
				//error
				cr.SetResult(SystemCallResults::DiskFull);
				return cr;
			}
			//restore
			if( m_uValidSize != m_uValidSize0 ) {
				assert( m_uValidSize > m_uValidSize0 );
				if( m_uFlags & MASK_CAN_READ ) {
					int64 iDelta = (int64)(m_uValidSize - m_uValidSize0);
					iCurPos += iDelta;
					m_iTotal += iDelta;
				}
				m_uValidSize0 = m_uValidSize;
			}
			cr = m_refStream.Deref().Seek(IO_SEEK_BEGIN, iCurPos, iNewPos);
			assert( cr.IsSucceeded() );
			//flag
			m_uFlags &= ~MASK_DIRTY;
		}
		return cr;
	}
	//outstanding work
	void do_outstanding_work() throw()
	{
		if( is_valid() ) {
			//process dirty flag
			CallResult cr(process_dirty_flag());
			assert( cr.IsSucceeded() );
		}
	}

	//pre-read
	CallResult _pre_read_buffer() throw()
	{
		CallResult cr;
		uint uRead = 0;
		cr = m_refStream.Deref().Read((uintptr)FixedArrayHelper::GetInternalPointer(m_buffer), BUFFER_SIZE, uRead);
		if( cr.IsFailed() )
			return cr;
		m_iTotal += ((int64)uRead);
		m_uValidSize0 = uRead;
		m_uValidSize = uRead;
		m_uPos = 0;
		m_uFlags |= MASK_VALID;
		return cr;
	}

	//read
	CallResult read_bytes_buffer(byte* p, uint uBytes, uint& uAct) throw()
	{
		assert( uBytes != 0 );
		CallResult cr;
		uAct = 0;
		//flags
		if( !(m_uFlags & MASK_TEST_READING) ) {
			//can write
			if( m_uFlags & MASK_VALID ) {
				assert( (m_uFlags & MASK_CAN_WRITE) && !(m_uFlags & MASK_CAN_READ) );
				cr = process_dirty_flag();
				if( cr.IsFailed() )
					return cr;
				if( m_uPos != 0 ) {
					int64 iNewPos;
					cr = m_refStream.Deref().Seek(IO_SEEK_CURRENT, (int64)m_uPos, iNewPos);
					if( cr.IsFailed() )
						return cr;
					m_iTotal += ((int64)m_uPos);
				}
				m_uFlags &= ~MASK_VALID;
			}
			//try
			m_uFlags |= MASK_TEST_READING;
			cr = m_refStream.Deref().Read((uintptr)p, uBytes, uAct);
			if( cr.IsFailed() )
				return cr;
			m_uFlags |= MASK_CAN_READ;
			if( uAct == 0 ) {
				cr.SetResult(SystemCallResults::S_EOF);
				return cr;
			}
			m_iTotal += ((int64)uAct);
			return cr;
		}
		//can read
		if( !(m_uFlags & MASK_CAN_READ) ) {
			cr.SetResult(SystemCallResults::NoAccess);
			return cr;
		}
		//buffer
		if( m_uFlags & MASK_VALID ) {
			assert( m_uPos <= m_uValidSize );
			uint uRest = m_uValidSize - m_uPos;
			if( uRest > 0 ) {
				uint uCopy = (uBytes <= uRest) ? uBytes : uRest;
				mem_copy(FixedArrayHelper::GetInternalPointer(m_buffer) + m_uPos, uCopy, p);
				m_uPos += uCopy;
				uBytes -= uCopy;
				p += uCopy;
				uAct += uCopy;
				if( uBytes == 0 )
					return cr;
			}
			//file end
			if( m_uValidSize < BUFFER_SIZE ) {
				if( uRest == 0 ) {
					cr.SetResult(SystemCallResults::S_EOF);
					return cr;
				}
				return cr;
			}
			//process dirty flag
			cr = process_dirty_flag();
			if( cr.IsFailed() ) {
				if( uRest > 0 ) {
					cr.SetResult(SystemCallResults::OK);
					return cr;
				}
				return cr;
			}
			//flag
			m_uFlags &= ~MASK_VALID;
		}
		//middle
		uint uRead;
		cr = m_refStream.Deref().Read((uintptr)p, uBytes, uRead);
		if( cr.IsFailed() ) {
			if( uAct == 0 )
				return cr;
			cr.SetResult(SystemCallResults::OK);
			return cr;
		}
		if( uRead == 0 ) {
			if( uAct == 0 ) {
				cr.SetResult(SystemCallResults::S_EOF);
				return cr;
			}
			return cr;
		}
		uAct += uRead;
		m_iTotal += ((int64)uRead);
		if( uRead < uBytes )
			return cr;
		//pre-read
		cr = _pre_read_buffer();
		if( cr.IsFailed() ) {
			cr.SetResult(SystemCallResults::OK);
			return cr;
		}
		return cr;
	}

	//back
	CallResult back_bytes(int64 iBytes) throw()
	{
		int64 iNewPos;
		return m_refStream.Deref().Seek(IO_SEEK_CURRENT, -iBytes, iNewPos);
	}
	CallResult back_bytes_buffer(int64 iBytes) throw()
	{
		CallResult cr;
		int64 iActTotal = m_iTotal;
		//buffer used
		if( m_uFlags & MASK_VALID ) {
			if( (int64)m_uPos >= iBytes ) {
				m_uPos -= ((uint)iBytes);
				return cr;
			}
			iBytes -= (int64)m_uPos;
			if( m_uFlags & MASK_CAN_READ ) {
				cr = SafeOperators::Add((int64)m_uValidSize, iBytes, iBytes);
				if( cr.IsFailed() )
					return cr;
				iActTotal += (int64)(m_uValidSize - m_uValidSize0);
			}
		}
		if( iBytes > iActTotal ) {
			cr.SetResult(SystemCallResults::Invalid);
			return cr;
		}
		//process dirty flag
		cr = process_dirty_flag();
		if( cr.IsFailed() )
			return cr;
		//back
		cr = back_bytes(iBytes);
		if( cr.IsFailed() )
			return cr;
		m_iTotal -= iBytes;
		//buffer flag
		if( m_uFlags & MASK_VALID )
			m_uFlags &= ~MASK_VALID;
		return cr;
	}

	//write
	CallResult _calc_buffer_copy_chars(uintptr uChars, uint uBytesPerChar, uint& uCopyChars) throw()
	{
		CallResult cr;
		uint uBufferChars = BUFFER_SIZE / uBytesPerChar;
		//align
		if( (m_uPos % uBytesPerChar) != 0 || (m_uValidSize % uBytesPerChar) != 0 ) {
			cr.SetResult(SystemCallResults::Corrupt);
			return cr;
		}
		//extend
		uint uRestChars = (m_uValidSize - m_uPos) / uBytesPerChar;
		uCopyChars = (uint)uChars;
		if( uChars > (uintptr)uRestChars ) {
			uRestChars += (uBufferChars - m_uValidSize / uBytesPerChar);
			if( uChars > (uintptr)uRestChars )
				uCopyChars = uRestChars;
			m_uValidSize = m_uPos + uCopyChars * uBytesPerChar;
		}
		return cr;
	}
	void _do_swap_buffer(void* p, uint uChars, uint uBytesPerChar) throw()
	{
		if( m_bSwap ) {
			if( uBytesPerChar == sizeof(CharH) )
				process_swap<CharH>((CharH*)p, uChars);
			else if( uBytesPerChar == sizeof(CharL) )
				process_swap<CharL>((CharL*)p, uChars);
		}
	}
	void _copy_to_buffer(const void* p, uint uCopyChars, uint uBytesPerChar) throw()
	{
		assert( uCopyChars != 0 );
		uint uCopyBytes = uCopyChars * uBytesPerChar;
		//copy
		void* pd = FixedArrayHelper::GetInternalPointer(m_buffer) + m_uPos;
		mem_copy(p, uCopyBytes, pd);
		_do_swap_buffer(pd, uCopyChars, uBytesPerChar);
		//dirty
		uint uEnd = m_uPos + uCopyBytes - 1;
		if( !(m_uFlags & MASK_DIRTY) ) {
			m_uFlags |= MASK_DIRTY;
			m_uDirtyBegin = m_uPos;
			m_uDirtyEnd = uEnd;
		}
		else {
			if( m_uPos < m_uDirtyBegin )
				m_uDirtyBegin = m_uPos;
			if( uEnd > m_uDirtyEnd )
				m_uDirtyEnd = uEnd;
		}
		m_uPos += uCopyBytes;
	}
	CallResult _flush_buffer() throw()
	{
		CallResult cr;
		cr = process_dirty_flag();
		if( cr.IsFailed() )
			return cr;
		int64 iNewPos;
		int64 iMove;
		if( !(m_uFlags & MASK_CAN_READ) )
			iMove = (int64)m_uPos;
		else
			iMove = -(int64)(m_uValidSize - m_uPos);
		if( iMove != 0 ) {
			cr = m_refStream.Deref().Seek(IO_SEEK_CURRENT, iMove, iNewPos);
			if( cr.IsFailed() )
				return cr;
			m_iTotal += iMove;
		}
		return cr;
	}
	CallResult write_with_buffer(const byte* p, uintptr uChars, uint uBytesPerChar, uintptr& uActChars) throw()
	{
		assert( uChars != 0 );
		CallResult cr;
		uint uSavedFlags;
		uint uCopyChars = 0;
		uActChars = 0;
		//flags
		if( !(m_uFlags & MASK_TEST_WRITING) ) {
			//can read
			if( m_uFlags & MASK_VALID ) {
				assert( (m_uFlags & MASK_CAN_READ) && !(m_uFlags & MASK_CAN_WRITE) );
				assert( m_uPos <= m_uValidSize0 );
				if( m_uPos < m_uValidSize0 ) {
					int64 iBack = (int64)(m_uValidSize0 - m_uPos);
					cr = back_bytes(iBack);
					if( cr.IsFailed() )
						return cr;
					m_iTotal -= iBack;
				}
			}
			else {
				m_uFlags |= MASK_VALID;
			}
			//try
			m_uPos = m_uValidSize = m_uValidSize0 = 0;
			cr = _calc_buffer_copy_chars(uChars, uBytesPerChar, uCopyChars);
			assert( cr.IsSucceeded() );
			_copy_to_buffer(p, uCopyChars, uBytesPerChar);
			m_uFlags |= MASK_TEST_WRITING;
			uSavedFlags = m_uFlags;
			m_uFlags &= ~MASK_CAN_READ;
			m_uFlags |= MASK_CAN_WRITE;
			cr = _flush_buffer();
			m_uFlags = uSavedFlags;
			m_uFlags &= ~MASK_VALID;
			m_uFlags &= ~MASK_DIRTY;
			if( cr.IsFailed() )
				return cr;
			m_uFlags |= MASK_CAN_WRITE;
			p += (uCopyChars * uBytesPerChar);
			uChars -= uCopyChars;
			uActChars += uCopyChars;
			if( uChars == 0 )
				return cr;
		}
		//can write
		if( !(m_uFlags & MASK_CAN_WRITE) ) {
			cr.SetResult(SystemCallResults::NoAccess);
			return cr;
		}
		//buffer
		if( m_uFlags & MASK_VALID ) {
			assert( m_uPos <= m_uValidSize );
			assert( m_uValidSize <= BUFFER_SIZE );
			cr = _calc_buffer_copy_chars(uChars, uBytesPerChar, uCopyChars);
			if( cr.IsFailed() )
				return cr;
			if( uCopyChars != 0 ) {
				_copy_to_buffer(p, uCopyChars, uBytesPerChar);
				p += (uCopyChars * uBytesPerChar);
				uChars -= uCopyChars;
				uActChars += uCopyChars;
			}
			if( uChars == 0 )
				return cr;
			cr = _flush_buffer();
			if( cr.IsFailed() ) {
				if( uActChars == 0 )
					return cr;
				cr.SetResult(SystemCallResults::OK);
				return cr;
			}
		}
		else {
			m_uFlags |= MASK_VALID;
		}
		uSavedFlags = m_uFlags;
		m_uFlags &= ~MASK_CAN_READ;
		//loop
		while( true ) {
			m_uPos = m_uValidSize = m_uValidSize0 = 0;
			cr = _calc_buffer_copy_chars(uChars, uBytesPerChar, uCopyChars);
			assert( cr.IsSucceeded() );
			//last block
			if( uChars <= (uintptr)uCopyChars ) {
				m_uFlags = uSavedFlags;
				if( m_uFlags & MASK_CAN_READ ) {
					m_uFlags &= ~MASK_VALID;
					cr = _pre_read_buffer();
					if( cr.IsFailed() ) {
						if( uActChars == 0 )
							return cr;
						cr.SetResult(SystemCallResults::OK);
						return cr;
					}
					//recalc
					cr = _calc_buffer_copy_chars(uChars, uBytesPerChar, uCopyChars);
					if( cr.IsFailed() ) {
						if( uActChars == 0 )
							return cr;
						cr.SetResult(SystemCallResults::OK);
						return cr;
					}
				}
				_copy_to_buffer(p, uCopyChars, uBytesPerChar);
				uActChars += uCopyChars;
				break;
			}
			//middle
			_copy_to_buffer(p, uCopyChars, uBytesPerChar);
			cr = _flush_buffer();
			m_uFlags = uSavedFlags;
			m_uFlags &= ~MASK_VALID;
			if( cr.IsFailed() ) {
				if( uActChars == 0 )
					return cr;
				cr.SetResult(SystemCallResults::OK);
				return cr;
			}
			p += (uCopyChars * uBytesPerChar);
			uChars -= uCopyChars;
			uActChars += uCopyChars;
			m_uFlags |= MASK_VALID;
			m_uFlags &= ~MASK_CAN_READ;
		} //end while
		return cr;
	}

	//tools
	CallResult read_characters(byte* p, uintptr uChars, uint uBytesPerChar, uintptr& uActChars) throw()
	{
		CallResult cr;
		uint uMaxChars = Limits<uint>::Max / uBytesPerChar;
		uActChars = 0;
		while( uChars != 0 ) {
			uint uActBytes;
			uint uAcq = (uint)uChars;
			if( uChars > (uintptr)uMaxChars )
				uAcq = uMaxChars;
			cr = read_bytes_buffer(p, uAcq * uBytesPerChar, uActBytes);
			if( cr.IsFailed() || cr.GetResult() == SystemCallResults::S_EOF ) {
				if( uActChars == 0 )
					return cr;
				cr.SetResult(SystemCallResults::OK);
				return cr;
			}
			if( (uActBytes % uBytesPerChar) != 0 ) {
				cr.SetResult(SystemCallResults::Corrupt);
				return cr;
			}
			uAcq = uActBytes / uBytesPerChar;
			_do_swap_buffer(p, uAcq, uBytesPerChar);
			uChars -= uAcq;
			p += uActBytes;
			uActChars += uAcq;
		} //end while
		return cr;
	}
	CallResult write_characters(const byte* p, uintptr uChars, uint uBytesPerChar) throw()
	{
		uintptr uActChars;
		CallResult cr(write_with_buffer(p, uChars, uBytesPerChar, uActChars));
		if( cr.IsFailed() )
			return cr;
		if( uActChars != uChars ) {
			cr.SetResult(SystemCallResults::DiskFull);
			return cr;
		}
		return cr;
	}
	CallResult write_crlf(uint uBytesPerChar) throw();

	//get string
	template <class TString, class Thelper>
	CallResult get_string(int iMode, ConstArray<typename TString::EType>& carrSeparatorSet, TString& str) throw()
	{
		assert( iMode >= 0 && iMode < MODE_MAX );
		CallResult cr;
		const uintptr c_chars = BUFFER_SIZE / 4;  //characters
		uintptr uActChars;
		//separator set
		if( iMode == MODE_SEPARATOR ) {
			initialize_separator_set(carrSeparatorSet);
		}
		try {
			//a block
			uintptr uLength = c_chars;
			while( true ) {
				str.SetLength(uLength);  //may throw
				uintptr uStart = uLength - c_chars;
				cr = read_characters((byte*)(Thelper::GetInternalPointer(str) + uStart), c_chars, sizeof(typename TString::EType), uActChars);
				if( cr.IsFailed() )
					return cr;
				//file end
				if( cr.GetResult() == SystemCallResults::S_EOF ) {
					uLength -= c_chars;
					str.SetLength(uLength);
					if( iMode == MODE_ALL || uLength != 0 )
						cr.SetResult(SystemCallResults::OK);
					return cr;
				}
				if( uActChars < c_chars ) {
					uLength -= (c_chars - uActChars);
					str.SetLength(uLength);
				}
				//check
				if( iMode == MODE_CRLF ) {
					uintptr uNow = uStart;
					auto iter(str.ToIterator(str.GetPosition(uStart)));
					for( ; iter != str.GetEnd(); iter.MoveNext(), uNow ++ ) {
						uintptr uBack = uLength - uNow;
						if( iter.get_Value() == '\n' ) {
							str.SetLength(uNow);
							uBack --;
							if( uBack != 0 ) {
								cr = back_bytes_buffer(uBack * sizeof(typename TString::EType));
								assert( cr.IsSucceeded() );
							}
							return cr;
						}
						if( iter.get_Value() == '\r' ) {
							uBack --;
							auto iterNext(iter);
							iterNext.MoveNext();
							if( iterNext != str.GetEnd() ) {
								if( iterNext.get_Value() == '\n' )
									uBack --;
								str.SetLength(uNow);
								if( uBack != 0 ) {
									cr = back_bytes_buffer(uBack * sizeof(typename TString::EType));
									assert( cr.IsSucceeded() );
								}
								return cr;
							}
							str.SetLength(uNow);
							typename TString::EType ch;
							uintptr uAct;
							cr = read_characters((byte*)&ch, 1, sizeof(typename TString::EType), uAct);
							if( cr.IsFailed() )
								return cr;
							if( cr.GetResult() == SystemCallResults::S_EOF ) {
								cr.SetResult(SystemCallResults::OK);
								return cr;
							}
							if( ch == '\n' )
								return cr;
							cr = back_bytes_buffer(1 * sizeof(typename TString::EType));
							assert( cr.IsSucceeded() );
							return cr;
						}
					} //end for
				}
				else if( iMode == MODE_SEPARATOR ) {
					uintptr uNow = uStart;
					if( uStart == 0 ) {
						//skip
						auto iter(str.ToIterator(str.GetPosition(uStart)));
						for( ; iter != str.GetEnd(); iter.MoveNext(), uNow ++ ) {
							if( !check_separator_set<typename TString::EType>(iter.get_Value(), carrSeparatorSet) )
								break;
						}
						_StringOpHelper::Delete(uStart, uNow, str);
						uLength = str.GetLength();
						uNow = uStart;
					}
					if( uLength != 0 ) {
						//check
						auto iter(str.ToIterator(str.GetPosition(uStart)));
						for( ; iter != str.GetEnd(); iter.MoveNext(), uNow ++ ) {
							if( check_separator_set<typename TString::EType>(iter.get_Value(), carrSeparatorSet) ) {
								uintptr uBack = uLength - uNow - 1;
								str.SetLength(uNow);
								if( uBack != 0 ) {
									cr = back_bytes_buffer(uBack * sizeof(typename TString::EType));
									assert( cr.IsSucceeded() );
								}
								return cr;
							}
						}
					}
				} //end if
				//add size
				uLength = SafeOperators::AddThrow(uLength, c_chars);  //may throw
			} //end while
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}

	//valid
	bool is_valid() const throw()
	{
		return !m_refStream.IsNull();
	}

protected:
	RefPtr<_IByteStream>  m_refStream;

	ConstArray<CharA>  m_carrSeparatorSetA;
	ConstArray<CharH>  m_carrSeparatorSetH;
	ConstArray<CharL>  m_carrSeparatorSetL;

	int64  m_iTotal;      //the current position of stream without BOM
	uint   m_uFlags;      //for reading & writing

	FixedArray<byte, BUFFER_SIZE> m_buffer;  //reading buffer
	//buffer
	uint m_uValidSize0;
	uint m_uValidSize;  //the actual bytes have been read or written in buffer
	uint m_uPos;        //the current position in buffer
	//dirty region
	uint m_uDirtyBegin;
	uint m_uDirtyEnd;

	//type
	int  m_iBomType;
	//style
	int  m_iCRLFStyle;

	bool m_bSwap;
};

// _ITextStreamRootImpl

class NOVTABLE _ITextStreamRootImpl : public _TextStreamBase,
									public _ITextStreamRoot
{
public:
// _ITextStreamRoot methods
	virtual void Reset() throw()
	{
		reset_buffer();
	}
	virtual GKC::CallResult CheckBOM(int& iType) throw()
	{
		assert( is_valid() );

		//default
		iType = _BOMTypes::None;
		set_BOM_type(iType);

		uint uRead;
		byte btBOM[4];
		CallResult cr;

		uint uAcq = (uint)sizeof(btBOM);
		//check UTF32
		cr = m_refStream.Deref().Read((uintptr)btBOM, uAcq, uRead);
		if( cr.IsFailed() )
			return cr;
		if( uRead == uAcq ) {
			// FF FE 00 00
			if( btBOM[0] == 0xFF && btBOM[1] == 0xFE && btBOM[2] == 0x00 && btBOM[3] == 0x00 ) {
				iType = _BOMTypes::UTF32LE;
				set_BOM_type(iType);
				return cr;
			}
			// 00 00 FE FF
			if( btBOM[0] == 0x00 && btBOM[1] == 0x00 && btBOM[2] == 0xFE && btBOM[3] == 0xFF ) {
				iType = _BOMTypes::UTF32BE;
				set_BOM_type(iType);
				return cr;
			}
		}

		//check UTF8
		if( uRead >= 3 ) {
			// EF BB BF
			if( btBOM[0] == 0xEF && btBOM[1] == 0xBB && btBOM[2] == 0xBF ) {
				iType = _BOMTypes::UTF8;
				set_BOM_type(iType);
				//back
				if( uRead > 3 )
					cr = back_bytes((int64)(uRead - 3));
				return cr;
			}
		}

		//check UTF16
		if( uRead >= 2 ) {
			// FF FE
			if( btBOM[0] == 0xFF && btBOM[1] == 0xFE ) {
				iType = _BOMTypes::UTF16LE;
				set_BOM_type(iType);
				//back
				if( uRead > 2 )
					cr = back_bytes((int64)(uRead - 2));
				return cr;
			}
			// FE FF
			if( btBOM[0] == 0xFE && btBOM[1] == 0xFF ) {
				iType = _BOMTypes::UTF16BE;
				set_BOM_type(iType);
				//back
				if( uRead > 2 )
					cr = back_bytes((int64)(uRead - 2));
				return cr;
			}
		}

		//none
		if( uRead != 0 )
			cr = back_bytes((int64)uRead);

		return cr;
	}
	virtual void SetBOM(const int& iType) throw()
	{
		set_BOM_type(iType);
	}
	virtual int GetBOM() throw()
	{
		return m_iBomType;
	}
	virtual void SetCRLFStyle(const int& iStyle) throw()
	{
		assert( iStyle >= 0 && iStyle < _CRLFStyles::Max );
		m_iCRLFStyle = iStyle;
	}
	virtual int GetCRLFStyle() throw()
	{
		return m_iCRLFStyle;
	}
	virtual GKC::CallResult GetCharA(GKC::CharA& ch) throw()
	{
		assert( is_valid() );
		uintptr uActChars;
		return read_characters((byte*)(&ch), 1, sizeof(CharA), uActChars);
	}
	virtual GKC::CallResult UngetCharA(const int64& iCharNum) throw()
	{
		assert( is_valid() );
		assert( iCharNum > 0 );
		return back_bytes_buffer(iCharNum);  //sizeof(byte)
	}
	virtual GKC::CallResult GetCharH(GKC::CharH& ch) throw()
	{
		assert( is_valid() );
		uintptr uActChars;
		return read_characters((byte*)(&ch), 1, sizeof(CharH), uActChars);
	}
	virtual GKC::CallResult UngetCharH(const int64& iCharNum) throw()
	{
		assert( is_valid() );
		assert( iCharNum > 0 );
		int64 iBytes;
		CallResult cr = SafeOperators::Multiply(iCharNum, (int64)sizeof(CharH), iBytes);
		if( cr.IsFailed() )
			return cr;
		return back_bytes_buffer(iBytes);
	}
	virtual GKC::CallResult GetCharL(GKC::CharL& ch) throw()
	{
		assert( is_valid() );
		uintptr uActChars;
		return read_characters((byte*)(&ch), 1, sizeof(CharL), uActChars);
	}
	virtual GKC::CallResult UngetCharL(const int64& iCharNum) throw()
	{
		assert( is_valid() );
		assert( iCharNum > 0 );
		int64 iBytes;
		CallResult cr = SafeOperators::Multiply(iCharNum, (int64)sizeof(CharL), iBytes);
		if( cr.IsFailed() )
			return cr;
		return back_bytes_buffer(iBytes);
	}
	virtual GKC::CallResult GetChar(GKC::CharF& ch) throw()
	{
		CallResult cr;
		switch( m_iBomType ) {
		case _BOMTypes::UTF16LE:
		case _BOMTypes::UTF16BE:
			{
				CharH chRead;
				cr = GetCharH(chRead);
				if( cr.IsFailed() )
					return cr;
				ch = (CharF)(ushort)chRead;
			} //end block
			break;
		case _BOMTypes::UTF32LE:
		case _BOMTypes::UTF32BE:
			{
				CharL chRead;
				cr = GetCharL(chRead);
				if( cr.IsFailed() )
					return cr;
				ch = (CharF)(uint)chRead;
			} //end block
			break;
		default:
			{
				CharA chRead;
				cr = GetCharA(chRead);
				if( cr.IsFailed() )
					return cr;
				ch = (CharF)(byte)chRead;
			} //end block
			break;
		}
		return cr;
	}
	virtual GKC::CallResult UngetChar(const int64& iCharNum) throw()
	{
		CallResult cr;
		switch( m_iBomType ) {
		case _BOMTypes::UTF16LE:
		case _BOMTypes::UTF16BE:
			cr = UngetCharH(iCharNum);
			break;
		case _BOMTypes::UTF32LE:
		case _BOMTypes::UTF32BE:
			cr = UngetCharL(iCharNum);
			break;
		default:
			cr = UngetCharA(iCharNum);
			break;
		}
		return cr;
	}
	virtual GKC::CallResult WriteBOM() throw()
	{
		assert( is_valid() );

		CallResult cr;

		//BOM
		byte btBOM[4];
		uint uAcq = 0;
		if( m_iBomType == _BOMTypes::UTF16LE ) {
			btBOM[0] = 0xFF;  btBOM[1] = 0xFE;
			uAcq = 2;
		}
		else if( m_iBomType == _BOMTypes::UTF16BE ) {
			btBOM[0] = 0xFE;  btBOM[1] = 0xFF;
			uAcq = 2;
		}
		else if( m_iBomType == _BOMTypes::UTF8 ) {
			btBOM[0] = 0xEF;  btBOM[1] = 0xBB;  btBOM[2] = 0xBF;
			uAcq = 3;
		}
		else if( m_iBomType == _BOMTypes::UTF32LE ) {
			btBOM[0] = 0xFF;  btBOM[1] = 0xFE;  btBOM[2] = 0x00;  btBOM[3] = 0x00;
			uAcq = 4;
		}
		else if( m_iBomType == _BOMTypes::UTF32BE ) {
			btBOM[0] = 0x00;  btBOM[1] = 0x00;  btBOM[2] = 0xFE;  btBOM[3] = 0xFF;
			uAcq = 4;
		}

		//write
		if( uAcq != 0 ) {
			uint uWritten;
			cr = m_refStream.Deref().Write((uintptr)btBOM, uAcq, uWritten);
			if( cr.IsFailed() )
				return cr;
			if( uWritten != uAcq ) {
				cr.SetResult(SystemCallResults::DiskFull);
				return cr;
			}
		}

		return cr;
	}
	virtual GKC::CallResult MoveToEnd() throw()
	{
		assert( is_valid() );
		CallResult cr(process_dirty_flag());
		if( cr.IsFailed() )
			return cr;
		int64 iCurPos, iNewPos;
		cr = m_refStream.Deref().Seek(IO_SEEK_CURRENT, 0, iCurPos);
		if( cr.IsFailed() )
			return cr;
		cr = m_refStream.Deref().Seek(IO_SEEK_END, 0, iNewPos);
		if( cr.IsFailed() )
			return cr;
		m_iTotal += (iNewPos - iCurPos);
		if( m_uFlags & MASK_VALID )
			m_uFlags &= ~MASK_VALID;
		return cr;
	}
	virtual GKC::CallResult PutCharA(const GKC::CharA& ch) throw()
	{
		assert( is_valid() );
		return write_characters((const byte*)(&ch), 1, sizeof(CharA));
	}
	virtual GKC::CallResult PutCharH(const GKC::CharH& ch) throw()
	{
		assert( is_valid() );
		return write_characters((const byte*)(&ch), 1, sizeof(CharH));
	}
	virtual GKC::CallResult PutCharL(const GKC::CharL& ch) throw()
	{
		assert( is_valid() );
		return write_characters((const byte*)(&ch), 1, sizeof(CharL));
	}
	virtual GKC::CallResult PutChar(const GKC::CharF& ch) throw()
	{
		CallResult cr;
		switch( m_iBomType ) {
		case _BOMTypes::UTF16LE:
		case _BOMTypes::UTF16BE:
			cr = PutCharH((CharH)ch);
			break;
		case _BOMTypes::UTF32LE:
		case _BOMTypes::UTF32BE:
			cr = PutCharL((CharL)ch);
			break;
		default:
			cr = PutCharA((CharA)ch);
			break;
		}
		return cr;
	}
	virtual GKC::CallResult PutStringA(const GKC::ConstStringA& str) throw()
	{
		assert( is_valid() );
		return write_characters((const byte*)(ConstArrayHelper::GetInternalPointer(str)), str.GetCount(), sizeof(CharA));
	}
	virtual GKC::CallResult PutStringH(const GKC::ConstStringH& str) throw()
	{
		assert( is_valid() );
		return write_characters((const byte*)(ConstArrayHelper::GetInternalPointer(str)), str.GetCount(), sizeof(CharH));
	}
	virtual GKC::CallResult PutStringL(const GKC::ConstStringL& str) throw()
	{
		assert( is_valid() );
		return write_characters((const byte*)(ConstArrayHelper::GetInternalPointer(str)), str.GetCount(), sizeof(CharL));
	}
	virtual GKC::CallResult PutNewLineA() throw()
	{
		assert( is_valid() );
		return write_crlf(sizeof(CharA));
	}
	virtual GKC::CallResult PutNewLineH() throw()
	{
		assert( is_valid() );
		return write_crlf(sizeof(CharH));
	}
	virtual GKC::CallResult PutNewLineL() throw()
	{
		assert( is_valid() );
		return write_crlf(sizeof(CharL));
	}
	virtual GKC::CallResult PutNewLine() throw()
	{
		CallResult cr;
		switch( m_iBomType ) {
		case _BOMTypes::UTF16LE:
		case _BOMTypes::UTF16BE:
			cr = PutNewLineH();
			break;
		case _BOMTypes::UTF32LE:
		case _BOMTypes::UTF32BE:
			cr = PutNewLineL();
			break;
		default:
			cr = PutNewLineA();
			break;
		}
		return cr;
	}
	virtual GKC::CallResult PutLineA(const GKC::ConstStringA& str) throw()
	{
		CallResult cr(PutStringA(str));
		if( cr.IsSucceeded() )
			cr = PutNewLineA();
		return cr;
	}
	virtual GKC::CallResult PutLineH(const GKC::ConstStringH& str) throw()
	{
		CallResult cr(PutStringH(str));
		if( cr.IsSucceeded() )
			cr = PutNewLineH();
		return cr;
	}
	virtual GKC::CallResult PutLineL(const GKC::ConstStringL& str) throw()
	{
		CallResult cr(PutStringL(str));
		if( cr.IsSucceeded() )
			cr = PutNewLineL();
		return cr;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
