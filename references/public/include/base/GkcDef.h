
//Machine Type
#if ( !defined(CM_X86) && !defined(CM_X64) )
	//windows
	#define GKC_OS_WINDOWS
	//WIN64
	#ifdef _WIN64
		#ifndef WIN64
			#define WIN64
		#endif
		#define CM_X64
	#else
		#define CM_X86
	#endif //WIN64
#else
	//linux
	#define GKC_OS_LINUX
#endif //Machine Type
