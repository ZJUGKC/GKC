
//------------------------------------------------------------------------------
// Service

// log types
#define SERVICE_LOG_SUCCESS           LOG_NOTICE
#define SERVICE_LOG_ERROR             LOG_ERR
#define SERVICE_LOG_INFORMATION       LOG_INFO
#define SERVICE_LOG_WARNING           LOG_WARNING

// report_service_log
//  type: SERVICE_LOG_*
inline void report_service_log(const char_s* szService, uint type, const char_s* szMsg) throw()
{
	::openlog(szService, LOG_PID | LOG_NDELAY, LOG_USER);
	::syslog(LOG_USER | type, "%s", szMsg);
	::closelog();
}
