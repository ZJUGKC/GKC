/*
** Xin YUAN, 2023, BSD (2)
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcSys.h"

#include "base/SysDef.h"

////////////////////////////////////////////////////////////////////////////////

_LoggerCallback g_logger_cb = NULL;

void _Logger_SetCallback(_LoggerCallback p) noexcept
{
	g_logger_cb = p;
}

static constexpr const GKC::CharA c_delimiter[] = "[\n";

void _Logger_Output(const GKC::CharA* szFile, int iLine, const GKC::CharA* szMessage) noexcept
{
	if ( g_logger_cb == NULL )
		return ;

	GKC::MutexLock lock(*_MemoryMutex_Get());

	GKC::RefPtr<_IByteStream> refStream(g_logger_cb());  //no throw
	if ( refStream.IsNull() )
		return ;

	GKC::CallResult cr;
	uint uWritten;

	cr = refStream.Deref().Write((uintptr)(void*)&c_delimiter[0], 1, uWritten);
	if ( cr.IsFailed() )
		return ;
	cr = refStream.Deref().Write((uintptr)(void*)szFile, (uint)calc_string_length(szFile), uWritten);
	if ( cr.IsFailed() )
		return ;
	char buf[64];
	buf[0] = 0;
	int ret = value_to_string(buf, sizeof(buf) / sizeof(char), "][%d]", iLine);
	if( ret >= 0 )
		buf[ret] = 0;
	cr = refStream.Deref().Write((uintptr)(void*)buf, (uint)calc_string_length(buf), uWritten);
	if ( cr.IsFailed() )
		return ;
	cr = refStream.Deref().Write((uintptr)(void*)szMessage, (uint)calc_string_length(szMessage), uWritten);
	if ( cr.IsFailed() )
		return ;
	cr = refStream.Deref().Write((uintptr)(void*)&c_delimiter[1], 1, uWritten);
	if ( cr.IsFailed() )
		return ;
	cr = refStream.Deref().Commit();
	if ( cr.IsFailed() )
		return ;
}

////////////////////////////////////////////////////////////////////////////////
