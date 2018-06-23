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
This file contains main loop class for service.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __MAIN_LOOP_H__
#define __MAIN_LOOP_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

#define GKC_SERVICE_NAME  _S("GkcHostSvc")

class ServiceMainLoop
{
public:
	bool Prepare(const ConstArray<ConstStringS>& args) throw()
	{
		//environment variables
		try {
			m_strSystemRoot = StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager());  //may throw
			if( !EnvironmentVariableHelper::Query<CharS>(DECLARE_TEMP_CONST_STRING(ConstStringS, ENVVAR_GKC_SYSTEM_ROOT), m_strSystemRoot) )  //may throw
				return false;
			m_strLws = StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager());  //may throw
			if( !EnvironmentVariableHelper::Query<CharS>(DECLARE_TEMP_CONST_STRING(ConstStringS, ENVVAR_GKC_LOCAL_WORKSPACE), m_strLws) )  //may throw
				return false;
			m_strUws = StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager());  //may throw
			if( !EnvironmentVariableHelper::Query<CharS>(DECLARE_TEMP_CONST_STRING(ConstStringS, ENVVAR_GKC_UNIFIED_WORKSPACE), m_strUws) )  //may throw
				return false;
		}
		catch(...) {
			return false;
		}

		return true;
	}
	// return value : to be continued
	bool OneLoop() throw()
	{
		return false;
	}
	int Cleanup() throw()
	{
		return 0;
	}

private:
	StringS m_strSystemRoot;
	StringS m_strLws;
	StringS m_strUws;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
