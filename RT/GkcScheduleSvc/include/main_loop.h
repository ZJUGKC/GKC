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

////////////////////////////////////////////////////////////////////////////////

#define GKC_SERVICE_NAME   _S("GkcScheduleSvc")

class ServiceMainLoop
{
public:
	bool Prepare(const GKC::ConstArray<GKC::ConstStringS>& args) throw()
	{
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
};

////////////////////////////////////////////////////////////////////////////////
