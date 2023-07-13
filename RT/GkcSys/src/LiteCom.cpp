/*
** Xin YUAN, 2023, BSD (2)
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcSys.h"

#include "base/SysDef.h"

#include "stream/FileStream.h"
#include "stream/MemoryStreamU.h"
#include "stream/BufferStream.h"
#include "stream/TextStreamU.h"

#include "compress/PkzCompressor.h"
#include "compress/PkzDecompressor.h"

////////////////////////////////////////////////////////////////////////////////

//FileStreamL

class FileStreamL : public _IFileStream, public GKC::FileStream
{
public:
//_IFileStream methods
	virtual GKC::RefPtr<_IFileUtility> GetFileUtility() noexcept
	{
		return _LiteCom_GetInterface<FileStreamL, _IFileUtility, GKC::FileStream>(this);
	}
	virtual GKC::RefPtr<_IByteStream> GetByteStream() noexcept
	{
		return _LiteCom_GetInterface<FileStreamL, _IByteStream, GKC::FileStream>(this);
	}
	virtual GKC::RefPtr<_IStreamLocker> GetStreamLocker() noexcept
	{
		return _LiteCom_GetInterface<FileStreamL, _IStreamLocker, GKC::FileStream>(this);
	}
};

_LiteCom<_IFileStream> _FileStream_CreateL() noexcept
{
	return _LiteComObject<FileStreamL>::CreateInstance<_IFileStream>();
}

//MemoryStreamL

class MemoryStreamL : public _IMemoryStream, public GKC::MemoryStreamU
{
public:
//_IMemoryStream methods
	virtual GKC::RefPtr<_IMemoryUtilityU> GetMemoryUtility() noexcept
	{
		return _LiteCom_GetInterface<MemoryStreamL, _IMemoryUtilityU, GKC::MemoryStreamU>(this);
	}
	virtual GKC::RefPtr<_IByteStream> GetByteStream() noexcept
	{
		return _LiteCom_GetInterface<MemoryStreamL, _IByteStream, GKC::MemoryStreamU>(this);
	}
};

_LiteCom<_IMemoryStream> _MemoryStream_CreateL() noexcept
{
	return _LiteComObject<MemoryStreamL>::CreateInstance<_IMemoryStream>();
}

//BufferStreamL

class BufferStreamL : public _IBufferStream, public GKC::BufferStream
{
public:
//_IBufferStream methods
	virtual GKC::RefPtr<_IBufferUtility> GetBufferUtility() noexcept
	{
		return _LiteCom_GetInterface<BufferStreamL, _IBufferUtility, GKC::BufferStream>(this);
	}
	virtual GKC::RefPtr<_IByteStream> GetByteStream() noexcept
	{
		return _LiteCom_GetInterface<BufferStreamL, _IByteStream, GKC::BufferStream>(this);
	}
};

_LiteCom<_IBufferStream> _BufferStream_CreateL() noexcept
{
	return _LiteComObject<BufferStreamL>::CreateInstance<_IBufferStream>();
}

//TextStreamL

class TextStreamL : public _ITextStream, public GKC::TextStreamU
{
public:
//_ITextStream methods
	virtual GKC::RefPtr<_ITextUtilityU> GetTextUtility() noexcept
	{
		return _LiteCom_GetInterface<TextStreamL, _ITextUtilityU, GKC::TextStreamU>(this);
	}
	virtual GKC::RefPtr<_ITextStreamRoot> GetTextStreamRoot() noexcept
	{
		return _LiteCom_GetInterface<GKC::TextStreamU, _ITextStreamRoot, GKC::_ITextStreamRootImpl>(static_cast<GKC::TextStreamU*>(this));
	}
	virtual GKC::RefPtr<_ITextStreamStringU> GetTextStreamString() noexcept
	{
		return _LiteCom_GetInterface<TextStreamL, _ITextStreamStringU, GKC::TextStreamU>(this);
	}
};

_LiteCom<_ITextStream> _TextStream_CreateL() noexcept
{
	return _LiteComObject<TextStreamL>::CreateInstance<_ITextStream>();
}

//PkZipL

class PkZipL : public _IPkZip, public GKC::PkzCompressor
{
public:
//_IPkZip methods
	virtual GKC::RefPtr<_IPkCompressor> GetPkCompressor() noexcept
	{
		return _LiteCom_GetInterface<PkZipL, _IPkCompressor, GKC::PkzCompressor>(this);
	}
};

_LiteCom<_IPkZip> _PkZip_CreateL() noexcept
{
	return _LiteComObject<PkZipL>::CreateInstance<_IPkZip>();
}

//PkUnzipL

class PkUnzipL : public _IPkUnzip, public GKC::PkzDecompressor
{
public:
//_IPkUnzip methods
	virtual GKC::RefPtr<_IPkDecompressor> GetPkDecompressor() noexcept
	{
		return _LiteCom_GetInterface<PkUnzipL, _IPkDecompressor, GKC::PkzDecompressor>(this);
	}
};

_LiteCom<_IPkUnzip> _PkUnzip_CreateL() noexcept
{
	return _LiteComObject<PkUnzipL>::CreateInstance<_IPkUnzip>();
}

////////////////////////////////////////////////////////////////////////////////
