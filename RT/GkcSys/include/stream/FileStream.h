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
This file contains component class of file stream.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __FILE_STREAM_H__
#define __FILE_STREAM_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// FileStream

class FileStream : public _IByteStream,
					public _IFileUtility,
					public _IStreamLocker
{
public:
	FileStream() throw()
	{
	}
	~FileStream() throw()
	{
	}

// _IByteSequentialStream methods
	virtual GKC::CallResult Read(const uintptr& pv, const uint& uBytes, uint& uRead) throw()
	{
		assert( IsOpened() );
		return m_hd.Read((void*)pv, uBytes, uRead);
	}
	virtual GKC::CallResult Write(const uintptr& pv, const uint& uBytes, uint& uWritten) throw()
	{
		assert( IsOpened() );
		return m_hd.Write((const void*)pv, uBytes, uWritten);
	}

// _IByteStream methods
	virtual GKC::CallResult Commit() throw()
	{
		assert( IsOpened() );
		return file_io_handle_helper::Flush(m_hd);
	}
	virtual GKC::CallResult Seek(const uint& uMethod, const int64& iOffset, int64& iNewPos) throw()
	{
		assert( IsOpened() );
		return file_io_handle_helper::Seek(m_hd, iOffset, iNewPos, uMethod);
	}
	virtual GKC::CallResult SetSize(const int64& iSize) throw()
	{
		assert( IsOpened() );
		return file_io_handle_helper::SetSize(m_hd, iSize);
	}
	virtual GKC::CallResult GetStatus(GKC::StorageStatus& status) throw()
	{
		assert( IsOpened() );
		return file_io_handle_helper::GetStatus(m_hd, status);
	}

// _IFileUtility methods

	virtual GKC::CallResult Open(const GKC::RefPtr<GKC::CharS>& szFile, const int& iOpenType, const int& iCreateType) throw()
	{
		m_hd.Close();
		return file_io_handle_helper::Open(RefPtrHelper::GetInternalPointer(szFile), iOpenType, iCreateType, m_hd);
	}
	virtual void Close() throw()
	{
		m_hd.Close();
	}
	virtual bool IsOpened() throw()
	{
		return m_hd.IsValid();
	}

// _IStreamLocker methods

	virtual GKC::CallResult Lock(int64 iOffset, int64 iLen, bool bShare) throw()
	{
		return file_io_handle_helper::Lock(m_hd, iOffset, iLen, bShare, true);
	}
	virtual void Unlock(int64 iOffset, int64 iLen) throw()
	{
		file_io_handle_helper::Unlock(m_hd, iOffset, iLen);
	}

private:
	io_handle m_hd;

private:
	//noncopyable
	FileStream(const FileStream&) throw();
	FileStream& operator=(const FileStream&) throw();
};

DECLARE_COM_TYPECAST(FileStream)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
