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

static constexpr const GKC::CharA c_delimiter[] = "[]\n";

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

	//file name
	cr = refStream.Deref().Write((uintptr)(void*)&c_delimiter[0], 1, uWritten);
	if ( cr.IsFailed() )
		return ;
	cr = refStream.Deref().Write((uintptr)(void*)szFile, (uint)calc_string_length(szFile), uWritten);
	if ( cr.IsFailed() )
		return ;
	cr = refStream.Deref().Write((uintptr)(void*)&c_delimiter[1], 1, uWritten);
	if ( cr.IsFailed() )
		return ;
	//time
	char buf[64];
	buf[0] = 0;
	GKC::TimeValue tv;
	GKC::TimeHelper::FetchCurrentTime(tv);
	GKC::TimeDetail td = {};
	bool bRet = GKC::TimeHelper::ToLocalDetail(tv, td);
	(void)bRet;  //no check
	int ret = value_to_string(buf, sizeof(buf) / sizeof(char),
					"[%04d-%02d-%02d %02d:%02d:%02d]",
					td.iYear, td.iMonth, td.iDay,
					td.iHour, td.iMinute, td.iSecond);
	if ( ret >= 0 )
		buf[ret] = 0;
	cr = refStream.Deref().Write((uintptr)(void*)buf, (uint)calc_string_length(buf), uWritten);
	if ( cr.IsFailed() )
		return ;
	//line no
	buf[0] = 0;
	ret = value_to_string(buf, sizeof(buf) / sizeof(char), "[%d]", iLine);
	if ( ret >= 0 )
		buf[ret] = 0;
	cr = refStream.Deref().Write((uintptr)(void*)buf, (uint)calc_string_length(buf), uWritten);
	if ( cr.IsFailed() )
		return ;
	//message
	cr = refStream.Deref().Write((uintptr)(void*)szMessage, (uint)calc_string_length(szMessage), uWritten);
	if ( cr.IsFailed() )
		return ;
	cr = refStream.Deref().Write((uintptr)(void*)&c_delimiter[2], 1, uWritten);
	if ( cr.IsFailed() )
		return ;
	//flush
	cr = refStream.Deref().Commit();
	if ( cr.IsFailed() )
		return ;
}

////////////////////////////////////////////////////////////////////////////////
