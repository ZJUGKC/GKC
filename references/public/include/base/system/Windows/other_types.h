
//------------------------------------------------------------------------------
// Service

// log types
#define SERVICE_LOG_SUCCESS           EVENTLOG_SUCCESS
#define SERVICE_LOG_ERROR             EVENTLOG_ERROR_TYPE
#define SERVICE_LOG_INFORMATION       EVENTLOG_INFORMATION_TYPE
#define SERVICE_LOG_WARNING           EVENTLOG_WARNING_TYPE

// report_service_log
//  type: SERVICE_LOG_*
inline void report_service_log(const char_s* szService, uint type, const char_s* szMsg) throw()
{
	HANDLE hEventSource = ::RegisterEventSourceW(NULL, szService);
	if( hEventSource != NULL ) {
		::ReportEvent(hEventSource, type, (WORD)0, 0, NULL, (WORD)1, 0, &szMsg, NULL);  //no check
		::DeregisterEventSource(hEventSource);  //no check
	}
}
