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
This file contains classes for char stream.
Author: Lijuan Mei
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __CHAR_STREAM_H__
#define __CHAR_STREAM_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// char stream provider

class CharStream
{
private:
	enum {
		FILE_BUFFER_SIZE = 4 * 1024   // 4K
	};

public:
	CharStream() throw() : m_uReadTotal(0), m_uRead(0), m_uPos(0)
	{
	}
	~CharStream() throw()
	{
	}

	//close
	void Close() throw()
	{
		m_hd.Close();
		m_uReadTotal = 0;
		m_uRead = 0;
		m_uPos = 0;
	}

	//init
	CallResult Initialize(const RefPtr<CharS>& szFileName) throw()
	{
		CallResult cr;

		cr = IoHandleHelper::Open(szFileName, FileOpenTypes::Read, 0, m_hd);
		if( cr.IsFailed() )
			return cr;
		m_uReadTotal = 0;
		m_uRead = 0;
		m_uPos = 0;
		//check BOM (EF BB BF)
		byte btBOM[3];
		RefPtr<byte> refBOM(btBOM);
		cr = m_hd.Read(refBOM, sizeof(btBOM), m_uRead);
		if( cr.IsFailed() ) {
			m_hd.Close();
			return cr;
		}
		if( m_uRead != sizeof(btBOM) ) {
			m_hd.Close();
			cr.SetResult(CR_FAIL);
			return cr;
		}
		if( btBOM[0] != 0xEF || btBOM[1] != 0xBB || btBOM[2] != 0xBF ) {
			m_hd.Close();
			cr.SetResult(CR_FAIL);
			return cr;
		}
		//read block
		RefPtr<byte> refBuffer(FixedArrayHelper::GetInternalPointer(m_buffer));
		cr = m_hd.Read(refBuffer, sizeof(byte) * FILE_BUFFER_SIZE, m_uRead);
		if( cr.IsFailed() ) {
			m_hd.Close();
			return cr;
		}
		m_uRead /= sizeof(byte);

		m_uReadTotal += m_uRead;

		return cr;
	}
	//get char
	//  SystemCallResult::S_EOF means the end of file is reached.
	CallResult GetChar(byte& ch) throw()
	{
		CallResult cr;
		if( m_uPos < m_uRead ) {
			ch = m_buffer[m_uPos ++].get_Value();
			return cr;  //OK
		}
		//file end
		if( m_uRead < FILE_BUFFER_SIZE ) {
			cr.SetResult(SystemCallResults::S_EOF);
			return cr;
		}
		//read
		RefPtr<byte> refBuffer(FixedArrayHelper::GetInternalPointer(m_buffer));
		cr = m_hd.Read(refBuffer, sizeof(byte) * FILE_BUFFER_SIZE, m_uRead);
		if( cr.IsFailed() ) {
			return cr;
		}
		m_uRead /= sizeof(byte);
		//file end
		if( m_uRead == 0 ) {
			cr.SetResult(SystemCallResults::S_EOF);
			return cr;
		}
		m_uReadTotal += m_uRead;
		m_uPos = 0;
		ch = m_buffer[m_uPos ++].get_Value();
		return cr;
	}
	//unget char
	CallResult UngetChar(uint uNum) throw()
	{
		assert( uNum > 0 );
		CallResult cr;
		if( m_uPos >= uNum ) {
			m_uPos -= uNum;
			return cr;
		}
		//file begin
		if( (uint64)m_uRead == m_uReadTotal ) {
			//this is an error!
			cr.SetResult(CR_FAIL);
			return cr;
		}
		//read
		int64 iNewPos;
		cr = IoHandleHelper::Seek(m_hd, -(int64)((m_uRead + uNum - m_uPos) * sizeof(byte)), iNewPos, IO_SEEK_CURRENT);
		if( cr.IsFailed() )
			return cr;
		uint uRead;
		RefPtr<byte> refBuffer(FixedArrayHelper::GetInternalPointer(m_buffer));
		cr = m_hd.Read(refBuffer, sizeof(byte) * FILE_BUFFER_SIZE, uRead);
		if (cr.IsFailed()) {
			return cr;
		}
		uRead /= sizeof(byte);
		assert( uRead != 0 );
		m_uReadTotal = m_uReadTotal - (m_uRead + uNum - m_uPos) + uRead;
		m_uRead = uRead;
		m_uPos = 0;
		return cr;
	}

private:
	IoHandle m_hd;
	FixedArray<byte, FILE_BUFFER_SIZE> m_buffer;  //reading buffer
	uint64  m_uReadTotal;  //the total bytes have been read
	uint    m_uRead;       //the actual bytes have been read in buffer
	uint    m_uPos;        //the current position in buffer

private:
	//noncopyable
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif // __CHAR_STREAM_H__
////////////////////////////////////////////////////////////////////////////////
