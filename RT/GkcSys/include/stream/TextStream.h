/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
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
This file contains component class of text stream.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __TEXT_STREAM_H__
#define __TEXT_STREAM_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// TextStream
//   The author of buffer implementation : Lijuan Mei

class TextStream : public _ITextStream
{
private:
	enum { BUFFER_SIZE = 4 * 1024 };  // 4K

public:
	TextStream() throw() : m_iReadTotal(0), m_uRead(BUFFER_SIZE), m_uPos(BUFFER_SIZE)
	{
		assert( sizeof(byte) == 1 );
	}
	~TextStream() throw()
	{
	}

// _ITextStream methods
	virtual void SetStream(const _ShareCom<_IByteStream>& sp) throw()
	{
		m_spStream = sp;
		reset_buffer();
	}
	virtual GKC::CallResult CheckBOM(int& iType) throw()
	{
		if( m_spStream.IsBlockNull() )
			return CallResult(SystemCallResults::FDBad);

		//default
		iType = _BOMTypes::None;

		uint uRead;
		byte btBOM[4];
		CallResult cr;

		uint uAcq = (uint)sizeof(btBOM);
		//check UTF32
		cr = m_spStream.Deref().Read((uintptr)btBOM, uAcq, uRead);
		if( cr.IsFailed() )
			return cr;
		if( uRead == uAcq ) {
			// FF FE 00 00
			if( btBOM[0] == 0xFF && btBOM[1] == 0xFE && btBOM[2] == 0x00 && btBOM[3] == 0x00 ) {
				iType = _BOMTypes::UTF32LE;
				return cr;
			}
			// 00 00 FE FF
			if( btBOM[0] == 0x00 && btBOM[1] == 0x00 && btBOM[2] == 0xFE && btBOM[3] == 0xFF ) {
				iType = _BOMTypes::UTF32BE;
				return cr;
			}
		}

		//check UTF8
		if( uRead >= 3 ) {
			// EF BB BF
			if( btBOM[0] == 0xEF && btBOM[1] == 0xBB && btBOM[2] == 0xBF ) {
				iType = _BOMTypes::UTF8;
				//back
				if( uRead > 3 )
					cr = back_bytes((int64)(uRead - 3));
				return cr;
			}
		}

		//check UFT16
		if( uRead >= 2 ) {
			// FF FE
			if( btBOM[0] == 0xFF && btBOM[1] == 0xFE ) {
				iType = _BOMTypes::UTF16LE;
				//back
				if( uRead > 2 )
					cr = back_bytes((int64)(uRead - 2));
				return cr;
			}
			// FE FF
			if( btBOM[0] == 0xFE && btBOM[1] == 0xFF ) {
				iType = _BOMTypes::UTF16BE;
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
	virtual GKC::CallResult GetCharA(GKC::CharA& ch) throw()
	{
		if( m_spStream.IsBlockNull() )
			return CallResult(SystemCallResults::FDBad);
		return read_bytes_buffer(&ch, 1);
	}
	virtual GKC::CallResult UngetCharA(const int64& iCharNum) throw()
	{
		if( iCharNum <= 0 )
			return CallResult(SystemCallResults::Invalid);
		if( m_spStream.IsBlockNull() )
			return CallResult(SystemCallResults::FDBad);
		return back_bytes_buffer(iCharNum);  //sizeof(byte)
	}

private:
	void reset_buffer() throw()
	{
		m_iReadTotal = 0;
		m_uRead = BUFFER_SIZE;
		m_uPos = BUFFER_SIZE;
	}
	CallResult _read_bytes_buffer_direct(void* p, uint uBytes) throw()
	{
		if( uBytes > m_uRead - m_uPos )
			return CallResult(SystemCallResults::Corrupt);
		mem_copy(FixedArrayHelper::GetInternalPointer(m_buffer) + m_uPos, uBytes, p);
		m_uPos += uBytes;
		return CallResult();
	}
	CallResult read_bytes_buffer(void* p, uint uBytes) throw()
	{
		CallResult cr;
		if( m_uPos < m_uRead ) {
			cr = _read_bytes_buffer_direct(p, uBytes);
			return cr;
		}
		//file end
		if( m_uRead < BUFFER_SIZE ) {
			cr.SetResult(SystemCallResults::S_EOF);
			return cr;
		}
		//read
		cr = m_spStream.Deref().Read((uintptr)FixedArrayHelper::GetInternalPointer(m_buffer), BUFFER_SIZE, m_uRead);
		if( cr.IsFailed() )
			return cr;
		//file end
		if( m_uRead == 0 ) {
			cr.SetResult(SystemCallResults::S_EOF);
			return cr;
		}
		m_iReadTotal += ((int64)m_uRead);
		m_uPos = 0;
		cr = _read_bytes_buffer_direct(p, uBytes);
		return cr;
	}
	CallResult back_bytes(int64 iBytes) throw()
	{
		int64 iNewPos;
		return m_spStream.Deref().Seek(IO_SEEK_CURRENT, -iBytes, iNewPos);
	}
	CallResult back_bytes_buffer(int64 iBytes) throw()
	{
		CallResult cr;
		if( (int64)m_uPos >= iBytes ) {
			m_uPos -= ((uint)iBytes);
			return cr;
		}
		//file begin
		if( (int64)m_uRead == m_iReadTotal ) {
			//this is an error!
			cr.SetResult(SystemCallResults::Fail);
			return cr;
		}
		int64 iBack = 0;
		cr = SafeOperators::Add((int64)(m_uRead - m_uPos), iBytes, iBack);
		if( cr.IsFailed() )
			return cr;
		if( iBack > m_iReadTotal ) {
			cr.SetResult(SystemCallResults::Invalid);
			return cr;
		}
		//read
		cr = back_bytes(iBack);
		if( cr.IsFailed() )
			return cr;
		uint uRead = 0;
		cr = m_spStream.Deref().Read((uintptr)FixedArrayHelper::GetInternalPointer(m_buffer), BUFFER_SIZE, uRead);
		if( cr.IsFailed() )
			return cr;
		if( uRead == 0 ) {
			//this is an error!
			cr.SetResult(SystemCallResults::Fail);
			return cr;
		}
		m_iReadTotal = m_iReadTotal - iBack + (int64)uRead;
		m_uRead = uRead;
		m_uPos = 0;
		return cr;
	}

private:
	_ShareCom<_IByteStream>  m_spStream;
	FixedArray<byte, BUFFER_SIZE>  m_buffer;  //reading buffer
	int64  m_iReadTotal;  //the total bytes have been read
	uint   m_uRead;       //the actual bytes have been read in buffer
	uint   m_uPos;        //the current position in buffer
};

DECLARE_COM_TYPECAST(TextStream)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
